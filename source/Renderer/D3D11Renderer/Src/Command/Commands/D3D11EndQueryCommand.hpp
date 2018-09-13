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
	*	Commande de fin d'une requ�te.
	*/
	class EndQueryCommand
		: public CommandBase
	{
	public:
		EndQueryCommand( Device const & device
			, renderer::QueryPool const & pool
			, uint32_t query );
		void apply( Context const & context )const override;
		CommandPtr clone()const override;

	private:
		ID3D11Query * m_query;
	};
}
