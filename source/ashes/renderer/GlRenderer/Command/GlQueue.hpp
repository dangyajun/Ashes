/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

#include "renderer/GlRenderer/GlRendererPrerequisites.hpp"
#include "renderer/GlRenderer/Command/Commands/GlCommandBase.hpp"

namespace ashes::gl
{
	void applyBuffer( ContextLock const & lock
		, CmdBuffer const & cmds );
	void applyList( ContextLock const & lock
		, CmdList const & cmds );

	class Queue
		: public ashes::IcdObject
	{
	public:
		Queue( VkAllocationCallbacks const * allocInfo
			, VkDevice device
			, VkDeviceQueueCreateInfo createInfo );
		~Queue();

		VkResult submit( VkSubmitInfoArray const & values
			, VkFence fence )const;
		VkResult bindSparse( ArrayView< VkBindSparseInfo const > values
			, VkFence fence )const;
		VkResult present( VkPresentInfoKHR const & presentInfo )const;
		VkResult waitIdle()const;
#if VK_EXT_debug_utils
		void beginDebugUtilsLabel( VkDebugUtilsLabelEXT const & labelInfo )const;
		void endDebugUtilsLabel()const;
		void insertDebugUtilsLabel( VkDebugUtilsLabelEXT const & labelInfo )const;
#endif

		inline VkDevice getDevice()const
		{
			return m_device;
		}

	private:
		VkDevice m_device;
		VkDeviceQueueCreateInfo m_createInfo;
		mutable Optional< DebugLabel > m_label;
	};
}
