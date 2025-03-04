/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/GlQueue.hpp"

#include "Miscellaneous/GlCallLogger.hpp"

#include "Command/GlCommandBuffer.hpp"
#include "Command/Commands/GlBeginQueryCommand.hpp"
#include "Command/Commands/GlBeginRenderPassCommand.hpp"
#include "Command/Commands/GlBeginSubpassCommand.hpp"
#include "Command/Commands/GlBindComputePipelineCommand.hpp"
#include "Command/Commands/GlBindDescriptorSetCommand.hpp"
#include "Command/Commands/GlBindGeometryBuffersCommand.hpp"
#include "Command/Commands/GlBindPipelineCommand.hpp"
#include "Command/Commands/GlBlitImageCommand.hpp"
#include "Command/Commands/GlClearAttachmentsCommand.hpp"
#include "Command/Commands/GlClearColourCommand.hpp"
#include "Command/Commands/GlClearColourFboCommand.hpp"
#include "Command/Commands/GlClearDepthStencilCommand.hpp"
#include "Command/Commands/GlClearDepthStencilFboCommand.hpp"
#include "Command/Commands/GlCopyBufferCommand.hpp"
#include "Command/Commands/GlCopyBufferToImageCommand.hpp"
#include "Command/Commands/GlCopyImageCommand.hpp"
#include "Command/Commands/GlCopyImageToBufferCommand.hpp"
#include "Command/Commands/GlDispatchCommand.hpp"
#include "Command/Commands/GlDispatchIndirectCommand.hpp"
#include "Command/Commands/GlDrawCommand.hpp"
#include "Command/Commands/GlDrawIndexedCommand.hpp"
#include "Command/Commands/GlDrawIndexedIndirectCommand.hpp"
#include "Command/Commands/GlDrawIndirectCommand.hpp"
#include "Command/Commands/GlEndQueryCommand.hpp"
#include "Command/Commands/GlGenerateMipmapsCommand.hpp"
#include "Command/Commands/GlMemoryBarrierCommand.hpp"
#include "Command/Commands/GlPushConstantsCommand.hpp"
#include "Command/Commands/GlResetEventCommand.hpp"
#include "Command/Commands/GlSetDepthBiasCommand.hpp"
#include "Command/Commands/GlSetEventCommand.hpp"
#include "Command/Commands/GlSetLineWidthCommand.hpp"
#include "Command/Commands/GlWaitEventsCommand.hpp"
#include "Command/Commands/GlWriteTimestampCommand.hpp"
#include "Core/GlDevice.hpp"
#include "Sync/GlFence.hpp"
#include "Sync/GlSemaphore.hpp"
#include "Core/GlSwapChain.hpp"

#include "ashesgl_api.hpp"

