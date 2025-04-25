#include "pch.h"
#include "Application.h"

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

Application* Application::s_Instance = nullptr;
Application::Application()
{
	s_Instance = this;

	m_Window = std::shared_ptr<Window>(Window::Create());
	m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
	m_Meshes.push_back(Mesh::Create({
		{{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
		{{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
		{{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
	}));
}

Application::~Application()
{
}

void Application::Run()
{
	while (m_Running)
	{
		m_Window->OnUpdate();
	}
}

void Application::OnEvent(Event& event)
{
	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
	dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResize));
}

bool Application::OnWindowClose(WindowCloseEvent& event)
{
	m_Running = false;
	return false;
}

bool Application::OnWindowResize(WindowResizeEvent& event)
{
	return false;
}
