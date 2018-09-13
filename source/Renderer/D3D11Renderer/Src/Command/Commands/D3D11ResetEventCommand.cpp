/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#include "D3D11ResetEventCommand.hpp"

#include "Sync/D3D11Event.hpp"

namespace d3d11_renderer
{
	ResetEventCommand::ResetEventCommand( Device const & device
		, renderer::Event const & event
		, renderer::PipelineStageFlags stageFlags )
		: CommandBase{ device }
		, m_event{ event }
	{
	}

	void ResetEventCommand::apply( Context const & context )const
	{
	}

	CommandPtr ResetEventCommand::clone()const
	{
		return std::make_unique< ResetEventCommand >( *this );
	}
}