namespace ashes::gl
{
	namespace
	{
		void applyCmd( ContextLock const & lock, Command const & cmd )
		{
			switch ( cmd.op.type )
			{
			case OpType::eActiveTexture:
				apply( lock, map< OpType::eActiveTexture >( cmd ) );
				break;
			case OpType::eApplyDepthRanges:
				apply( lock, map< OpType::eApplyDepthRanges >( cmd ) );
				break;
			case OpType::eApplyScissor:
				apply( lock, map< OpType::eApplyScissor >( cmd ) );
				break;
			case OpType::eApplyScissors:
				apply( lock, map< OpType::eApplyScissors >( cmd ) );
				break;
			case OpType::eApplyViewport:
				apply( lock, map< OpType::eApplyViewport >( cmd ) );
				break;
			case OpType::eApplyViewports:
				apply( lock, map< OpType::eApplyViewports >( cmd ) );
				break;
			case OpType::eBeginQuery:
				apply( lock, map< OpType::eBeginQuery >( cmd ) );
				break;
			case OpType::eBindBuffer:
				apply( lock, map< OpType::eBindBuffer >( cmd ) );
				break;
			case OpType::eBindBufferRange:
				apply( lock, map< OpType::eBindBufferRange >( cmd ) );
				break;
			case OpType::eBindContextState:
				apply( lock, map< OpType::eBindContextState >( cmd ) );
				break;
			case OpType::eBindFramebuffer:
				apply( lock, map< OpType::eBindFramebuffer >( cmd ) );
				break;
			case OpType::eBindSrcFramebuffer:
				apply( lock, map< OpType::eBindSrcFramebuffer >( cmd ) );
				break;
			case OpType::eBindDstFramebuffer:
				apply( lock, map< OpType::eBindDstFramebuffer >( cmd ) );
				break;
			case OpType::eBindImage:
				apply( lock, map< OpType::eBindImage >( cmd ) );
				break;
			case OpType::eBindSampler:
				apply( lock, map< OpType::eBindSampler >( cmd ) );
				break;
			case OpType::eBindTexture:
				apply( lock, map< OpType::eBindTexture >( cmd ) );
				break;
			case OpType::eBindVextexArray:
				apply( lock, map< OpType::eBindVextexArray >( cmd ) );
				break;
			case OpType::eBlendConstants:
				apply( lock, map< OpType::eBlendConstants >( cmd ) );
				break;
			case OpType::eBlendEquation:
				apply( lock, map< OpType::eBlendEquation >( cmd ) );
				break;
			case OpType::eBlendFunc:
				apply( lock, map< OpType::eBlendFunc >( cmd ) );
				break;
			case OpType::eBlitFramebuffer:
				apply( lock, map< OpType::eBlitFramebuffer >( cmd ) );
				break;
			case OpType::eCheckFramebuffer:
				apply( lock, map< OpType::eCheckFramebuffer >( cmd ) );
				break;
			case OpType::eClearBack:
				apply( lock, map< OpType::eClearBack >( cmd ) );
				break;
			case OpType::eClearBackColour:
				apply( lock, map< OpType::eClearBackColour >( cmd ) );
				break;
			case OpType::eClearBackDepth:
				apply( lock, map< OpType::eClearBackDepth >( cmd ) );
				break;
			case OpType::eClearBackDepthStencil:
				apply( lock, map< OpType::eClearBackDepthStencil >( cmd ) );
				break;
			case OpType::eClearBackStencil:
				apply( lock, map< OpType::eClearBackStencil >( cmd ) );
				break;
			case OpType::eClearColour:
				apply( lock, map< OpType::eClearColour >( cmd ) );
				break;
			case OpType::eClearDepth:
				apply( lock, map< OpType::eClearDepth >( cmd ) );
				break;
			case OpType::eClearDepthStencil:
				apply( lock, map< OpType::eClearDepthStencil >( cmd ) );
				break;
			case OpType::eClearStencil:
				apply( lock, map< OpType::eClearStencil >( cmd ) );
				break;
			case OpType::eClearTexColorF:
				apply( lock, map< OpType::eClearTexColorF >( cmd ) );
				break;
			case OpType::eClearTexColorSI:
				apply( lock, map< OpType::eClearTexColorSI >( cmd ) );
				break;
			case OpType::eClearTexColorUI:
				apply( lock, map< OpType::eClearTexColorUI >( cmd ) );
				break;
			case OpType::eClearTexDepth:
				apply( lock, map< OpType::eClearTexDepth >( cmd ) );
				break;
			case OpType::eClearTexDepthStencil:
				apply( lock, map< OpType::eClearTexDepthStencil >( cmd ) );
				break;
			case OpType::eClearTexStencil:
				apply( lock, map< OpType::eClearTexStencil >( cmd ) );
				break;
			case OpType::eColorMask:
				apply( lock, map< OpType::eColorMask >( cmd ) );
				break;
			case OpType::eCompressedTexSubImage1D:
				apply( lock, map< OpType::eCompressedTexSubImage1D >( cmd ) );
				break;
			case OpType::eCompressedTexSubImage2D:
				apply( lock, map< OpType::eCompressedTexSubImage2D >( cmd ) );
				break;
			case OpType::eCompressedTexSubImage3D:
				apply( lock, map< OpType::eCompressedTexSubImage3D >( cmd ) );
				break;
			case OpType::eCopyBufferSubData:
				apply( lock, map< OpType::eCopyBufferSubData >( cmd ) );
				break;
			case OpType::eCopyImageSubData:
				apply( lock, map< OpType::eCopyImageSubData >( cmd ) );
				break;
			case OpType::eCullFace:
				apply( lock, map< OpType::eCullFace >( cmd ) );
				break;
			case OpType::eDepthFunc:
				apply( lock, map< OpType::eDepthFunc >( cmd ) );
				break;
			case OpType::eDepthMask:
				apply( lock, map< OpType::eDepthMask >( cmd ) );
				break;
			case OpType::eDepthRange:
				apply( lock, map< OpType::eDepthRange >( cmd ) );
				break;
			case OpType::eDisable:
				apply( lock, map< OpType::eDisable >( cmd ) );
				break;
			case OpType::eDispatch:
				apply( lock, map< OpType::eDispatch >( cmd ) );
				break;
			case OpType::eDispatchIndirect:
				apply( lock, map< OpType::eDispatchIndirect >( cmd ) );
				break;
			case OpType::eDownloadMemory:
				apply( lock, map< OpType::eDownloadMemory >( cmd ) );
				break;
			case OpType::eDraw:
				apply( lock, map< OpType::eDraw >( cmd ) );
				break;
			case OpType::eDrawBaseInstance:
				apply( lock, map< OpType::eDrawBaseInstance >( cmd ) );
				break;
			case OpType::eDrawBuffer:
				apply( lock, map< OpType::eDrawBuffer >( cmd ) );
				break;
			case OpType::eDrawBuffers:
				apply( lock, map< OpType::eDrawBuffers >( cmd ) );
				break;
			case OpType::eDrawIndexed:
				apply( lock, map< OpType::eDrawIndexed >( cmd ) );
				break;
			case OpType::eDrawIndexedBaseInstance:
				apply( lock, map< OpType::eDrawIndexedBaseInstance >( cmd ) );
				break;
			case OpType::eDrawIndexedIndirect:
				apply( lock, map< OpType::eDrawIndexedIndirect >( cmd ) );
				break;
			case OpType::eDrawIndirect:
				apply( lock, map< OpType::eDrawIndirect >( cmd ) );
				break;
			case OpType::eEnable:
				apply( lock, map< OpType::eEnable >( cmd ) );
				break;
			case OpType::eEndQuery:
				apply( lock, map< OpType::eEndQuery >( cmd ) );
				break;
			case OpType::eFillBuffer:
				apply( lock, map< OpType::eFillBuffer >( cmd ) );
				break;
			case OpType::eFramebufferTexture:
				apply( lock, map< OpType::eFramebufferTexture >( cmd ) );
				break;
			case OpType::eFramebufferTexture1D:
				apply( lock, map< OpType::eFramebufferTexture1D >( cmd ) );
				break;
			case OpType::eFramebufferTexture2D:
				apply( lock, map< OpType::eFramebufferTexture2D >( cmd ) );
				break;
			case OpType::eFramebufferTexture3D:
				apply( lock, map< OpType::eFramebufferTexture3D >( cmd ) );
				break;
			case OpType::eFramebufferTextureLayer:
				apply( lock, map< OpType::eFramebufferTextureLayer >( cmd ) );
				break;
			case OpType::eFrontFace:
				apply( lock, map< OpType::eFrontFace >( cmd ) );
				break;
			case OpType::eGenerateMipmaps:
				apply( lock, map< OpType::eGenerateMipmaps >( cmd ) );
				break;
			case OpType::eGetCompressedTexImage:
				apply( lock, map< OpType::eGetCompressedTexImage >( cmd ) );
				break;
			case OpType::eGetTexImage:
				apply( lock, map< OpType::eGetTexImage >( cmd ) );
				break;
			case OpType::eGetQueryResults:
				apply( lock, map< OpType::eGetQueryResults >( cmd ) );
				break;
			case OpType::eLineWidth:
				apply( lock, map< OpType::eLineWidth >( cmd ) );
				break;
			case OpType::eLogCommand:
				apply( lock, map< OpType::eLogCommand >( cmd ) );
				break;
			case OpType::eLogicOp:
				apply( lock, map< OpType::eLogicOp >( cmd ) );
				break;
			case OpType::eMemoryBarrier:
				apply( lock, map< OpType::eMemoryBarrier >( cmd ) );
				break;
			case OpType::eMinSampleShading:
				apply( lock, map< OpType::eMinSampleShading >( cmd ) );
				break;
			case OpType::ePatchParameter:
				apply( lock, map< OpType::ePatchParameter >( cmd ) );
				break;
			case OpType::ePixelStore:
				apply( lock, map< OpType::ePixelStore >( cmd ) );
				break;
			case OpType::ePolygonMode:
				apply( lock, map< OpType::ePolygonMode >( cmd ) );
				break;
			case OpType::ePolygonOffset:
				apply( lock, map< OpType::ePolygonOffset >( cmd ) );
				break;
			case OpType::ePopDebugGroup:
				apply( lock, map< OpType::ePopDebugGroup >( cmd ) );
				break;
			case OpType::ePrimitiveRestartIndex:
				apply( lock, map< OpType::ePrimitiveRestartIndex >( cmd ) );
				break;
			case OpType::eProgramUniform1fv:
				apply( lock, map< OpType::eProgramUniform1fv >( cmd ) );
				break;
			case OpType::eProgramUniform2fv:
				apply( lock, map< OpType::eProgramUniform2fv >( cmd ) );
				break;
			case OpType::eProgramUniform3fv:
				apply( lock, map< OpType::eProgramUniform3fv >( cmd ) );
				break;
			case OpType::eProgramUniform4fv:
				apply( lock, map< OpType::eProgramUniform4fv >( cmd ) );
				break;
			case OpType::eProgramUniform1iv:
				apply( lock, map< OpType::eProgramUniform1iv >( cmd ) );
				break;
			case OpType::eProgramUniform2iv:
				apply( lock, map< OpType::eProgramUniform2iv >( cmd ) );
				break;
			case OpType::eProgramUniform3iv:
				apply( lock, map< OpType::eProgramUniform3iv >( cmd ) );
				break;
			case OpType::eProgramUniform4iv:
				apply( lock, map< OpType::eProgramUniform4iv >( cmd ) );
				break;
			case OpType::eProgramUniform1uiv:
				apply( lock, map< OpType::eProgramUniform1uiv >( cmd ) );
				break;
			case OpType::eProgramUniform2uiv:
				apply( lock, map< OpType::eProgramUniform2uiv >( cmd ) );
				break;
			case OpType::eProgramUniform3uiv:
				apply( lock, map< OpType::eProgramUniform3uiv >( cmd ) );
				break;
			case OpType::eProgramUniform4uiv:
				apply( lock, map< OpType::eProgramUniform4uiv >( cmd ) );
				break;
			case OpType::eProgramUniformMatrix2fv:
				apply( lock, map< OpType::eProgramUniformMatrix2fv >( cmd ) );
				break;
			case OpType::eProgramUniformMatrix3fv:
				apply( lock, map< OpType::eProgramUniformMatrix3fv >( cmd ) );
				break;
			case OpType::eProgramUniformMatrix4fv:
				apply( lock, map< OpType::eProgramUniformMatrix4fv >( cmd ) );
				break;
			case OpType::ePushDebugGroup:
				apply( lock, map< OpType::ePushDebugGroup >( cmd ) );
				break;
			case OpType::eReadBuffer:
				apply( lock, map< OpType::eReadBuffer >( cmd ) );
				break;
			case OpType::eReadPixels:
				apply( lock, map< OpType::eReadPixels >( cmd ) );
				break;
			case OpType::eResetEvent:
				apply( lock, map< OpType::eResetEvent >( cmd ) );
				break;
			case OpType::eSetEvent:
				apply( lock, map< OpType::eSetEvent >( cmd ) );
				break;
			case OpType::eSetLineWidth:
				apply( lock, map< OpType::eSetLineWidth >( cmd ) );
				break;
			case OpType::eStencilFunc:
				apply( lock, map< OpType::eStencilFunc >( cmd ) );
				break;
			case OpType::eStencilMask:
				apply( lock, map< OpType::eStencilMask >( cmd ) );
				break;
			case OpType::eStencilOp:
				apply( lock, map< OpType::eStencilOp >( cmd ) );
				break;
			case OpType::eTexParameteri:
				apply( lock, map< OpType::eTexParameteri >( cmd ) );
				break;
			case OpType::eTexParameterf:
				apply( lock, map< OpType::eTexParameterf >( cmd ) );
				break;
			case OpType::eTexSubImage1D:
				apply( lock, map< OpType::eTexSubImage1D >( cmd ) );
				break;
			case OpType::eTexSubImage2D:
				apply( lock, map< OpType::eTexSubImage2D >( cmd ) );
				break;
			case OpType::eTexSubImage3D:
				apply( lock, map< OpType::eTexSubImage3D >( cmd ) );
				break;
			case OpType::eUniform1fv:
				apply( lock, map< OpType::eUniform1fv >( cmd ) );
				break;
			case OpType::eUniform2fv:
				apply( lock, map< OpType::eUniform2fv >( cmd ) );
				break;
			case OpType::eUniform3fv:
				apply( lock, map< OpType::eUniform3fv >( cmd ) );
				break;
			case OpType::eUniform4fv:
				apply( lock, map< OpType::eUniform4fv >( cmd ) );
				break;
			case OpType::eUniform1iv:
				apply( lock, map< OpType::eUniform1iv >( cmd ) );
				break;
			case OpType::eUniform2iv:
				apply( lock, map< OpType::eUniform2iv >( cmd ) );
				break;
			case OpType::eUniform3iv:
				apply( lock, map< OpType::eUniform3iv >( cmd ) );
				break;
			case OpType::eUniform4iv:
				apply( lock, map< OpType::eUniform4iv >( cmd ) );
				break;
			case OpType::eUniform1uiv:
				apply( lock, map< OpType::eUniform1uiv >( cmd ) );
				break;
			case OpType::eUniform2uiv:
				apply( lock, map< OpType::eUniform2uiv >( cmd ) );
				break;
			case OpType::eUniform3uiv:
				apply( lock, map< OpType::eUniform3uiv >( cmd ) );
				break;
			case OpType::eUniform4uiv:
				apply( lock, map< OpType::eUniform4uiv >( cmd ) );
				break;
			case OpType::eUniformMatrix2fv:
				apply( lock, map< OpType::eUniformMatrix2fv >( cmd ) );
				break;
			case OpType::eUniformMatrix3fv:
				apply( lock, map< OpType::eUniformMatrix3fv >( cmd ) );
				break;
			case OpType::eUniformMatrix4fv:
				apply( lock, map< OpType::eUniformMatrix4fv >( cmd ) );
				break;
			case OpType::eUpdateBuffer:
				apply( lock, map< OpType::eUpdateBuffer >( cmd ) );
				break;
			case OpType::eUploadMemory:
				apply( lock, map< OpType::eUploadMemory >( cmd ) );
				break;
			case OpType::eUseProgram:
				apply( lock, map< OpType::eUseProgram >( cmd ) );
				break;
			case OpType::eUseProgramPipeline:
				apply( lock, map< OpType::eUseProgramPipeline >( cmd ) );
				break;
			case OpType::eWaitEvents:
				apply( lock, map< OpType::eWaitEvents >( cmd ) );
				break;
			case OpType::eWriteTimestamp:
				apply( lock, map< OpType::eWriteTimestamp >( cmd ) );
				break;
			default:
				assert( false && "Unsupported command type." );
				break;
			}
		}
	}

