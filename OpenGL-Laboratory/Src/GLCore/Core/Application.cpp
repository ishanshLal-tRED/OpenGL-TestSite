#include "pch.h"
#include "Application.h"
#include "Log.h"
#include "Input.h"

#include <glfw/glfw3.h>

namespace GLCore {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application(const std::string& name, uint32_t width, uint32_t height)
	{
		if (!s_Instance)
		{
			// Initialize core
			Log::Init();
		}

		LOG_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create({ name, width, height }));
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		m_ImGuiLayer.OnAttach ();
	}

	void Application::OnEvent(Event &e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

		if (!e.Handled)
			m_ImGuiLayer.OnEvent (e);
		m_TestsManager.ProcessEvent (e);
	}

	void Application::Run ()
	{
		while (m_Running)
		{
			float time = (float)glfwGetTime();
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			m_ImGuiLayer.OnUpdate (timestep);
			m_TestsManager.UpdateActiveLayers (timestep);

			m_ImGuiLayer.Begin();
			m_TestsManager.ImGuiRender ();
			m_ImGuiLayer.End();

			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose (WindowCloseEvent &e)
	{
		return ApplicationClose ();
	}

	bool Application::ApplicationClose ()
	{
		m_Running = false;
		return true;
	}

}