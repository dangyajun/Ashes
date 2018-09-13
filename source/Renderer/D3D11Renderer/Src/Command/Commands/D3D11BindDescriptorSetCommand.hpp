/*
This file belongs to RendererLib.
See LICENSE file in root folder
*/
#pragma once

#include "D3D11CommandBase.hpp"

namespace d3d11_renderer
{
	/**
	*\brief
	*	Commande d'activation d'un set de descripteurs.
	*/
	class BindDescriptorSetCommand
		: public CommandBase
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] descriptorSet
		*	Le descriptor set.
		*\param[in] layout
		*	Le layout de pipeline.
		*\param[in] bindingPoint
		*	Le point d'attache du set.
		*/
		BindDescriptorSetCommand( Device const & device
			, renderer::DescriptorSet const & descriptorSet
			, renderer::PipelineLayout const & layout
			, renderer::UInt32Array const & dynamicOffsets
			, renderer::PipelineBindPoint bindingPoint );

		void apply( Context const & context )const override;
		void remove( Context const & context )const override;
		void fillContext( Context & context )const override;
		CommandPtr clone()const override;

	private:
		DescriptorSet const & m_descriptorSet;
		PipelineLayout const & m_layout;
		renderer::PipelineBindPoint m_bindingPoint;
		renderer::UInt32Array m_dynamicOffsets;
	};
}
