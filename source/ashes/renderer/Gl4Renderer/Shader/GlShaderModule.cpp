/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Shader/GlShaderModule.hpp"

#include "Core/GlDevice.hpp"
#include "Core/GlPhysicalDevice.hpp"
#include "Core/GlInstance.hpp"
#include "Miscellaneous/GlValidator.hpp"

#include <iostream>
#include <regex>

#if Gl4Renderer_USE_SPIRV_CROSS
#	include "spirv_cross/spirv_cpp.hpp"
#	include "spirv_cross/spirv_cross_util.hpp"
#	include "spirv_cross/spirv_glsl.hpp"
#endif

#include "ashesgl4_api.hpp"

namespace ashes::gl4
{
	namespace
	{
		static uint32_t constexpr OpCodeSPIRV = 0x07230203;

		struct BlockLocale
		{
			BlockLocale()
			{
				m_prvLoc = std::locale( "" );

				if ( m_prvLoc.name() != "C" )
				{
					std::locale::global( std::locale{ "C" } );
				}
			}

			~BlockLocale()
			{
				if ( m_prvLoc.name() != "C" )
				{
					std::locale::global( m_prvLoc );
				}
			}

		private:
			std::locale m_prvLoc;
		};

		std::string retrieveLinkerLog( ContextLock const & context
			, GLuint programName )
		{
			std::string log;
			int infologLength = 0;
			int charsWritten = 0;
			glLogCall( context
				, glGetProgramiv
				, programName
				, GL_INFO_LOG_LENGTH
				, &infologLength );

			if ( infologLength > 0 )
			{
				std::vector< char > infoLog( infologLength + 1 );
				glLogCall( context
					, glGetProgramInfoLog
					, programName
					, infologLength
					, &charsWritten
					, infoLog.data() );
				log = infoLog.data();
			}

			if ( !log.empty() )
			{
				log = log.substr( 0, log.size() - 1 );
			}

			return log;
		}

#if Gl4Renderer_USE_SPIRV_CROSS

		spv::ExecutionModel getExecutionModel( VkShaderStageFlagBits stage )
		{
			spv::ExecutionModel result{};

			switch ( stage )
			{
			case VK_SHADER_STAGE_VERTEX_BIT:
				result = spv::ExecutionModelVertex;
				break;
			case VK_SHADER_STAGE_GEOMETRY_BIT:
				result = spv::ExecutionModelGeometry;
				break;
			case VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT:
				result = spv::ExecutionModelTessellationControl;
				break;
			case VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT:
				result = spv::ExecutionModelTessellationEvaluation;
				break;
			case VK_SHADER_STAGE_FRAGMENT_BIT:
				result = spv::ExecutionModelFragment;
				break;
			case VK_SHADER_STAGE_COMPUTE_BIT:
				result = spv::ExecutionModelGLCompute;
				break;
			default:
				assert( false && "Unsupported shader stage flag" );
				break;
			}

			return result;
		}

		void doFillConstant( VkSpecializationInfo const & specialisationInfo
			, VkSpecializationMapEntry const & entry
			, spirv_cross::SPIRType const & type
			, spirv_cross::SPIRConstant & constant )
		{
			auto offset = entry.offset;
			auto size = type.width;
			auto entrySize = uint32_t( entry.size / ( type.columns * type.vecsize ) );

			for ( auto col = 0u; col < type.columns; ++col )
			{
				for ( auto vec = 0u; vec < type.vecsize; ++vec )
				{
					std::memcpy( &constant.m.c[col].r[vec]
						, reinterpret_cast< uint8_t const * >( specialisationInfo.pData ) + offset
						, entrySize );
					offset += entrySize;
				}
			}
		}

		void doProcessSpecializationConstants( VkPipelineShaderStageCreateInfo const & state
			, spirv_cross::CompilerGLSL & compiler )
		{
			if ( state.pSpecializationInfo )
			{
				auto constants = compiler.get_specialization_constants();

				for ( auto itEntry = state.pSpecializationInfo->pMapEntries;
					itEntry != state.pSpecializationInfo->pMapEntries + state.pSpecializationInfo->mapEntryCount;
					++itEntry )
				{
					auto it = std::find_if( constants.begin()
						, constants.end()
						, [itEntry]( spirv_cross::SpecializationConstant const & lookup )
						{
							return lookup.constant_id == itEntry->constantID;
						} );

					if ( it != constants.end() )
					{
						auto & constant = compiler.get_constant( it->id );
						auto & type = compiler.get_type( constant.constant_type );
						doFillConstant( *state.pSpecializationInfo
							, *itEntry
							, type
							, constant );
					}
				}
			}
		}

