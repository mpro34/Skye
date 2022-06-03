# Skye
A modern C++ game engine.


#### Getting Started
- Building the Project:
On Windows, run the `GenerateProjects.bat` file to re-generate all visual studio sources files.

- Running the project:
Open the visual studio solution file in Skye.sln, run the project with F5.

#### Project Structure
##### Submodules
In order to support external dependencies, I am using git submodules for the following, which all all stored in the /vendor sub-directory:
- spdlog: A fast c++ logging library
- GLFW: A helper library to support OpenGL renderering.
- imgui: A fast c++ gui library.
- glm: An all-purpose 3D math library.

##### Sandbox
This directory is an example Game that is built on the Skye engine. It creates and runs a `Skye` Application which provides all the necessary bindings to render objects.

##### Skye
This directory is the main entry point for the engine.
- `Skye/src`
    - We use precompiled headers in `skpch.hpp` for generic std library c++ functions.
    - `Skye.hpp` is the Skye API abstraction that applications using the engine should reference.

- `Skye/src/Platform`
    - Contains Graphics API abstractions so that the engine can be used with OpenGL, DirectX11, Vulkan, and DirectX12.
    - Each Graphics API abstractions is within its own directory , i.e. OpenGL.
    - Also contained in this directory is a `Windows` folder which contains Window specific rendering logic. (Will be only OS supported for now).

- **Layers**: I define a concept, called Layers, which are rendered to the screen via the supported Graphics API. Overlays are a specific layer type that are always rendered on-top. An example of an overlay is the IMGUI Debug menus that are rendered on top of the game layer.
- **Mouse and Key Codes**: In order to support other Graphics APIs, I define Skye mouse and keycodes to stray away from DirectX or OpenGL specific codes.
- **Application**: An instance of an application is created by users of the Skye engine, i.e. Sandbox. The Application classes provides the main source of calling the Skye Rendering API.

- `Skye/src/Skye/Events`
  **Events**: The input and layer levels will communicate through events. There are Application, Key and Mouse Events that will be dispatched to the Application for processing.

- `Skye/src/Skye/ImGui`
  ImGui is a ui library which we import as a submodule. I provide a wrapper around ImGui in order to more easily add and modify debug menus as an overlay.
  
- `Skye/src/Skye/Renderer`
  The generic Rendering API that is subclassed in the `Platform` directory. Classes in this directory should be subclassed per Graphics API (i.e. OpenGL,DirectX)
  - `GraphicsContext`: Grabs the graphics context based on the Graphics API in use.
  - `Buffer`: The generic buffer object which is subclassed for vertex and index buffers.
  - `RenderCommand`: A static abstract class that defines functions to render data from a vertex buffer.
  - `Renderer`: Defines more generic render functions that will focus on drawing an entire scene and not just a single mesh.
  - `RendererAPI`: Used to define the type of Graphics API in use.
  - `Shader`: Defines an abstraction to create, compile and use a shader program with a vertex and fragment shader.
  - `VertexArray`: Defines the concept of a vertex array that hold pointers to 1 or more vertex buffer and a single index buffer.

### 2D Sprite Sheet Render Example - Sandbox 2D
Sandbox2D is a sample class that utilizes the Skye game engine to render 2d sprites. Gameplay can be added in the `OnUpdate()` function, where each render pass is currently performed.
Skye allows for rendering of 2d sprite, given a sprite sheet and coordinates on said spritesheet. The simple InmGui window shows the rendering statistics, as well as a selectable 2d texture that will draw on Left Mouse Click. My main goal for Skye was to learn the inner workings of a simple game engine in order to apply this knowledge to larger projects in UE5.
![2D Sprites Render](https://github.com/mpro34/Skye/blob/main/Screenshots/Skye_2drender1.jpg)