	void applyList( ContextLock const & lock
		, CmdList const & cmds )
	{
		Command const * pCmd = nullptr;

		for ( CmdBuffer const & cmdBuf : cmds )
		{
			auto it = cmdBuf.begin();

			if ( map( it, cmdBuf.end(), pCmd ) )
			{
				auto & cmd = *pCmd;
				applyCmd( lock, cmd );
			}
		}
	}

	void applyBuffer( ContextLock const & lock
		, CmdBuffer const & cmds )
	{
		auto it = cmds.begin();
		auto end = cmds.end();
		Command const * pCmd = nullptr;

		while ( map( it, end, pCmd ) )
		{
			auto & cmd = *pCmd;
			it += cmd.op.size;
			applyCmd( lock, cmd );
		}
	}

	Queue::Queue( VkAllocationCallbacks const * allocInfo
		, VkDevice device
		, VkDeviceQueueCreateInfo createInfo )
		: m_device{ device }
		, m_createInfo{ std::move( createInfo ) }
	{
		registerObject( m_device, *this );
	}

	Queue::~Queue()
	{
		unregisterObject( m_device, *this );
	}

	VkResult Queue::submit( VkSubmitInfoArray const & values
		, VkFence fence )const
	{
		try
		{
			auto context = get( m_device )->getContext();

			for ( auto & value : values )
			{
				for ( auto commandBuffer : makeArrayView( value.pCommandBuffers, value.commandBufferCount ) )
				{
					auto & glCommandBuffer = *get( commandBuffer );
					glCommandBuffer.initialiseGeometryBuffers( context );
					applyBuffer( context, glCommandBuffer.getCmds() );
					applyBuffer( context, glCommandBuffer.getCmdsAfterSubmit() );
				}
			}

			if ( fence )
			{
				get( fence )->insert( context );
			}

			return VK_SUCCESS;
		}
		catch ( Exception & exc )
		{
			return exc.getResult();
		}
		catch ( ... )
		{
			return VK_ERROR_DEVICE_LOST;
		}
	}

