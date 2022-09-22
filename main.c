/***********************************************************************/
/* Copyright (c) 2022 Clay Kress                                       */
/*                                                                     */
/* This file is part of VioletGE.                                      */
/* VioletGE is free software: you can redistribute it and/or modify it */
/* under the terms of the GNU General Public License as published by   */
/* the Free Software Foundation, either version 3 of the License, or   */
/* (at your option) any later version.                                 */
/*                                                                     */
/* Foobar is distributed in the hope that it will be useful,           */
/* but WITHOUT ANY WARRANTY; without even the implied warranty         */
/* of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.             */
/*                                                                     */
/* See the GNU General Public License for more details.                */
/*                                                                     */
/* You should have received a copy of the GNU General Public License   */
/* along with Foobar. If not, see <https://www.gnu.org/licenses/>.     */
/*                                                                     */
/***********************************************************************/

/*****************************************************************************************************************************************************************/
#include <stdlib.h>
#include <stdio.h>

#define GLEW_STATIC
#include "lib/glew.h"
#include "lib/glfw3.h"

#include "code/list.h"
#include "code/map.h"
#include "code/clag.h"
#include "code/openGL.h"

#define WIDTH 1440
#define HEIGHT 810

float buf[9]= {
     -0.5, -0.5, 0.0,
      0.5, -0.5, 0.0,
      0.0,  0.5, 0.0
};
unsigned int index[3]= {
      0, 1, 2
};

int main(int argc, char** argv) {
      printf("Hello, World!\n");
      stream= stdout;
      Window w;
      window_open(&w, "Minecraft", WIDTH, HEIGHT);
      window_setIcon(&w, "icon.png");
      VideoShader s;
      videoShader_load(&s, "shaders/defaultShader.vs.glsl", "shaders/defaultShader.fs.glsl", "Basic Shader");
      videoShader_bind(&s);
      VertexBuffer vb;
      IndexBuffer ib;
      VertexArrayObject vao;
      vertexBuffer_init(&vb, buf, 3, 3);
      vertexArrayObject_init(&vao, 3);
      vertexArrayObject_addElement(&vao, 3);
      indexBuffer_init(&ib, index, 3);
      do {
            glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, NULL);
      } while (window_update(&w));
      window_delete(&w);
      glfwTerminate();
      return EXIT_SUCCESS;
}
