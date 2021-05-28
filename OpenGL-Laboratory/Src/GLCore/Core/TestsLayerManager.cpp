#include "pch.h"
#include "TestsLayerManager.h"

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
		ImGuiRenderAll ();
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