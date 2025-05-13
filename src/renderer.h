#ifndef RENDERER_H
#define RENDERER_H

#include "list.h"

typedef void (*RenderFunction)(void*); // Pass object to render via void* argument

typedef struct {
    List renderPrepareFunctions;
    List renderFunctions; // List of RenderFunctions to call
    List renderObjects; // List of objects which the renderfunctions will render
} Renderer;

Renderer createRenderer(void);
void renderer_addObject(Renderer* self, RenderFunction renderPrepareFuncPtr, RenderFunction renderFuncPtr, void* objectPtr);
void renderer_prepare(Renderer* self);
void renderer_render(Renderer* self);

#endif
