/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_Texture_HPP___
#define ___AshesPP_Texture_HPP___
#pragma once

#include "ashespp/Image/ImageCreateInfo.hpp"
#include "ashespp/Miscellaneous/DeviceMemory.hpp"

#include <ashes/common/VkTypeTraits.hpp>

namespace ashes
{
	VkAccessFlags getAccessMask( VkImageLayout layout );
	VkPipelineStageFlags getStageMask( VkImageLayout layout );
	/**
	*\brief
	*	A texture image.
	*/
	class Image
		: public VkObject
	{
	public:
		/**
		*\brief
		*	Contains an image mapped in RAM informations.
		*/
		struct Mapped
		{
			uint8_t * data;
			uint64_t size;
			uint64_t rowPitch;
			uint64_t arrayPitch;
			uint64_t depthPitch;
		};

	private:
		Image( Image const & ) = delete;
		Image & operator=( Image const & ) = delete;

	public:
		Image( Image && rhs )noexcept;
		Image & operator=( Image && rhs )noexcept;
		Image();
		/**
		*\brief
		*	Constructor.
		*\param[in] device
		*	The logical device.
		*\param[in] createInfo
		*	The image creation informations.
		*/
		Image( Device const & device
			, ImageCreateInfo createInfo );
		/**
		*\brief
		*	Constructor.
		*\param[in] device
		*	The logical device.
		*\param[in] createInfo
		*	The image creation informations.
		*/
		Image( Device const & device
			, std::string const & debugName
			, ImageCreateInfo createInfo );
		/**
		*\brief
		*	Constructor.
		*\param[in] device
		*	The logical device.
		*\param[in] image
		*	The image.
		*/
		Image( Device const & device
			, VkImage image );
		/**
		*\brief
		*	Constructor.
		*\param[in] device
		*	The logical device.
		*\param[in] image
		*	The image.
		*/
		Image( Device const & device
			, VkImage image
			, ImageCreateInfo createInfo );
		/**
		*\brief
		*	Destructor.
		*/
		~Image();
		/**
		*\brief
		*	Binds this buffer to given device memory object.
		*\param[in] memory
		*	The memory object.
		*/
		void bindMemory( DeviceMemoryPtr memory );
		/**
		*\brief
		*	Maps the buffer's memory in RAM.
		*\param[in] offset
		*	The memory mapping starting offset.
		*\param[in] size
		*	The memory mappping size.
		*\param[in] flags
		*	The memory mapping flags.
		*\return
		*	\p nullptr if the mapping failed.
		*/
		Mapped lock( uint32_t offset
			, uint32_t size
			, VkMemoryMapFlags flags )const;
		/**
		*\brief
		*	Invalidates the buffer content.
		*\param[in] offset
		*	The mapped memory starting offset.
		*\param[in] size
		*	The range size.
		*/
		void invalidate( uint32_t offset
			, uint32_t size )const;
		/**
		*\brief
		*	Updates the VRAM.
		*\param[in] offset
		*	The mapped memory starting offset.
		*\param[in] size
		*	The range size.
		*/
		void flush( uint32_t offset
			, uint32_t size )const;
		/**
		*\brief
		*	Unmaps the buffer's memory from RAM.
		*/
		void unlock()const;
		/**
		*\brief
		*	Generates the image mipmaps.
		*\param[in] commandBuffer
		*	A command buffer, in record state.
		*/
		void generateMipmaps( CommandBuffer & commandBuffer
			, VkImageLayout srcImageLayout
			, VkImageLayout srcMipsImageLayout
			, VkImageLayout dstImageLayout )const;
		/**
		*\brief
		*	Generates the image mipmaps.
		*/
		void generateMipmaps( CommandPool const & commandPool
			, Queue const & queue
			, VkImageLayout srcImageLayout
			, VkImageLayout srcMipsImageLayout
			, VkImageLayout dstImageLayout )const;
		/**
		*\brief
		*	Generates the image mipmaps.
		*\param[in] commandBuffer
		*	A command buffer, in record state.
		*/
		void generateMipmaps( CommandBuffer & commandBuffer
			, uint32_t baseArrayLayer
			, uint32_t layerCount
			, VkImageLayout srcImageLayout
			, VkImageLayout srcMipsImageLayout
			, VkImageLayout dstImageLayout )const;
		/**
		*\brief
		*	Generates the image mipmaps.
		*/
		void generateMipmaps( CommandPool const & commandPool
			, Queue const & queue
			, uint32_t baseArrayLayer
			, uint32_t layerCount
			, VkImageLayout srcImageLayout
			, VkImageLayout srcMipsImageLayout
			, VkImageLayout dstImageLayout )const;
		/**
		*\brief
		*	Generates the image mipmaps.
		*\param[in] commandBuffer
		*	A command buffer, in record state.
		*/
		void generateMipmaps( CommandBuffer & commandBuffer
			, VkImageLayout dstImageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL )const
		{
			generateMipmaps( commandBuffer
				, VK_IMAGE_LAYOUT_UNDEFINED
				, VK_IMAGE_LAYOUT_UNDEFINED
				, dstImageLayout );
		}
		/**
		*\brief
		*	Generates the image mipmaps.
		*/
		void generateMipmaps( CommandPool const & commandPool
			, Queue const & queue
			, VkImageLayout dstImageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL )const
		{
			generateMipmaps( commandPool
				, queue
				, VK_IMAGE_LAYOUT_UNDEFINED
				, VK_IMAGE_LAYOUT_UNDEFINED
				, dstImageLayout );
		}
		/**
		*\brief
		*	Generates the image mipmaps.
		*\param[in] commandBuffer
		*	A command buffer, in record state.
		*/
		void generateMipmaps( CommandBuffer & commandBuffer
			, uint32_t baseArrayLayer
			, uint32_t layerCount
			, VkImageLayout dstImageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL )const
		{
			generateMipmaps( commandBuffer
				, baseArrayLayer
				, layerCount
				, VK_IMAGE_LAYOUT_UNDEFINED
				, VK_IMAGE_LAYOUT_UNDEFINED
				, dstImageLayout );
		}
		/**
		*\brief
		*	Generates the image mipmaps.
		*/
		void generateMipmaps( CommandPool const & commandPool
			, Queue const & queue
			, uint32_t baseArrayLayer
			, uint32_t layerCount
			, VkImageLayout dstImageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL )const
		{
			generateMipmaps( commandPool
				, queue
				, baseArrayLayer
				, layerCount
				, VK_IMAGE_LAYOUT_UNDEFINED
				, VK_IMAGE_LAYOUT_UNDEFINED
				, dstImageLayout );
		}
		/**
		*\return
		*	The memory requirements for this buffer.
		*/
		VkMemoryRequirements getMemoryRequirements()const;
		/**
		*\brief
		*	Creates a view to the image.
		*\param[in] createInfo
		*	The view creation informations.
		*/
		ImageView createView( VkImageViewCreateInfo createInfo )const;
		/**
		*\brief
		*	Creates a view to the image.
		*\param[in] createInfo
		*	The view creation informations.
		*/
		ImageView createView( std::string const & debugName
			, VkImageViewCreateInfo createInfo )const;
		/**
		*\brief
		*	Creates a view to the image.
		*\param[in] type
		*	The view type.
		*\param[in] format
		*	The view's pixels format.
		*\param[in] baseMipLevel
		*	The first mipmap level accessible to the view.
		*\param[in] levelCount
		*	The number of mipmap levels (starting from \p baseMipLevel) accessible to the view.
		*\param[in] baseArrayLayer
		*	The first array layer accessible to the view.
		*\param[in] layerCount
		*	The number of array layers (starting from \p baseArrayLayer) accessible to the view.
		*\param[in] mapping
		*	The colours component mapping.
		*/
		ImageView createView( VkImageViewType type
			, VkFormat format
			, uint32_t baseMipLevel = 0u
			, uint32_t levelCount = 1u
			, uint32_t baseArrayLayer = 0u
			, uint32_t layerCount = 1u
			, VkComponentMapping const & mapping = VkComponentMapping{} )const;
		/**
		*\brief
		*	Creates a view to the image.
		*\param[in] type
		*	The view type.
		*\param[in] format
		*	The view's pixels format.
		*\param[in] baseMipLevel
		*	The first mipmap level accessible to the view.
		*\param[in] levelCount
		*	The number of mipmap levels (starting from \p baseMipLevel) accessible to the view.
		*\param[in] baseArrayLayer
		*	The first array layer accessible to the view.
		*\param[in] layerCount
		*	The number of array layers (starting from \p baseArrayLayer) accessible to the view.
		*\param[in] mapping
		*	The colours component mapping.
		*/
		ImageView createView( std::string const & debugName
			, VkImageViewType type
			, VkFormat format
			, uint32_t baseMipLevel = 0u
			, uint32_t levelCount = 1u
			, uint32_t baseArrayLayer = 0u
			, uint32_t layerCount = 1u
			, VkComponentMapping const & mapping = VkComponentMapping{} )const;
		VkImageMemoryBarrier makeTransition( VkImageLayout srcLayout
			, VkImageLayout dstLayout
			, VkImageSubresourceRange mipSubRange
			, uint32_t mipLevel
			, uint32_t srcQueueFamily
			, uint32_t dstQueueFamily )const;
		VkImageMemoryBarrier makeTransition( VkImageLayout srcLayout
			, VkImageLayout dstLayout
			, VkImageSubresourceRange mipSubRange
			, uint32_t srcQueueFamily
			, uint32_t dstQueueFamily )const;

		VkImageMemoryBarrier makeTransition( VkImageLayout srcLayout
			, VkImageLayout dstLayout
			, VkImageSubresourceRange mipSubRange
			, uint32_t mipLevel )const
		{
			return makeTransition( srcLayout
				, dstLayout
				, mipSubRange
				, mipLevel
				, VK_QUEUE_FAMILY_IGNORED
				, VK_QUEUE_FAMILY_IGNORED );
		}

		VkImageMemoryBarrier makeTransition( VkImageLayout srcLayout
			, VkImageLayout dstLayout
			, VkImageSubresourceRange mipSubRange )const
		{
			return makeTransition( srcLayout
				, dstLayout
				, mipSubRange
				, VK_QUEUE_FAMILY_IGNORED
				, VK_QUEUE_FAMILY_IGNORED );
		}
		/**
		*\name
		*	Getters.
		**/
		/**@{*/
		inline VkFormat getFormat()const noexcept
		{
			return m_createInfo->format;
		}

		inline uint32_t getLayerCount()const noexcept
		{
			return m_createInfo->arrayLayers;
		}

		inline VkImageCreateFlags getFlags()const noexcept
		{
			return m_createInfo->flags;
		}

		inline uint32_t getMipmapLevels()const noexcept
		{
			return m_createInfo->mipLevels;
		}

		inline VkExtent3D const & getDimensions()const noexcept
		{
			return m_createInfo->extent;
		}

		inline VkImageType getType()const
		{
			return m_createInfo->imageType;
		}

		inline VkImageTiling getTiling()const
		{
			return m_createInfo->tiling;
		}

		inline VkSampleCountFlagBits getSampleCount()const
		{
			return m_createInfo->samples;
		}

		inline VkImageUsageFlags getUsage()const
		{
			return m_createInfo->usage;
		}
		/**@}*/
		/**
		*\brief
		*	VkImage implicit cast operator.
		*/
		inline operator VkImage const & ()const
		{
			return m_internal;
		}

	private:
		ImageViewCache::iterator doDestroyView( VkImageViewCreateInfo const & view )const;
		ImageViewCache::iterator doDestroyView( ImageView view )const;

	private:
		Device const * m_device{ nullptr };
		ImageCreateInfo m_createInfo{ 0u, VK_IMAGE_TYPE_2D, VK_FORMAT_UNDEFINED, { 1u, 1u, 1u }, 1u, 1u, VK_SAMPLE_COUNT_1_BIT, VK_IMAGE_TILING_OPTIMAL, 0u };
		VkImage m_internal{};
		DeviceMemoryPtr m_storage;
		bool m_ownInternal{ true };
		mutable ImageViewCache m_views;
	};
}

#endif
