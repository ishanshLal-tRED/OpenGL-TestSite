#include "GLCore.h"
#include "ExampleLayer.h"

using namespace GLCore;

class Example : public Application
{
public:
	Example()
		: Application("OpenGL Examples")
	{
		PushLayer<ExampleLayer> ();
		PushLayer<ExampleLayer> ("Example Layer 2");
		//ActivateLayer (0);
	}
};

int main()
{
	std::unique_ptr<Example> app = std::make_unique<Example>();
	app->Run();
}