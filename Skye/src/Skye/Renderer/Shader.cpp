#include "skpch.hpp"
#include "Shader.hpp"

#include "Renderer.hpp"
#include "Platform/OpenGL/OpenGLShader.hpp"

namespace Skye {

	Ref<Shader> Shader::Create(const std::string& filepath)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:		SK_CORE_ASSERT(false, "RendererAPI::NONE is not currently supported!");  return nullptr;
		case RendererAPI::API::OpenGL:		return std::make_shared<OpenGLShader>(filepath);
		}

		SK_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<Shader> Shader::Create(const std::string name, const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:		SK_CORE_ASSERT(false, "RendererAPI::NONE is not currently supported!");  return nullptr;
		case RendererAPI::API::OpenGL:		return std::make_shared<OpenGLShader>(name, vertexSrc, fragmentSrc);
		}

		SK_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	void ShaderLibrary::Add(const std::string name, const Ref<Shader>& shader)
	{
		SK_CORE_ASSERT(Exists(name), "Shader already exists!");
		m_Shaders[name] = shader;
	}

	void ShaderLibrary::Add(const Ref<Shader>& shader)
	{
		auto& name = shader->GetName();
		Add(name, shader);
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& filepath)
	{
		auto shader = Shader::Create(filepath);
		Add(shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filepath)
	{
		auto shader = Shader::Create(filepath);
		Add(name, shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		auto shader = Shader::Create(name, vertexSrc, fragmentSrc);
		Add(name, shader);
		return shader;
	}


	Ref<Shader> ShaderLibrary::Get(const std::string& name)
	{
		SK_CORE_ASSERT(!Exists(name), "Shader doesn't exist!");
		return m_Shaders[name];
	}

	bool ShaderLibrary::Exists(const std::string& name) const
	{
		return m_Shaders.find(name) != m_Shaders.end();
	}
}