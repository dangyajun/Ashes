#include "Buffer/D3D11UniformBuffer.hpp"

#include "Core/D3D11Device.hpp"
#include "Core/D3D11PhysicalDevice.hpp"

namespace d3d11_renderer
{
	namespace
	{
		uint32_t doGetAlignedSize( uint32_t size, uint32_t align )
		{
			uint32_t result = 0u;
			auto align32 = uint32_t( align );

			while ( size > align )
			{
				size -= align32;
				result += align32;
			}

			return result + align32;
		}
	}

	UniformBuffer::UniformBuffer( renderer::Device const & device
		, uint32_t count
		, uint32_t size
		, renderer::BufferTargets target
		, renderer::MemoryPropertyFlags flags )
		: renderer::UniformBufferBase{ device
			, count
			, size
			, target
			, flags }
	{
		m_buffer = m_device.createBuffer( count * getAlignedSize( getElementSize() )
			, target | renderer::BufferTarget::eUniformBuffer
			, flags );
	}

	uint32_t UniformBuffer::getAlignedSize( uint32_t size )const
	{
		return doGetAlignedSize( size
			, uint32_t( static_cast< Device const & >( m_device ).getPhysicalDevice().getProperties().limits.minUniformBufferOffsetAlignment ) );
	}
}
