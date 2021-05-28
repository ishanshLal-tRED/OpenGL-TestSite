#include "pch.h"
#include "GLCore/Core/TestBase.h"
#include "TestsLayerManager.h"

#include "GLCore/Core/Application.h"
#include "imgui/imgui.h"

namespace GLCore
{
	TestsLayerManager::~TestsLayerManager ()
	{
		constexpr uint16_t Active_Tests_stack_size = sizeof (m_ActiveTests)/sizeof (m_ActiveTests[0]);

		for (uint16_t i = 0; i < Active_Tests_stack_size; i++) {
			if (m_ActiveTests[i]) {
				m_ActiveTests[i]->OnDetach ();
				m_ActiveTests[i] = nullptr;
			}
		}
		{
			for (uint16_t i = 0; i < m_AllTests.size (); i++)
				delete m_AllTests[i];
			m_AllTests.clear ();
		}
	}
	void TestsLayerManager::UpdateActiveLayers (Timestep deltatime)
	{
		for (Layer *test : m_ActiveTests) 	{
			if (test)
			{
				////
				// Here Will be code for frame buffer
				////
	
				test->OnUpdate (deltatime);
			} else break;
		}
	}
	void TestsLayerManager::ImGuiRender ()
	{
		{// DockSpace

			static bool dockspaceOpen = true;
			static constexpr bool optFullscreenPersistant = true;
			bool optFullscreen = optFullscreenPersistant;

			static ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None;

			// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
			// because it would be confusing to have two docking targets within each others.
			ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
			if (optFullscreen) {
				ImGuiViewport *viewPort = ImGui::GetMainViewport ();
				ImGui::SetNextWindowPos (viewPort->Pos);
				ImGui::SetNextWindowSize (viewPort->Size);
				ImGui::SetNextWindowViewport (viewPort->ID);
				ImGui::PushStyleVar (ImGuiStyleVar_WindowRounding, 0.0f);
				ImGui::PushStyleVar (ImGuiStyleVar_WindowBorderSize, 0.0f);

				windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
			}

			// when using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render background.
			if (dockspaceFlags & ImGuiDockNodeFlags_PassthruCentralNode)
				windowFlags |= ImGuiWindowFlags_NoBackground;

			// Important: note that we proceed even if Begin() return false (i.e window is collapsed).
			// This is because we want to keep our DockSpace() active. If a DockSpace is inactive,
			// all active windows docked into it will lose their parent and become undocked.
			// any change of dockspce/settings would lead towindows being stuck in limbo and never being visible.
			ImGui::PushStyleVar (ImGuiStyleVar_WindowPadding, ImVec2 (0.0f, 0.0f));
			ImGui::Begin ("Main DockSpace", &dockspaceOpen, windowFlags);
			ImGui::PopStyleVar ();

			if (optFullscreen)
				ImGui::PopStyleVar (2);

			// DockSpace
			ImGuiIO &io = ImGui::GetIO ();
			ImGuiStyle &style = ImGui::GetStyle ();
			float defaultMinWinSize = style.WindowMinSize.x;
			style.WindowMinSize.x = 280;

			if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
				m_DockspaceID = ImGui::GetID ("MyDockSpace");
				ImGui::DockSpace (m_DockspaceID, ImVec2 (0.0f, 0.0f), dockspaceFlags);
			}

			style.WindowMinSize.x = defaultMinWinSize;

			// DockSpace's MenuBar
			if (ImGui::BeginMenuBar ()) {
				if (ImGui::BeginMenu ("Main")) {

					if (ImGui::MenuItem ("Exit")) Application::Get ().ApplicationClose ();

					ImGui::EndMenu ();
				}
				{
					int i = 0;
					for (TestBase *test : m_ActiveTests) {
						if (test) {
							ImGui::PushID (i);
							ImGui::Bullet ();
							test->OnImGuiMenuOptions ();
							ImGui::PopID ();
						} else break;
					}
				}
				ImGui::EndMenuBar ();
			}

			// Here goes Stuff that will be put inside DockSpace
			ImGuiRenderAll ();

			ImGui::End ();
		}
	}
	void TestsLayerManager::ImGuiRenderAll ()
	{
		for (Layer *test : m_ActiveTests) {
			if (test)
			{
				////
				// Here Will be code for framebuffer-out -> view-port_Window, new-ImGuiWindow(a persistant one that will force your viewport-window to-be attached to itself), pop-on-close-buttonpress etc.
				////

				test->OnImGuiRender ();
			}else break;
		}
	}
	void TestsLayerManager::ProcessEvent (Event &event)
	{
		for (Layer *test : m_ActiveTests) {
			if (test && !event.Handled) {
				test->OnEvent (event);
			} else break;
		}
	}
	
	void TestsLayerManager::ActivateTest (uint16_t posn)
	{
		if (m_ActiveTests[1] == nullptr) {
			if (m_ActiveTests[0] == nullptr) {
				m_ActiveTests[0] = m_AllTests[posn];
				m_ActiveTests[0]->OnAttach ();
				return;
			}
			m_ActiveTests[1] = m_AllTests[posn];
			m_ActiveTests[1]->OnAttach ();
			return;
		}
		LOG_WARN ("!! No more Active tests allowed !!");
	}
	void TestsLayerManager::DeActivateTest (uint16_t posn)
	{
		constexpr uint16_t Active_Tests_stack_size = sizeof (m_ActiveTests)/sizeof (m_ActiveTests[0]);
		if (posn < Active_Tests_stack_size) {
			if (m_ActiveTests[posn] != nullptr) {
				for (uint16_t i = posn; i < (Active_Tests_stack_size - 1); i++) {
					m_ActiveTests[i] = m_ActiveTests[i + 1];
				}
				m_ActiveTests[Active_Tests_stack_size - 1] = nullptr;
			}
			LOG_WARN ("!! nullptr found at: posn {0}", posn);
		}
		LOG_WARN ("!! Out of bounds: posn {0}, Active_Tests_stack_size {1} !!", posn, Active_Tests_stack_size);
	}
}