#pragma once

#include "Skye/Renderer/GraphicsContext.hpp"

struct GLFWwindow;

namespace Skye {

	class OpenGLContext : GraphicsContext
	{
	public:
		OpenGLContext(GLFWwindow* windowHandle);

		virtual void Init() override;
		virtual void SwapBuffers() override;
	private:
		GLFWwindow* m_windowHandle;
	};

}