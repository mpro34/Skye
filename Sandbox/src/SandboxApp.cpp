#include <Skye.hpp>
#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>

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
		std::shared_ptr<Skye::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Skye::VertexBuffer::Create(vertices, sizeof(vertices)));
		// Describe layout of vertex buffer (no vertex arrays in D3D!)
		Skye::BufferLayout layout = {
			{ Skye::ShaderDataType::Float3, "a_Position" },
			{ Skye::ShaderDataType::Float4, "a_Color" }
		};
		vertexBuffer->SetLayout(layout);
		m_TriangleVA->AddVertexBuffer(vertexBuffer);
		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<Skye::IndexBuffer> indexBuffer;
		indexBuffer.reset(Skye::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_TriangleVA->SetIndexBuffer(indexBuffer);

		// -- Create Square -- //
		m_SquareVA.reset(Skye::VertexArray::Create());
		float vertices2[4 * 3] = {
			-0.5f, 0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.5f, 0.5f, 0.0f,
			-0.5f, -0.5f, 0.0f
		};
		std::shared_ptr<Skye::VertexBuffer> squareVB;
		squareVB.reset(Skye::VertexBuffer::Create(vertices2, sizeof(vertices2)));
		squareVB->SetLayout({
			{Skye::ShaderDataType::Float3, "a_Position"}
			});
		m_SquareVA->AddVertexBuffer(squareVB);
		uint32_t indices2[6] = { 0, 1, 2, 1, 3, 0 };
		std::shared_ptr<Skye::IndexBuffer> indexBuffer2;
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

		m_Shader = std::make_unique<Skye::Shader>(vertexSrc, fragmentSrc);

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

			in vec3 v_Position;
			
			void main()
			{
				color = vec4(0.1, 0.8, 0.4, 1.0);
			}	
		)";

		m_BlueShader = std::make_unique<Skye::Shader>(vertexSrc2, fragmentSrc2);
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
			static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
			// Draw 5 squares with the same scale and various translations
			for (int y = 0; y < 10; ++y)
			{
				for (int x = 0; x < 10; x++)
				{
					glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
					glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
					Skye::Renderer::Submit(m_BlueShader, m_SquareVA, transform);
				}
			}
			// Draw Triangle
			Skye::Renderer::Submit(m_Shader, m_TriangleVA);
		}
		Skye::Renderer::EndScene();
	}

	void OnImGuiRender() override
	{

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
	std::shared_ptr<Skye::Shader> m_Shader;
	std::shared_ptr<Skye::VertexArray> m_TriangleVA;

	// Square
	std::shared_ptr<Skye::Shader> m_BlueShader;
	std::shared_ptr<Skye::VertexArray> m_SquareVA;

	Skye::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraMoveSpeed{ 2.0f };
	float m_CameraRotation{ 0.0f };
	float m_CameraRotationSpeed{ 90.0f };
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