#pragma once

#ifdef LD_PLATFORM_WINDOWS

extern LazyDog::Application* LazyDog::CreateAppliction();

int main(int argc, char** argv)
{
	LazyDog::Log::Init();
	LD_CORE_WARN("Initialized Log!");
	int a = 5;
	LD_INFO("Hello! Var={0}", a);

	auto app = LazyDog::CreateAppliction();
	app->Run();
	delete app;
}

#endif