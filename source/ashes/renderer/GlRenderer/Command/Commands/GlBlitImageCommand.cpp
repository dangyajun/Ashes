/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlBlitImageCommand.hpp"

#include "Command/Commands/GlCopyImageCommand.hpp"
#include "Command/Commands/GlCopyImageToBufferCommand.hpp"
#include "Command/Commands/GlCopyBufferToImageCommand.hpp"
#include "Core/GlDevice.hpp"
#include "Image/GlImage.hpp"
#include "Image/GlImageView.hpp"
#include "Miscellaneous/GlCallLogger.hpp"
#include "Miscellaneous/GlImageMemoryBinding.hpp"
#include "RenderPass/GlFrameBuffer.hpp"

#include "ashesgl_api.hpp"

namespace ashes::gl
{
	namespace
	{
		GlImageAspectFlags getMask( VkFormat format )
		{
			GlImageAspectFlags result = 0u;

			if ( isDepthFormat( format ) )
			{
				result |= GL_DEPTH_BUFFER_BIT;
			}

			if ( isStencilFormat( format ) )
			{
				result |= GL_STENCIL_BUFFER_BIT;
			}

			if ( !isDepthFormat( format )
				&& !isStencilFormat( format ) )
			{
				result |= GL_COLOR_BUFFER_BIT;
			}

			return result;
		}

		VkBufferImageCopy getImageToBufferCopy( VkImageBlit const & copyInfo
			, VkImage dstImage )
		{
			VkBufferImageCopy result{};
			auto & dstBinding = static_cast< ImageMemoryBinding const & >( get( dstImage )->getMemoryBinding() );
			result.bufferOffset = dstBinding.getMipLevelOffset( copyInfo.dstSubresource.mipLevel ) - dstBinding.getOffset();
			result.imageExtent.width = uint32_t( copyInfo.dstOffsets[1].x );
			result.imageExtent.height = uint32_t( copyInfo.dstOffsets[1].y );
			result.imageExtent.depth = uint32_t( copyInfo.dstOffsets[1].z );
			result.imageOffset = copyInfo.dstOffsets[0];
			result.imageSubresource = copyInfo.dstSubresource;
			return result;
		}
	}

	void buildBlitImageCommand( ContextStateStack & stack
		, VkDevice device
		, VkImage srcImage
		, VkImage dstImage
		, VkImageBlit region
		, VkFilter filter
		, CmdList & list )
	{
		glLogCommand( list, "BlitImageCommand" );
		assert( region.srcSubresource.layerCount == region.dstSubresource.layerCount
			|| region.srcSubresource.layerCount == uint32_t( region.dstOffsets[1].z )
			|| region.dstSubresource.layerCount == uint32_t( region.srcOffsets[1].z ) );

		auto srcLayerCount = region.srcSubresource.layerCount;
		auto dstLayerCount = region.dstSubresource.layerCount;
		auto srcSliceCount = region.srcOffsets[1].z;
		auto dstSliceCount = region.dstOffsets[1].z;
		float sliceRatio = float( srcSliceCount ) / float( dstSliceCount );
		auto layerCount = std::max( std::max( srcLayerCount, dstLayerCount )
			, uint32_t( std::min( srcSliceCount, dstSliceCount ) ) );
		auto srcBaseArrayLayer = region.srcSubresource.baseArrayLayer;
		auto dstBaseArrayLayer = region.dstSubresource.baseArrayLayer;
		auto srcBaseSlice = region.srcOffsets[0].z;
		auto dstBaseSlice = region.dstOffsets[0].z;

		for ( uint32_t layer = 0u; layer < layerCount; ++layer )
		{
			LayerCopy layerCopy{ device
				, region
				, srcImage
				, dstImage };

			list.push_back( makeCmd< OpType::eBindSrcFramebuffer >( GL_READ_FRAMEBUFFER ) );
			layerCopy.bindSrc( stack
				, srcBaseArrayLayer + layer
				, uint32_t( float( srcBaseSlice + layer ) * sliceRatio )
				, GL_READ_FRAMEBUFFER
				, list );
			list.push_back( makeCmd< OpType::eBindDstFramebuffer >( GL_DRAW_FRAMEBUFFER ) );
			layerCopy.bindDst( stack
				, dstBaseArrayLayer + layer
				, dstBaseSlice + layer
				, GL_DRAW_FRAMEBUFFER
				, list );
			list.push_back( makeCmd< OpType::eBlitFramebuffer >( layerCopy.region.srcOffsets[0].x
				, layerCopy.region.srcOffsets[0].y
				, layerCopy.region.srcOffsets[1].x
				, layerCopy.region.srcOffsets[1].y
				, layerCopy.region.dstOffsets[0].x
				, layerCopy.region.dstOffsets[0].y
				, layerCopy.region.dstOffsets[1].x
				, layerCopy.region.dstOffsets[1].y
				, getMask( get( srcImage )->getFormatVk() )
				, convert( filter ) ) );
			list.push_back( makeCmd< OpType::eBindFramebuffer >( GL_READ_FRAMEBUFFER
				, nullptr ) );
			list.push_back( makeCmd< OpType::eBindFramebuffer >( GL_DRAW_FRAMEBUFFER
				, nullptr ) );

			if ( stack.hasCurrentFramebuffer() )
			{
				stack.setCurrentFramebuffer( nullptr );
			}
		}

		if ( get( get( dstImage )->getMemoryBinding().getParent() )->getInternal() != GL_INVALID_INDEX )
		{
			buildCopyImageToBufferCommand( stack
				, device
				, getImageToBufferCopy( region, dstImage )
				, dstImage
				, get( dstImage )->getMemoryBinding()
				, list );
		}
	}
}
