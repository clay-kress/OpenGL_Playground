Sandbox (Cubed)

My adventures with OpenGL rendering and learning github and git
This will probably involve a minecraft clone written in C or something

- Important: ---

-> Click on the screen to control the player, press Escape to free the mouse
-> Speed up or slow down the sun cycle by scrolling to see the pretty sunset! default should be 30 seconds/day
-> Light up the dark night by pressing 'l'
-> Some of the constellations are from the real world. See how many you can spot

This is mainly a demonstration of postprocessing lighting and UI effects using framebuffers.
All of the matrices were programmed by hand using the functions in linAlg.c


Controls: ---
 WASD, Space, Left Shift, & Mouse - Movement

 Esc          - Frees the mouse
 Scroll wheel - Changes day length
 "\"          - Sets default day length (0.5 minutes/day)

 "l" - Toggles the players head as a light
 "p" - Toggles the (somewhat dodgy) physics controller


The atmosphere and stars are both postprocessing effects

The window can be resized and the framebuffer will update


Progress since the review:
 - Fixed day cycle to be faster and save you time
 - Fixed makefile and includes to use -lGL & gl.h instead of -lGLEW & glew.h


- Less Important: ---

Uses: glfw & glew/gl for Windowing and OpenGL & includes stb_image.h for loading .png files
Pay no attention to the blocky form of the terrain! It is not the important!

notable files:
    The atmospheric lighting & stars (postprocessing):  shaders/framebuffer.fs
    The terrain lighting:                               shaders/basic3D.fs
    The meshing and texture algorithm:                  src/chunkMesh.c & src/ChunkMesh.h
    All matrices & linear algrbra:                      src/linAlg.c & src/linAlg.h


Textures:
All block textures (and the sun) are stored in one image that is loaded using
 stbi_load() and always bound in the OpenGL context. When a block face is
 "created" in chunkMesh.c->addFace() the s,t coordinates are offset to show the texture
 corresponding to the input block type

Atmosphere stuff:
The atmosphere is calculated using separate functions for red, green, and blue
 which depend on the angle of the sun in the sky and the viewing angle of the
 pixel on screen (towards or away from the sun.)

Stars:
The stars are done as a postprocessing effect as well using a cubemap texture.
 The texture is rendered not onto an inside out cube but onto the background
 of the sky using the postprocessing shader.

Terrain lighting:
The terrain lighting is calculated using the classic dot product which is scaled
 so perpendicular faces aren't so dark
The mesh algorithm translates each face in a chunk and sets its texture coordinates
 which index through the "blockAtlas.png" texture.

To be added:

TODO: Clouds:
 Clouds will reflect the color of the sky and glow especially during sunsets
