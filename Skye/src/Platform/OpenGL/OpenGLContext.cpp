#include "skpch.hpp"
#include "OpenGLContext.hpp"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Skye {

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_windowHandle(windowHandle)
	{
		SK_CORE_ASSERT(windowHandle, "Window Handle is null!");
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_windowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		SK_CORE_ASSERT(status, "Failed to initialize Glad!");

		SK_CORE_INFO("OpenGL Info:");
		SK_CORE_INFO("  Vendor: {0}", (char*)glGetString(GL_VENDOR));
		SK_CORE_INFO("  Renderer: {0}", (char*)glGetString(GL_RENDERER));
		SK_CORE_INFO("  Version: {0}", (char*)glGetString(GL_VERSION));
	}
	
	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_windowHandle);
	}
}