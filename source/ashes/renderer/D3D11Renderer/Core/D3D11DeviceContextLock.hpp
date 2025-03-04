/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/D3D11Renderer/D3D11RendererPrerequisites.hpp"

namespace ashes::d3d11
{
	class DeviceContextLock
	{
	public:
		DeviceContextLock( DeviceContextLock const & ) = delete;
		DeviceContextLock & operator=( DeviceContextLock const & ) = delete;
		DeviceContextLock( DeviceContextLock && rhs );
		DeviceContextLock & operator=( DeviceContextLock && rhs );
		DeviceContextLock( Device const * device );
		~DeviceContextLock();

		ID3D11DeviceContext * operator->()const
		{
			return m_context;
		}

		ID3D11DeviceContext * operator*()const
		{
			return m_context;
		}

	private:
		Device const * m_device;
		ID3D11DeviceContext * m_context;
	};
}
