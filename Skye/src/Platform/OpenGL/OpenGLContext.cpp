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
		SK_PROFILE_FUNCTION();
		glfwMakeContextCurrent(m_windowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		SK_CORE_ASSERT(status, "Failed to initialize Glad!");

		SK_CORE_INFO("OpenGL Info:");
		SK_CORE_INFO("  Vendor: {0}", (char*)glGetString(GL_VENDOR));
		SK_CORE_INFO("  Renderer: {0}", (char*)glGetString(GL_RENDERER));
		SK_CORE_INFO("  Version: {0}", (char*)glGetString(GL_VERSION));

	#ifdef SK_ENABLE_ASSERTS
		int versionMajor;
		int versionMinor;
		glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
		glGetIntegetv(GL_MINOR_VERSION, &minorVersion);

		SK_CORE_ASSERT(verionMajor > 4 || (versionMajor == 4 && versionMinor >= 5), "Skye engine requires at least OpenGL version 4.5!");
	#endif
	}
	
	void OpenGLContext::SwapBuffers()
	{
		SK_PROFILE_FUNCTION();

		glfwSwapBuffers(m_windowHandle);
	}
}