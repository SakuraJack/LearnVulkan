#pragma once

#include "Events/Event.h"
#include "Events/ApplicationEvent.h"
#include "Window.h"
#include "Renderer/Mesh.h"

class Application
{
public:
	Application();
	virtual ~Application();

	void Run();

	void OnUpdate() {}
	void OnEvent(Event& event);
private:
	bool OnWindowClose(WindowCloseEvent& event);
	bool OnWindowResize(WindowResizeEvent& event);
private:
	bool m_Running = true;
	std::shared_ptr<Window> m_Window;
	static Application* s_Instance;
	std::vector<std::shared_ptr<Mesh>> m_Meshes;
};