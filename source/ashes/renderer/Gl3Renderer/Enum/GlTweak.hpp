/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

namespace ashes::gl3
{
	enum GlTweak
		: uint32_t
	{
		GL_CULL_FACE = 0x0B44,
		GL_DEPTH_RANGE = 0x0B70,
		GL_DEPTH_TEST = 0x0B71,
		GL_DEPTH_WRITEMASK = 0x0B72,
		GL_DEPTH_FUNC = 0x0B74,
		GL_STENCIL_TEST = 0x0B90,
		GL_STENCIL_FRONT_FUNC = 0x0B92,
		GL_STENCIL_FRONT_VALUE_MASK = 0x0B93,
		GL_STENCIL_FRONT_FAIL = 0x0B94,
		GL_STENCIL_FRONT_PASS_DEPTH_FAIL = 0x0B95,
		GL_STENCIL_FRONT_PASS_DEPTH_PASS = 0x0B96,
		GL_STENCIL_FRONT_REF = 0x0B97,
		GL_STENCIL_FRONT_WRITEMASK = 0x0B98,
		GL_BLEND = 0x0BE2,
		GL_SCISSOR_TEST = 0x0C11,
		GL_POLYGON_OFFSET_POINT = 0x2A01,
		GL_POLYGON_OFFSET_LINE = 0x2A02,
		GL_MULTISAMPLE = 0x809D,
		GL_SAMPLE_ALPHA_TO_COVERAGE = 0x809E,
		GL_SAMPLE_ALPHA_TO_ONE = 0x809F,
		GL_POLYGON_OFFSET_FILL = 0x8037,
		GL_PROGRAM_POINT_SIZE = 0x8642,
		GL_DEPTH_CLAMP = 0x864F,
		GL_STENCIL_BACK_FUNC = 0x8800,
		GL_STENCIL_BACK_FAIL = 0x8801,
		GL_STENCIL_BACK_PASS_DEPTH_FAIL = 0x8802,
		GL_STENCIL_BACK_PASS_DEPTH_PASS = 0x8803,
		GL_TEXTURE_CUBE_MAP_SEAMLESS = 0x884F,
		GL_RASTERIZER_DISCARD = 0x8C89,
		GL_STENCIL_BACK_REF = 0x8CA3,
		GL_STENCIL_BACK_VALUE_MASK = 0x8CA4,
		GL_STENCIL_BACK_WRITEMASK = 0x8CA5,
		GL_PRIMITIVE_RESTART = 0x8F9D,
		GL_SAMPLE_SHADING = 0x8C36,
		GL_FRAMEBUFFER_SRGB = 0x8DB9,
		GL_DEBUG_OUTPUT_SYNC = 0x8242,
	};
	std::string getName( GlTweak value );
}