		void doSetEntryPoint( VkShaderStageFlagBits stage
			, spirv_cross::CompilerGLSL & compiler )
		{
			auto model = getExecutionModel( stage );
			std::string entryPoint;

			for ( auto & e : compiler.get_entry_points_and_stages() )
			{
				if ( entryPoint.empty() && e.execution_model == model )
				{
					entryPoint = e.name;
				}
			}

			if ( entryPoint.empty() )
			{
				throw std::runtime_error{ "Could not find an entry point with stage: " + getShaderStageFlagsName( convertShaderStageFlag( stage ) ) };
			}

			compiler.set_entry_point( entryPoint, model );
		}

		void doSetupOptions( VkDevice device
			, spirv_cross::CompilerGLSL & compiler
			, bool invertY )
		{
			auto options = compiler.get_common_options();
			options.version = get( get( device )->getInstance() )->getExtensions().getShaderVersion();
			options.es = false;
			options.separate_shader_objects = true;
			options.enable_420pack_extension = true;
			options.vertex.fixup_clipspace = true;
			options.vertex.flip_vert_y = invertY;
			options.vertex.support_nonzero_base_instance = true;
			compiler.set_common_options( options );
		}

		ConstantFormat getIntFormat( uint32_t size )
		{
			switch ( size )
			{
			case 1:
				return ConstantFormat::eInt;
			case 2:
				return ConstantFormat::eVec2i;
			case 3:
				return ConstantFormat::eVec3i;
			case 4:
				return ConstantFormat::eVec4i;
			default:
				assert( false && "Unsupported row count" );
				return ConstantFormat::eInt;
			}
		}

		ConstantFormat getUIntFormat( uint32_t size )
		{
			switch ( size )
			{
			case 1:
				return ConstantFormat::eUInt;
			case 2:
				return ConstantFormat::eVec2ui;
			case 3:
				return ConstantFormat::eVec3ui;
			case 4:
				return ConstantFormat::eVec4ui;
			default:
				assert( false && "Unsupported row count" );
				return ConstantFormat::eUInt;
			}
		}

		ConstantFormat getFloatFormat( uint32_t size )
		{
			switch ( size )
			{
			case 1:
				return ConstantFormat::eFloat;
			case 2:
				return ConstantFormat::eVec2f;
			case 3:
				return ConstantFormat::eVec3f;
			case 4:
				return ConstantFormat::eVec4f;
			default:
				assert( false && "Unsupported row count" );
				return ConstantFormat::eFloat;
			}
		}

		ConstantFormat getFloatFormat( uint32_t cols
			, uint32_t rows )
		{
			switch ( cols )
			{
			case 1:
				return getFloatFormat( rows );
			case 2:
				return ConstantFormat::eMat2f;
			case 3:
				return ConstantFormat::eMat3f;
			case 4:
				return ConstantFormat::eMat4f;
			default:
				assert( false && "Unsupported column count" );
				return ConstantFormat::eFloat;
			}
		}

		ConstantFormat getFormat( spirv_cross::SPIRType const & type )
		{
			switch ( type.basetype )
			{
			case spirv_cross::SPIRType::Int:
				return getIntFormat( type.vecsize );

			case spirv_cross::SPIRType::UInt:
				return getUIntFormat( type.vecsize );

			case spirv_cross::SPIRType::Float:
				return getFloatFormat( type.columns, type.vecsize );

			default:
				assert( false && "Unsupported SPIRType" );
				return ConstantFormat::eVec4f;
			}
		}

		uint32_t getArraySize( spirv_cross::CompilerGLSL & compiler
			, spirv_cross::SPIRType const & type )
		{
			return !type.array.empty()
				? ( type.array_size_literal[0]
					? type.array[0]
					: uint32_t( compiler.get_constant( type.array[0] ).scalar_u64() ) )
				: 0u;
		}

