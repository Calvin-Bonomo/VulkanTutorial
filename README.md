# VulkanTutorial

A Vulkan renderer following the tutorial at [Vulkan Tutorial](https://vulkan-tutorial.com/Introduction) with some modifications to the coding style for legibility.

## Learning Goals
I started this project in order to learn about Vulkan and modern 3D rendering technques and APIs. 

## Thoughts
Vulkan is unweildy at its best, and this project really threw me into the deep end. Every single line of 
code I wrote opened a world of articles detailing how what I did wasn't the right way to do it or how it 
could be optimized. I intended to use this project as a base for future projects, but that seems like a 
bad idea now. I'm glad that I'm working on this project to gain a deeper understanding of 3d rendering, 
but this would likely lead to more problems down the line to the detriment of my future projects.

## How to Compile
**You must use Visual Studio 2022 to compile this project!**

1. Download Vulkan 1.3.283.0 and clone this project (or retarget it in linker and include tabs).
2. Retarget include and library dependencies to Vulkan.
3. That's it!

## Future Considerations
- Use smartpointers instead of raw pointers.
- ~~Move the renderer into another class, App should just be asking the renderer to draw stuff.~~
  - The app is the renderer, but some objects like vertex buffers should be put into classes.
- ~~Extend for use with Direct3D and/or OpenGL.~~
  - This is a vulkan renderer; anything like this should be forked into a new project.
- Write a custom allocation API to fall more in line with this [article](https://developer.nvidia.com/vulkan-memory-management).
- Move to cmake or another compiler for portability.
