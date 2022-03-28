#include <Skye.hpp>
#include <Skye/Core/EntryPoint.hpp>

#include "Platform/OpenGL/OpenGLShader.hpp"
#include "Platform/OpenGL/OpenGLTexture.hpp"
#include "imgui/imgui.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Sandbox2D.hpp"

class ExampleLayer : public Skye::Layer
{
public:
	ExampleLayer()
		: Layer("Example"), m_CameraController(1280.0f / 720.0f, true)
	{
		// -- Create Triangle -- //
		m_TriangleVA = Skye::VertexArray::Create();
		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			0.5f, -0.5f, 0.0f, 0.2f, 0.2f, 0.8f, 1.0f,
			0.0f, 0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};
		Skye::Ref<Skye::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Skye::VertexBuffer::Create(vertices, sizeof(vertices)));
		// Describe layout of vertex buffer (no vertex arrays in D3D!)
		Skye::BufferLayout layout = {
			{ Skye::ShaderDataType::Float3, "a_Position" },
			{ Skye::ShaderDataType::Float4, "a_Color" }
		};
		vertexBuffer->SetLayout(layout);
		m_TriangleVA->AddVertexBuffer(vertexBuffer);
		uint32_t indices[3] = { 0, 1, 2 };
		Skye::Ref<Skye::IndexBuffer> indexBuffer;
		indexBuffer.reset(Skye::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_TriangleVA->SetIndexBuffer(indexBuffer);

		// -- Create Square -- //
		m_SquareVA = Skye::VertexArray::Create();
		float vertices2[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f, 0.5f, 0.0f, 0.0f, 1.0f
		};
		Skye::Ref<Skye::VertexBuffer> squareVB;
		squareVB.reset(Skye::VertexBuffer::Create(vertices2, sizeof(vertices2)));
		squareVB->SetLayout({
			{Skye::ShaderDataType::Float3, "a_Position"},
			{Skye::ShaderDataType::Float2, "a_TexCoord"}
		});
		m_SquareVA->AddVertexBuffer(squareVB);
		uint32_t indices2[6] = { 0, 1, 2, 2, 3, 0 };
		Skye::Ref<Skye::IndexBuffer> indexBuffer2;
		indexBuffer2.reset(Skye::IndexBuffer::Create(indices2, sizeof(indices2) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(indexBuffer2);

		// Create and bind shader
		std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec3 a_Color;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = vec4(a_Color, 1.0);
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}	
		)";

		std::string fragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;
			
			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}	
		)";

		auto triangleShader = m_ShaderLibrary.Load("TriangleShader", vertexSrc, fragmentSrc);

		// Create and bind shader
		std::string vertexSrc2 = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}	
		)";

		std::string fragmentSrc2 = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			uniform vec3 u_Color;

			in vec3 v_Position;
			
			void main()
			{
				color = vec4(0.1, 0.8, 0.4, 1.0);
				color = vec4(u_Color, 1.0);
			}	
		)";

		// Load shader from strings
		auto flatColorShader = m_ShaderLibrary.Load("FlatColorShader", vertexSrc2, fragmentSrc2);
		// Load shader from file
		auto textureShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");

		m_Texture = Skye::Texture2D::Create("assets/textures/Checkerboard.png");
		m_LogoTexture = Skye::Texture2D::Create("assets/textures/Logo.png");
		std::dynamic_pointer_cast<Skye::OpenGLShader>(textureShader)->Bind();
		std::dynamic_pointer_cast<Skye::OpenGLShader>(textureShader)->UploadUniformInt("u_Texture", 0); // Texture is bound to 0
	}

	//Input Polling
	void OnUpdate(Skye::Timestep ts) override
	{
		// Update
		m_CameraController.OnUpdate(ts);

		// Render
		Skye::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 0 });
		Skye::RenderCommand::Clear();

		Skye::Renderer::BeginScene(m_CameraController.GetCamera());
		{
			glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

			//Skye::Material* material = new Skye::Material(m_FlatColorShader);
			auto flatColorShader = m_ShaderLibrary.Get("FlatColorShader");
			std::dynamic_pointer_cast<Skye::OpenGLShader>(flatColorShader)->Bind();
			std::dynamic_pointer_cast<Skye::OpenGLShader>(flatColorShader)->UploadUniformFloat3("u_Color", m_SquareColor);

			for (int y = 0; y < 10; ++y)
			{
				for (int x = 0; x < 10; x++)
				{
					glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
					glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
					Skye::Renderer::Submit(flatColorShader, m_SquareVA, transform);
				}
			}

			auto textureShader = m_ShaderLibrary.Get("Texture");

			// Draw Square
			m_Texture->Bind();
			Skye::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

			m_LogoTexture->Bind();
			Skye::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

			// Draw Triangle
			//Skye::Renderer::Submit(m_Shader, m_TriangleVA);
		}
		Skye::Renderer::EndScene();
	}

	void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}

	// Input handling via Events
	void OnEvent(Skye::Event& event) override
	{
		// Camera
		m_CameraController.OnEvent(event);

		// Render
		Skye::EventDispatcher dispatcher(event);
		dispatcher.Dispatch<Skye::KeyPressedEvent>(SK_BIND_EVENT_FN(ExampleLayer::OnKeyPressedEvent));
	}

	bool OnKeyPressedEvent(Skye::KeyPressedEvent& event)
	{
		return false;
	}

private:
	// Store all shaders into library instead of refs to each one!
	Skye::ShaderLibrary m_ShaderLibrary;

	// Triangle
	Skye::Ref<Skye::VertexArray> m_TriangleVA;

	// Square
	Skye::Ref<Skye::VertexArray> m_SquareVA;

	Skye::Ref<Skye::Texture2D> m_Texture;
	Skye::Ref<Skye::Texture2D> m_LogoTexture;

	Skye::OrthographicCameraController m_CameraController;
	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.4f };
};

class Sandbox : public Skye::Application
{
public:
	Sandbox()
	{
		// Create layer structure for application.
		//PushLayer(new ExampleLayer());
		PushLayer(new Sandbox2D());
	}

	~Sandbox()
	{
	}
};

Skye::Application* Skye::CreateApplication()
{
	return new Sandbox();
}