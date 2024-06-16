# VulkanTutorial

A Vulkan renderer following the tutorial at [Vulkan Tutorial](https://vulkan-tutorial.com/Introduction) with some modifications to the coding style for legibility. I started this project in order to learn about Vulkan and modern 3D rendering technques and APIs.

## Future Considerations
- Use smartpointers instead of raw pointers.
- ~~Move the renderer into another class, App should just be asking the renderer to draw stuff.~~
  - The app is the renderer, but some objects like vertex buffers should be put into classes.
- ~~Extend for use with Direct3D and/or OpenGL.~~
  - This is a vulkan renderer; anything like this should be forked into a new project.
- Write a custom allocation API to fall more in line with this [article](https://developer.nvidia.com/vulkan-memory-management).
- Please for the love of god document this.