		ConstantsLayout doRetrievePushConstants( spirv_cross::CompilerGLSL & compiler
			, VkShaderStageFlagBits shaderStage )
		{
			ConstantsLayout result;
			spirv_cross::ShaderResources resources = compiler.get_shader_resources();

			for ( auto & pcb : resources.push_constant_buffers )
			{
				auto & pcbType = compiler.get_type( pcb.type_id );
				assert( pcbType.parent_type );
				auto & structType = compiler.get_type( pcbType.parent_type );

				uint32_t index = 0u;

				for ( auto & mbrTypeId : structType.member_types )
				{
					spirv_cross::SPIRType const & mbrType = compiler.get_type( mbrTypeId );
					uint32_t offset = compiler.get_member_decoration( structType.self, index, spv::Decoration::DecorationOffset );

					result.push_back( ConstantDesc
						{
							0u,
							shaderStage,
							compiler.get_name( pcb.id ) + "." + compiler.get_member_name( structType.self, index ),
							0u,
							getFormat( mbrType ),
							getSize( getFormat( mbrType ) ),
							getArraySize( compiler, mbrType ),
							offset,
						} );

					if ( result.back().arraySize > 0 )
					{
						result.back().name += "[0]";
					}

					++index;
				}
			}

			return result;
		}

		void reportMissingBinding( VkDevice device
			, VkShaderModule module
			, std::string const & typeName
			, uint32_t binding
			, uint32_t set )
		{
			assert( false );
			std::stringstream stream;
			stream.imbue( std::locale{ "C" } );
			stream << typeName << ", binding=" << binding << ", set=" << set;
			reportError( module
				, VK_ERROR_VALIDATION_FAILED_EXT
				, "Missing binding"
				, stream.str() );
		}

		void doReworkBindings( VkDevice device
			, VkShaderModule module
			, std::string const & typeName
			, spirv_cross::CompilerGLSL & compiler
			, spirv_cross::SmallVector< spirv_cross::Resource > & resources
			, ShaderBindingMap const & bindings
			, bool failOnError
			, ShaderBindingMap const * fallback = nullptr )
		{
			for ( auto & obj : resources )
			{
				auto binding = compiler.get_decoration( obj.id, spv::DecorationBinding );
				auto set = compiler.get_decoration( obj.id, spv::DecorationDescriptorSet );
				compiler.unset_decoration( obj.id, spv::DecorationDescriptorSet );
				auto it = bindings.find( makeShaderBindingKey( set, binding ) );

				if ( it != bindings.end() )
				{
					compiler.set_decoration( obj.id, spv::DecorationBinding, it->second );
				}
				else if ( fallback )
				{
					it = fallback->find( makeShaderBindingKey( set, binding ) );

					if ( it != fallback->end() )
					{
						compiler.set_decoration( obj.id, spv::DecorationBinding, it->second );
					}
					else if ( failOnError )
					{
						reportMissingBinding( device, module, typeName, binding, set );
					}
				}
				else if ( failOnError )
				{
					reportMissingBinding( device, module, typeName, binding, set );
				}
			}
		}

		void doReworkBindings( VkPipelineLayout pipelineLayout
			, VkPipelineCreateFlags createFlags
			, VkShaderModule module
			, spirv_cross::CompilerGLSL & compiler )
		{
			uint32_t const ssboMask = ( 1u << 16u );
			spirv_cross::ShaderResources resources = compiler.get_shader_resources();
			auto & bindings = get( pipelineLayout )->getShaderBindings();
			auto device = get( pipelineLayout )->getDevice();
			auto failOnError = !( checkFlag( createFlags, VK_PIPELINE_CREATE_ALLOW_DERIVATIVES_BIT )
				|| checkFlag( createFlags, VK_PIPELINE_CREATE_DERIVATIVE_BIT ) );
			doReworkBindings( device, module, "UniformBuffer", compiler, resources.uniform_buffers, bindings.ubo, failOnError );
			doReworkBindings( device, module, "StorageBuffer", compiler, resources.storage_buffers, bindings.sbo, failOnError );
			doReworkBindings( device, module, "CombinedSamplerImage", compiler, resources.sampled_images, bindings.tex, failOnError, &bindings.tbo );
			doReworkBindings( device, module, "SampledImage", compiler, resources.separate_images, bindings.tex, failOnError );
			doReworkBindings( device, module, "Sampler", compiler, resources.separate_samplers, bindings.tex, failOnError );
			doReworkBindings( device, module, "StorageImage", compiler, resources.storage_images, bindings.img, failOnError );
			doReworkBindings( device, module, "SubpassInput", compiler, resources.subpass_inputs, bindings.tex, failOnError );
		}

#endif

