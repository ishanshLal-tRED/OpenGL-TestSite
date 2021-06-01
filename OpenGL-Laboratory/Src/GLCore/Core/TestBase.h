#pragma once
#include "GLCore/Core/Layer.h"
#include "GLCore/Util/Core/Framebuffer.h"
#include <glm/glm.hpp>
#include "GLCore/Events/LayerEvent.h"

namespace GLCore
{
	class TestsLayerManager;
	class TestBase
		: public Layer
	{
	public:
		TestBase(const std::string& name = "TestBase", const std::string& discription = "No_Discripton");
		virtual ~TestBase () = default;

		virtual void ImGuiMenuOptions() {}

		const std::string &GetDiscription () { return m_TestDiscription; }
	protected:
		enum Flags
		{
			None = 0,
			Viewport_Focused = BIT (0),
			Viewport_Hovered = BIT (1)
		};
		bool CheckFlags (int flags)
		{
			return m_Flags & flags;
		}
	private:
		void FlagSetter (Flags, bool);
		void FilteredEvent (Event &event);
		friend class TestsLayerManager;
	private:
		//////
		// Frame-buffer and etc.
		/////
		int m_Flags;
		Utils::Framebuffer m_Framebuffer;
		glm::vec2 m_ViewPortSize;

		std::string m_TestDiscription;
	};
}