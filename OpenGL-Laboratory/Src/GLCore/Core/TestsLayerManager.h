#pragma once

namespace GLCore
{
	class TestBase;

	class TestsLayerManager
	{
	public:
		TestsLayerManager () = default;
		~TestsLayerManager ();

		template<class Typ>
		void PushTest ()
		{
			m_AllTests.push_back (static_cast<TestBase *>(new Typ ()));
		}

		void UpdateActiveLayers (Timestep deltatime);
		void ImGuiRender ();
		void ImGuiRenderAll ();
		void ProcessEvent (Event &event);

		void ActivateTest (uint16_t posn);
		void DeActivateTest (uint16_t posn);
	private:
		uint32_t m_DockspaceID;
		std::array  <TestBase*,2> m_ActiveTests = { nullptr, nullptr };
		std::vector <TestBase*>   m_AllTests;
	};
}