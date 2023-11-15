#include "rvpch.h"
#include "RenderCommand.h"

#include "MyRevoke/Platform/OpenGL/GLRendererAPI.h"

namespace Revoke
{
	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;
}
 