		std::string compileSpvToGlsl( VkDevice device
			, VkPipelineLayout pipelineLayout
			, VkPipelineCreateFlags createFlags
			, VkShaderModule module
			, UInt32Array const & shader
			, VkShaderStageFlagBits stage
			, VkPipelineShaderStageCreateInfo const & state
			, bool invertY
			, ConstantsLayout & constants
			, bool & isGlsl )
		{
			if ( shader[0] == OpCodeSPIRV )
			{
				isGlsl = false;
#if Gl4Renderer_USE_SPIRV_CROSS
				BlockLocale guard;
				spirv_cross::CompilerGLSL compiler{ shader };
				doProcessSpecializationConstants( state, compiler );
				doSetEntryPoint( stage, compiler );
				doSetupOptions( device, compiler, invertY );
				constants = doRetrievePushConstants( compiler, stage );
				doReworkBindings( pipelineLayout, createFlags, module, compiler );
				return compiler.compile();
#else
				throw std::runtime_error{ "Can't parse SPIR-V shaders, pull submodule SpirvCross" };
#endif
			}

			isGlsl = true;
			std::vector< char > glslCode( shader.size() * sizeof( uint32_t ) );
			std::memcpy( glslCode.data(), shader.data(), glslCode.size() );
			return std::string( glslCode.data(), glslCode.data() + strlen( glslCode.data() ) );
		}
	}

	ShaderModule::ShaderModule( VkDevice device
		, VkShaderModuleCreateInfo createInfo )
		: m_device{ device }
		, m_flags{ createInfo.flags }
		, m_code{ UInt32Array( createInfo.pCode, createInfo.pCode + ( createInfo.codeSize / sizeof( uint32_t ) ) ) }
	{
	}

	ShaderDesc ShaderModule::compile( VkPipelineShaderStageCreateInfo const & state
		, VkPipelineLayout pipelineLayout
		, VkPipelineCreateFlags createFlags
		, bool invertY )
	{
		auto context = get( m_device )->getContext();
		ShaderDesc result{};
		bool isGlsl;
		m_source = compileSpvToGlsl( m_device
			, pipelineLayout
			, createFlags
			, get( this )
			, m_code
			, state.stage
			, state
			, invertY
			, m_constants
			, isGlsl );

		char const * data = m_source.data();
		auto program = glLogNonVoidCall( context
			, glCreateShaderProgramv
			, getShaderStage( state.stage )
			, 1u
			, &data );
		auto linkerLog = retrieveLinkerLog( context, program );

		if ( linkerLog.find( "ERROR" ) == std::string::npos )
		{
			int validated = 0;
			glLogCall( context
				, glGetProgramiv
				, program
				, GL_INFO_VALIDATE_STATUS
				, &validated );

			if ( !linkerLog.empty()
				|| !validated )
			{
				std::stringstream stream;
				stream << "Shader program link: " << std::endl;

				if ( !validated )
				{
					stream << "Not validated - " << std::endl;
				}

				if ( !linkerLog.empty() )
				{
					stream << linkerLog << std::endl;
				}

				get( get( m_device )->getInstance() )->reportMessage( VK_DEBUG_REPORT_WARNING_BIT_EXT
					, VK_DEBUG_REPORT_OBJECT_TYPE_SHADER_MODULE_EXT
					, uint64_t( get( this ) )
					, 0u
					, 0u
					, "CORE"
					, stream.str().c_str() );
			}

			for ( auto & constant : m_constants )
			{
				constant.program = program;
			}

			result = getShaderDesc( context, state.stage, program );
			result.isGlsl = isGlsl;

			if ( !m_constants.empty() )
			{
				assert( m_constants.size() >= result.pcb.size() );
				for ( auto & constant : result.pcb )
				{
					auto it = std::find_if( m_constants.begin()
						, m_constants.end()
						, [&constant]( ConstantDesc const & lookup )
						{
							return lookup.name == constant.name;
						} );
					assert( it != m_constants.end() );
					constant.stageFlag = state.stage;
					constant.program = program;
					constant.offset = it->offset;
				}
			}
			else
			{
				uint32_t offset = 0u;

				for ( auto & constant : result.pcb )
				{
					constant.offset = offset;
					constant.stageFlag = state.stage;
					offset += constant.size;
				}
			}

			result.stageFlags = state.stage;
		}
		else
		{
			std::stringstream stream;
			stream << "Shader program link: " << std::endl;

			if ( !linkerLog.empty() )
			{
				stream << linkerLog << std::endl;
			}

			get( get( m_device )->getInstance() )->reportMessage( VK_DEBUG_REPORT_ERROR_BIT_EXT
				, VK_DEBUG_REPORT_OBJECT_TYPE_SHADER_MODULE_EXT
				, uint64_t( get( this ) )
				, 0u
				, 0u
				, "CORE"
				, stream.str().c_str() );
			glLogCall( context
				, glDeleteProgram
				, program );
			program = 0;
		}

		result.program = program;
		return result;
	}
}
