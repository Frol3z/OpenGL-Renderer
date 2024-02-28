# OpenGL Renderer

Welcome to my OpenGL Renderer project!
This is an ongoing collection of projects where I'm currently experimenting by creating, modifying and destroying stuff during my learning journey in graphics programming.

![image](https://github.com/Frol3z/OpenGL-Renderer/assets/56247530/9e4c529a-930c-42c8-b980-f42cb5ce806d)

## Introduction
I started my journey with zero knowledge about computer graphics and C++, although I did have prior experience with other programming languages and computer science.
This repository is meant to be a place to collect different small projects where I explore and implement computer graphics techniques while simultaneously learning C++ and OpenGL.
Feel free to take a look :)

> Please note that these projects are not polished, the code might not be optimized, and design choices might not be optimal. Despite my efforts to keep everything organized and clean, they are primarily experimental, driven by trial and error, and subject to constant iteration.

## Implemented Features

- Establishment of a basic OpenGL setup utilizing GLFW for efficient window management.
- Rendering capability for fundamental geometric primitives such as points, lines, and triangles.
- Integration of basic shaders (vertex, fragment).
- Application of basic textures.
- Implementation of a 3D camera system employing transformations.
- Implementation of materials based on the Phong shading model to achieve realistic lighting effects.
- Incorporation of diffuse, specular, and emission maps to augment lighting properties and enrich texture appearance.
- Integration of ImGui for real-time scene manipulation, providing an intuitive interface for users.
- Implementation of other shading techniques including Gouraud shading, flat shading, and Gooch shading to diversify rendering aesthetics.
- Incorporation of directional and point light sources to simulate realistic lighting conditions within the rendered scene.

## Planned Features

- Addition of support for spotlights, allowing for more advanced lighting techniques.
- Implementation of a flashlight mechanic using spotlights.
- Support for loading and rendering 3D models from external files.
- Implementation of post-processing effects such as bloom and depth of field to enhance visual quality and immersion.
- Others to come...

## Learning Objectives

My main learning objectives with this project include:
- Mastering fundamental OpenGL concepts and rendering techniques.
- Enhancing proficiency in C++ programming and software development practices.
- Gaining practical experience in building a graphics rendering engine from scratch.
- Learning computer graphics principles and techniques to understand the underlying concepts driving the rendering process.

## Challenges Faced

Throughout the development process, I encountered various challenges such as:
- Abstracting OpenGL functionality to create a flexible and scalable rendering engine.

## Lessons Learned

Some of the most valuable lessons I've learned while working on this project include:
- The importance of proper memory management and resource handling in graphics programming.
- Understanding the intricate nature of building and maintaining a graphics rendering engine, including the complexities of abstraction and optimization.

## Next Steps

Moving forward, I plan to:
- Explore post-processing shaders such as depth of field, bloom, posterization.
- Explore advanced techniques for rendering particles, water, and hair.
- Dive deeper into GPU programming with compute shaders and parallel computing.
- Experiment with integrating other graphics APIs and libraries such as Vulkan and Direct3D.

## Usage

### Camera Controls

- **Movement**: Use the arrow keys or W, A, S, D keys to move the camera forward, backward, left, and right, respectively.
- **Rotation**: Drag the mouse to rotate the camera's view.
- **Zoom**: Use the scroll-wheel to zoom in and out.

### Other Controls
- Press ESC to close the application
- Right click to free the mouse.
- Press F to toggle the flashlight.
- Press G to toggle wireframe mode.
- Press the F11 key to toggle borderless fullscreen mode.

## Resources
These are the resources I use for learning:

- [Real-Time Rendering](http://www.realtimerendering.com/)
- [Learn OpenGL](https://learnopengl.com/)
- [The Cherno YouTube Channel](https://www.youtube.com/user/TheChernoProject)
