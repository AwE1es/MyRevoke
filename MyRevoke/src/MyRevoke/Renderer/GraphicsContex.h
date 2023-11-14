#pragma once


namespace Revoke
{
	class RenderContex
	{
	public:
		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;
	};

}