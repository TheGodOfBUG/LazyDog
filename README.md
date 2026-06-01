# LazyDog

> [中文文档](README.zh.md)

LazyDog is a Windows game engine written in C++17, built on an OpenGL rendering backend with a Layer-driven architecture at its core.

## Features

- **Layer Stack** — Modular logic organized as stackable, overlay-able layers
- **Event System** — Unified dispatch and handling of keyboard, mouse, and window events
- **Renderer**
  - Orthographic Camera with a built-in camera controller
  - Vertex Buffer / Index Buffer / Vertex Array abstractions
  - Shader & ShaderLibrary (inline source strings and `.glsl` file loading)
  - Texture2D (powered by stb_image)
  - Platform-agnostic RenderCommand and RendererAPI layer
- **ImGui Integration** — Ready-to-use debug UI overlay
- **Logging** — Built on spdlog with separate engine-core and client loggers
- **Input System** — Poll keyboard/mouse state from any layer
- **Timestep** — Frame-time encapsulation for frame-rate-independent update logic

## Directory Structure

```
LazyDog/
├── LazyDog/               # Engine static library
│   ├── src/
│   │   ├── LazyDog/       # Engine core (Application, Layer, Input, Renderer, ...)
│   │   └── Platform/      # Platform-specific implementations
│   │       ├── OpenGL/    # OpenGL rendering backend
│   │       └── Windows/   # Windows window & input
│   └── vendor/            # Third-party dependencies
│       ├── GLFW/
│       ├── GLAD/
│       ├── imgui/
│       ├── glm/
│       ├── spdlog/
│       └── stb_image/
├── Sandbox/               # Example application
│   └── src/
│       └── SandboxApp.cpp
└── premake5.lua           # Build script
```

## Dependencies

| Library | Purpose |
|---|---|
| [GLFW](https://www.glfw.org/) | Window creation & input |
| [GLAD](https://glad.dav1d.de/) | OpenGL function loading |
| [Dear ImGui](https://github.com/ocornut/imgui) | Debug UI |
| [glm](https://github.com/g-truc/glm) | Math library (vectors, matrices) |
| [spdlog](https://github.com/gabime/spdlog) | Logging |
| [stb_image](https://github.com/nothings/stb) | Image loading |

## Building

### Prerequisites

- Windows 10 / 11 (Windows is the only supported platform at this time)
- Visual Studio 2019 or 2022
- [Premake5](https://premake.github.io/)

### Generate Project Files

Run the following command from the repository root:

```bat
premake5 vs2022
```

Then open the generated `.sln`, set **Sandbox** as the startup project, and build.

Three build configurations are available:

| Configuration | Description |
|---|---|
| `Debug` | Debug symbols enabled, assertions active |
| `Release` | Optimized build, logging retained |
| `Dist` | Distribution build, maximum optimization |

## Quick Start

Inherit from `LazyDog::Application`, push a custom layer, and the engine handles the rest:

```cpp
#include <LazyDog.h>

class MyLayer : public LazyDog::Layer
{
public:
    MyLayer() : Layer("MyLayer") {}

    void OnUpdate(LazyDog::Timestep ts) override
    {
        LazyDog::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
        LazyDog::RenderCommand::Clear();
    }

    void OnImGuiRender() override
    {
        ImGui::Begin("Debug");
        ImGui::Text("Hello, LazyDog!");
        ImGui::End();
    }

    void OnEvent(LazyDog::Event& e) override {}
};

class MyApp : public LazyDog::Application
{
public:
    MyApp() { PushLayer(new MyLayer()); }
};

LazyDog::Application* LazyDog::CreateAppliction()
{
    return new MyApp();
}
```
