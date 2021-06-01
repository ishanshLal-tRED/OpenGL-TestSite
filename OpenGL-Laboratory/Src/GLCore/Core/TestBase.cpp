#include "pch.h"
#include "TestBase.h"
#include "GLCore/Events/MouseEvent.h"
#include "GLCore/Events/KeyEvent.h"
#include "GLCore/Events/LayerEvent.h"

namespace GLCore
{
	TestBase::TestBase (const std::string &name, const std::string &discription)
		: Layer (name), m_TestDiscription (discription), m_Framebuffer (Utils::FramebufferSpecification{ 600, 600, { Utils::FramebufferTextureFormat::RGBA8, Utils::FramebufferTextureFormat::Depth } })
	{	}

	void TestBase::FlagSetter (Flags flag, bool val)
	{
		if (val == (bool)(m_Flags & flag))
			return;
		if (val) {
			m_Flags |= flag; // Set bit
			switch (flag)
			{
				case Flags::Viewport_Focused:
					LayerViewportFocusEvent event;
					OnEvent (event); break;
			}
		} else {
			m_Flags &= ~(flag); // Clear bit
			switch (flag) {
				case Flags::Viewport_Focused:
					LayerViewportLostFocusEvent event;
					OnEvent (event); break;
			}
		}
	}

	void TestBase::FilteredEvent (Event &event)
	{
		EventDispatcher dispatcher (event);
		// Input
		dispatcher.CategoryDispatch<EventCategory::EventCategoryMouseButton> (
			[&](Event &e) {
				if (m_Flags & Flags::Viewport_Focused)
					OnEvent (e);
				return e.Handled;
			});
		dispatcher.CategoryDispatch<EventCategory::EventCategoryKeyboard> (
			[&](Event &e) {
				if (m_Flags & Flags::Viewport_Focused)
					OnEvent (e);
				return e.Handled;
			});
		dispatcher.Dispatch<MouseScrolledEvent> (
			[&](MouseScrolledEvent &e) {
				if (m_Flags & Flags::Viewport_Focused)
					OnEvent (e);
				return e.Handled;
			});
		dispatcher.Dispatch<MouseMovedEvent> (
			[&](MouseMovedEvent &e) {
				if (m_Flags & Flags::Viewport_Hovered)
					OnEvent (e);
				return e.Handled;
			});
	}
}