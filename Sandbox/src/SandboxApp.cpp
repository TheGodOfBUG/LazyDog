
#include <iostream>
#include <LazyDog.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>

#include "Platform/OpenGL/OpenGLShader.h"


class ExampleLayer : public LazyDog::Layer
{
public:
	ExampleLayer()
		: Layer("Example") , m_CameraController(1280.f / 720.f, true)
	{
		m_VertexArray.reset(LazyDog::VertexArray::Create());

		float vertices[7 * 3] = {
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		};
		LazyDog::Ref<LazyDog::VertexBuffer> m_VertexBuffer;
		m_VertexBuffer.reset(LazyDog::VertexBuffer::Create(vertices, sizeof(vertices)));
		{
			LazyDog::BufferLayout layout = {
				{ LazyDog::ShaderDataType::Float3, "a_Position" },
				{ LazyDog::ShaderDataType::Float4, "a_Color"}
			};

			m_VertexBuffer->SetLayout(layout);

		}
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);

		unsigned int indices[3] = { 0, 1, 2 };
		LazyDog::Ref<LazyDog::IndexBuffer> m_IndexBuffer;
		m_IndexBuffer.reset(LazyDog::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

		m_VertexArray->SetIndexBuffer(m_IndexBuffer);


		m_SquareVA.reset(LazyDog::VertexArray::Create());

		float squareVertices[4 * 5] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		LazyDog::Ref<LazyDog::VertexBuffer> squareVB;
		squareVB.reset(LazyDog::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({
			{ LazyDog::ShaderDataType::Float3, "a_Position" },
			{ LazyDog::ShaderDataType::Float2, "a_TexCoord" }
			});
		m_SquareVA->AddVertexBuffer(squareVB);
		unsigned int squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		LazyDog::Ref<LazyDog::IndexBuffer> squareIB;
		squareIB.reset(LazyDog::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));

		m_SquareVA->SetIndexBuffer(squareIB);

		std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
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
		m_Shader = LazyDog::Shader::Create("VertexPosColor", vertexSrc, fragmentSrc);

		std::string flatColorShaderVertexSrc = R"(
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

		std::string flatColorShaderFragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec3 v_Position;

			uniform vec3 u_Color;

			void main()
			{
				color = vec4(u_Color, 1.0f);
			}
		)";

		m_FlatColorShader = LazyDog::Shader::Create("FlatShader", flatColorShaderVertexSrc, flatColorShaderFragmentSrc);

		auto texttureShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");

		m_Texture = LazyDog::Texture2D::create("assets/textures/awesomeface.png");
		m_BlenderTexture = LazyDog::Texture2D::create("assets/textures/grass.png");

		std::dynamic_pointer_cast<LazyDog::OpenGLShader>(texttureShader)->Bind();
		std::dynamic_pointer_cast<LazyDog::OpenGLShader>(texttureShader)->UploadUniformInt("u_Texture", 0);
	}

	void OnUpdate(LazyDog::Timestep ts) override
	{
		//LD_TRACE("Delta time : {0}s ({1}ms)", ts.GetSeconds(), ts.GetMilliseconds());

		// Update
		m_CameraController.OnUpdate(ts);

		LazyDog::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		LazyDog::RenderCommand::Clear();

		LazyDog::Renderer::BeginScene(m_CameraController.GetCamera());

		
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		glm::vec4 redColor(0.8f, 0.2f, 0.3f, 1.0f);
		glm::vec4 blueColor(0.2f, 0.3f, 0.8f, 1.0f);


		std::dynamic_pointer_cast<LazyDog::OpenGLShader>(m_FlatColorShader)->Bind();
		std::dynamic_pointer_cast<LazyDog::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat3("u_Color", m_SquareColor);
		for (int x = 0; x < 20; x++)
		{
			for (int y = 0; y < 20 ;y++)
			{
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				LazyDog::Renderer::Submit(m_FlatColorShader, m_SquareVA, transform);
			}
		}

		auto texttureShader = m_ShaderLibrary.Get("Texture");

		m_Texture->Bind();
		LazyDog::Renderer::Submit(texttureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
		m_BlenderTexture->Bind();
		LazyDog::Renderer::Submit(texttureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
		/*LazyDog::Renderer::Submit(m_Shader, m_VertexArray);*/

		LazyDog::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}

	void OnEvent(LazyDog::Event& event) override
	{
		m_CameraController.OnEvent(event);
	}

private:
	//TEMP
	LazyDog::ShaderLibrary m_ShaderLibrary;
	LazyDog::Ref<LazyDog::Shader> m_Shader;
	LazyDog::Ref<LazyDog::VertexArray> m_VertexArray;

	LazyDog::Ref<LazyDog::Shader> m_FlatColorShader;
	LazyDog::Ref<LazyDog::VertexArray> m_SquareVA;

	LazyDog::Ref<LazyDog::Texture2D> m_Texture, m_BlenderTexture;

	LazyDog::OrthographicCameraController m_CameraController;
	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f};
};

class Sandbox : public LazyDog::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}
	~Sandbox(){}
};

LazyDog::Application* LazyDog::CreateAppliction()
{
	return new Sandbox();
}