# LazyDog

> [English](README.md)

LazyDog 是一个基于 C++17 的 Windows 游戏引擎，采用 OpenGL 渲染后端，以层（Layer）驱动架构为核心进行设计。

## 特性

- **层系统（Layer Stack）** — 支持将逻辑拆分为多个可叠加、可覆盖的层，方便模块化管理
- **事件系统（Event System）** — 键盘、鼠标、窗口事件的统一分发与处理
- **渲染器（Renderer）**
  - 正交相机（OrthographicCamera）及相机控制器
  - Vertex Buffer / Index Buffer / Vertex Array 抽象
  - Shader 及 ShaderLibrary（支持内联字符串与 `.glsl` 文件加载）
  - Texture2D（基于 stb_image）
  - RenderCommand 与 RendererAPI 的平台抽象层
- **ImGui 集成** — 开箱即用的调试 UI 覆盖层
- **日志系统** — 基于 spdlog，区分引擎核心日志与客户端日志
- **输入系统（Input）** — 跨层轮询键盘/鼠标状态
- **Timestep** — 帧时间步长封装，用于与帧率无关的更新逻辑

## 目录结构

```
LazyDog/
├── LazyDog/               # 引擎静态库
│   ├── src/
│   │   ├── LazyDog/       # 引擎核心（Application、Layer、Input、Renderer 等）
│   │   └── Platform/      # 平台相关实现
│   │       ├── OpenGL/    # OpenGL 渲染后端
│   │       └── Windows/   # Windows 窗口与输入
│   └── vendor/            # 第三方依赖
│       ├── GLFW/
│       ├── GLAD/
│       ├── imgui/
│       ├── glm/
│       ├── spdlog/
│       └── stb_image/
├── Sandbox/               # 示例应用程序
│   └── src/
│       └── SandboxApp.cpp
└── premake5.lua           # 构建脚本
```

## 依赖

| 库 | 用途 |
|---|---|
| [GLFW](https://www.glfw.org/) | 窗口创建与输入 |
| [GLAD](https://glad.dav1d.de/) | OpenGL 函数加载 |
| [Dear ImGui](https://github.com/ocornut/imgui) | 调试 UI |
| [glm](https://github.com/g-truc/glm) | 数学库（向量、矩阵） |
| [spdlog](https://github.com/gabime/spdlog) | 日志 |
| [stb_image](https://github.com/nothings/stb) | 图片加载 |

## 构建

### 前置条件

- Windows 10 / 11（目前仅支持 Windows 平台）
- Visual Studio 2019 或 2022
- [Premake5](https://premake.github.io/)

### 生成项目

在项目根目录运行：

```bat
premake5 vs2022
```

生成 Visual Studio 解决方案后，以 `Sandbox` 为启动项目进行编译与运行。

支持三种构建配置：

| 配置 | 说明 |
|---|---|
| `Debug` | 开启符号信息与断言 |
| `Release` | 开启优化，保留日志 |
| `Dist` | 发布版本，最大优化 |

## 快速上手

继承 `LazyDog::Application` 并推送自定义层即可启动引擎：

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
