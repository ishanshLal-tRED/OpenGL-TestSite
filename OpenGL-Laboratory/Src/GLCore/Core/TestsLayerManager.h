#include "GLCore/Core/Test.h"

namespace GLCore
{
	class TestsLayerManager
	{
	public:
		TestsLayerManager () = default;
		~TestsLayerManager ();

		template<class Typ>
		void PushTest ()
		{
			m_AllTests.push_back (static_cast<Layer*>(new Typ ()));
		}

		void UpdateActiveLayers (Timestep deltatime);
		void ImGuiRender ();
		void ImGuiRenderAll ();
		void ProcessEvent (Event &event);

		void ActivateTest (uint16_t posn);
		void DeActivateTest (uint16_t posn);
	private:
		std::array<Layer*,2> m_ActiveTests = { nullptr, nullptr };
		std::vector<Layer*> m_AllTests;
	};
}