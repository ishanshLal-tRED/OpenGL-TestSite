#include "pch.h"
#include "TestBase.h"

#include "GLCore/Util/Core/Framebuffer.h"

namespace GLCore
{
	TestBase::TestBase (const std::string &name, const std::string &discription)
		: Layer (name), m_TestDiscription (discription), m_Framebuffer (Utils::FramebufferSpecification{ 600, 600, { Utils::FramebufferTextureFormat::RGBA8, Utils::FramebufferTextureFormat::Depth } })
	{
	}
	void TestBase::BindFramebuffer ()
	{
		m_Framebuffer.Bind ();
	}
	void TestBase::UnBindFramebuffer ()
	{
		m_Framebuffer.Unbind ();
	}
	uint64_t TestBase::GetColorAttachmentID ()
	{
		return m_Framebuffer.GetColorAttachmentRendererID ();
	}
}