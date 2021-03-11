/**
*\file
*	Image.h
*\author
*	Sylvain Doremus
*/
#ifndef ___GlRenderer_TextureView_HPP___
#define ___GlRenderer_TextureView_HPP___
#pragma once

#include "renderer/GlRenderer/GlRendererPrerequisites.hpp"
#include "renderer/GlRenderer/Enum/GlTextureViewType.hpp"

namespace ashes::gl
{
	namespace gl3
	{
		GlTextureType convert( VkImageViewType viewType
			, VkImageType imageType
			, VkImageCreateFlags flags
			, VkSampleCountFlagBits samples
			, uint32_t baseArrayLayer );
	}

	class ImageView
		: public IcdObject
	{
	private:
		ImageView( VkAllocationCallbacks const * allocInfo
			, VkDevice device
			, VkImageViewCreateInfo createInfo
			, bool createView );

	public:
		ImageView( VkAllocationCallbacks const * allocInfo
			, VkDevice device
			, VkImage image );
		ImageView( VkAllocationCallbacks const * allocInfo
			, VkDevice device
			, VkImageViewCreateInfo createInfo );
		~ImageView();

		GLuint getInternal()const noexcept;

		inline GlTextureType getTextureType()const noexcept
		{
			return m_gltextureType;
		}

		inline gl4::GlTextureViewType getViewType()const noexcept
		{
			return m_glviewType;
		}

		inline VkImageViewType getType()const noexcept
		{
			return m_createInfo.viewType;
		}

		inline VkFormat getFormatVk()const noexcept
		{
			return m_createInfo.format;
		}

		inline GlInternal getInternalFormat()const noexcept
		{
			return m_pixelFormat.internal;
		}

		inline GlFormat getDrawFormat()const noexcept
		{
			return m_pixelFormat.drawFormat;
		}

		inline GlType getDrawType()const noexcept
		{
			return m_pixelFormat.drawType;
		}

		inline GlFormat getGetFormat()const noexcept
		{
			return m_pixelFormat.getFormat;
		}

		inline GlType getGetType()const noexcept
		{
			return m_pixelFormat.getType;
		}

		inline bool isReadSupported()const noexcept
		{
			return m_pixelFormat.readSupport;
		}

		inline GlFormat getReadFormat()const noexcept
		{
			assert( isReadSupported() );
			return m_pixelFormat.readFormat;
		}

		inline GlType getReadType()const noexcept
		{
			assert( isReadSupported() );
			return m_pixelFormat.readType;
		}

		inline GlComponentMapping getFormatSwizzle()const noexcept
		{
			return m_pixelFormat.swizzle;
		}

		inline VkComponentMapping const & getComponents()const noexcept
		{
			return m_createInfo.components;
		}

		inline VkImageSubresourceRange const & getSubresourceRange()const noexcept
		{
			return m_createInfo.subresourceRange;
		}

		inline VkImage getImage()const noexcept
		{
			return m_createInfo.image;
		}

	private:
		VkDevice m_device;
		VkImageViewCreateInfo m_createInfo;
		GlTextureType m_gltextureType;
		gl4::GlTextureViewType m_glviewType;
		PixelFormat m_pixelFormat;
	};
}

#endif
