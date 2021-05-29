#pragma once
#include "GLCore/Core/Layer.h"
#include "GLCore/Util/Core/Framebuffer.h"
namespace GLCore
{
	class TestBase
		: public Layer
	{
	public:
		TestBase(const std::string& name = "TestBase", const std::string& discription = "No_Discripton");
		virtual ~TestBase () = default;

		void BindFramebuffer ();
		void UnBindFramebuffer ();
		uint64_t GetColorAttachmentID ();

		virtual void ImGuiMenuOptions() {}

		const std::string &GetDiscription () { return m_TestDiscription; }
	private:
		//////
		// Frame-buffer and etc.
		/////
		Utils::Framebuffer m_Framebuffer;

		std::string m_TestDiscription;
	};
}