	VkResult Queue::present( VkPresentInfoKHR const & presentInfo )const
	{
		try
		{
			auto itIndex = presentInfo.pImageIndices;

			if ( presentInfo.pResults )
			{
				auto itResult = presentInfo.pResults;

				for ( auto itSwapchain = presentInfo.pSwapchains;
					itSwapchain != presentInfo.pSwapchains + presentInfo.swapchainCount;
					++itIndex, ++itResult, ++itSwapchain )
				{
					*itResult = get( *itSwapchain )->present( *itIndex );
				}
			}
			else
			{
				for ( auto itSwapchain = presentInfo.pSwapchains;
					itSwapchain != presentInfo.pSwapchains + presentInfo.swapchainCount;
					++itIndex, ++itSwapchain )
				{
					get( *itSwapchain )->present( *itIndex );
				}
			}

			return VK_SUCCESS;
		}
		catch ( Exception & exc )
		{
			return exc.getResult();
		}
		catch ( ... )
		{
			return VK_ERROR_DEVICE_LOST;
		}
	}

	VkResult Queue::bindSparse( ArrayView< VkBindSparseInfo const > values
		, VkFence fence )const
	{
		return VK_ERROR_FEATURE_NOT_PRESENT;
	}

	VkResult Queue::waitIdle()const
	{
		try
		{
			auto context = get( m_device )->getContext();
			logDebug( "*** vkQueueWaitIdle ***" );
			glLogEmptyCall( context
				, glFinish );
			return VK_SUCCESS;
		}
		catch ( Exception & exc )
		{
			return exc.getResult();
		}
		catch ( ... )
		{
			return VK_ERROR_DEVICE_LOST;
		}
	}

#if VK_EXT_debug_utils

	void Queue::beginDebugUtilsLabel( VkDebugUtilsLabelEXT const & labelInfo )const
	{
		m_label = DebugLabel
		{
			{ labelInfo.color[0], labelInfo.color[1], labelInfo.color[2], labelInfo.color[3] },
			labelInfo.pLabelName,
		};
	}

	void Queue::endDebugUtilsLabel()const
	{
		m_label = ashes::nullopt;
	}

	void Queue::insertDebugUtilsLabel( VkDebugUtilsLabelEXT const & labelInfo )const
	{
		m_label = DebugLabel
		{
			{ labelInfo.color[0], labelInfo.color[1], labelInfo.color[2], labelInfo.color[3] },
			labelInfo.pLabelName,
		};
	}

#endif
}
