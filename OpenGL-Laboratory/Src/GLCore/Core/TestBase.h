#pragma once
#include "GLCore/Core/Layer.h"

namespace GLCore
{
	class TestBase
		: public Layer
	{
	public:
		TestBase(const std::string& name = "TestBase", const std::string& discription = "No_Discripton");
		virtual ~TestBase () = default;

		virtual void OnImGuiMenuOptions() {}

		const std::string &GetDiscription () { return m_TestDiscription; }
	private:
		//////
		// Frame-buffer and etc.
		/////

		std::string m_TestDiscription;
	};
}