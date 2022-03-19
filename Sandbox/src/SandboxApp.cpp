#include <Skye.hpp>
#include "Platform/OpenGL/OpenGLShader.hpp"
#include "Platform/OpenGL/OpenGLTexture.hpp"

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class ExampleLayer : public Skye::Layer
{
public:
	ExampleLayer()
		: Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f)
	{
		// -- Create Triangle -- //
		m_TriangleVA.reset(Skye::VertexArray::Create());
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
		m_SquareVA.reset(Skye::VertexArray::Create());
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

		m_Shader.reset(Skye::Shader::Create(vertexSrc, fragmentSrc));

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

		m_FlatColorShader.reset(Skye::Shader::Create(vertexSrc2, fragmentSrc2));

		// Create and bind shader
		std::string textureVertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec2 a_TexCoord;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec2 v_TexCoord;

			void main()
			{
				v_TexCoord = a_TexCoord;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}	
		)";

		std::string textureFragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec2 v_TexCoord;

			uniform sampler2D u_Texture;
			
			void main()
			{
				color = texture(u_Texture, v_TexCoord);
			}	
		)";

		m_TextureShader.reset(Skye::Shader::Create(textureVertexSrc, textureFragmentSrc));

		m_Texture = Skye::Texture2D::Create("assets/textures/Checkerboard.png");
		m_LogoTexture = Skye::Texture2D::Create("assets/textures/Logo.png");
		std::dynamic_pointer_cast<Skye::OpenGLShader>(m_TextureShader)->Bind();
		std::dynamic_pointer_cast<Skye::OpenGLShader>(m_TextureShader)->UploadUniformInt("u_Texture", 0); // Texture is bound to 0
	}

	//Input Polling
	void OnUpdate(Skye::Timestep ts) override
	{
		if (Skye::Input::IsKeyPressed(SK_KEY_LEFT))
		{
			m_CameraPosition.x += m_CameraMoveSpeed * ts;
		}
		else if (Skye::Input::IsKeyPressed(SK_KEY_RIGHT))
		{
			m_CameraPosition.x -= m_CameraMoveSpeed * ts;
		}

		if (Skye::Input::IsKeyPressed(SK_KEY_UP))
		{
			m_CameraPosition.y -= m_CameraMoveSpeed * ts;
		}
		else if (Skye::Input::IsKeyPressed(SK_KEY_DOWN))
		{
			m_CameraPosition.y += m_CameraMoveSpeed * ts;
		}

		if (Skye::Input::IsKeyPressed(SK_KEY_A))
		{
			m_CameraRotation += m_CameraRotationSpeed * ts;
		}
		if (Skye::Input::IsKeyPressed(SK_KEY_D))
		{
			m_CameraRotation -= m_CameraRotationSpeed * ts;
		}

		Skye::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 0 });
		Skye::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		Skye::Renderer::BeginScene(m_Camera);
		{
			glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

			//Skye::Material* material = new Skye::Material(m_FlatColorShader);
			std::dynamic_pointer_cast<Skye::OpenGLShader>(m_FlatColorShader)->Bind();
			std::dynamic_pointer_cast<Skye::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat3("u_Color", m_SquareColor);

			for (int y = 0; y < 10; ++y)
			{
				for (int x = 0; x < 10; x++)
				{
					glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
					glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
					Skye::Renderer::Submit(m_FlatColorShader, m_SquareVA, transform);
				}
			}
			// Draw Square
			m_Texture->Bind();
			Skye::Renderer::Submit(m_TextureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

			m_LogoTexture->Bind();
			Skye::Renderer::Submit(m_TextureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

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
		Skye::EventDispatcher dispatcher(event);
		dispatcher.Dispatch<Skye::KeyPressedEvent>(SK_BIND_EVENT_FN(ExampleLayer::OnKeyPressedEvent));
	}

	bool OnKeyPressedEvent(Skye::KeyPressedEvent& event)
	{
		return false;
	}

private:
	// Triangle
	Skye::Ref<Skye::Shader> m_Shader;
	Skye::Ref<Skye::VertexArray> m_TriangleVA;

	// Square
	Skye::Ref<Skye::Shader> m_FlatColorShader;
	Skye::Ref<Skye::Shader> m_TextureShader;
	Skye::Ref<Skye::VertexArray> m_SquareVA;

	Skye::Ref<Skye::Texture2D> m_Texture;
	Skye::Ref<Skye::Texture2D> m_LogoTexture;

	Skye::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraMoveSpeed{ 2.0f };
	float m_CameraRotation{ 0.0f };
	float m_CameraRotationSpeed{ 90.0f };

	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.4f };
};

class Sandbox : public Skye::Application
{
public:
	Sandbox()
	{
		// Create layer structure for application.
		PushLayer(new ExampleLayer());
	}

	~Sandbox()
	{
	}
};

Skye::Application* Skye::CreateApplication()
{
	return new Sandbox();
}