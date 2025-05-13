#include "renderer.h"

Renderer createRenderer(void) {
    Renderer self;
    self.renderPrepareFunctions= initList(sizeof(RenderFunction), 0);
    self.renderFunctions= initList(sizeof(RenderFunction), 0);
    self.renderObjects= initList(sizeof(void*), 0);
    return self;
}

void renderer_addObject(Renderer* self, RenderFunction renderPrepareFuncPtr, RenderFunction renderFuncPtr, void* objectPtr) {
    list_push(&self->renderPrepareFunctions, &renderPrepareFuncPtr);
    list_push(&self->renderFunctions, &renderFuncPtr);
    list_push(&self->renderObjects, &objectPtr);
}

void renderer_prepare(Renderer* self) {
    for (int i= 0; i < list_len(&self->renderObjects); i++) {
        (*(RenderFunction*) list_getPtr(&self->renderPrepareFunctions, i))(*(void**) list_getPtr(&self->renderObjects, i)); // Call each render prepare function
    }
}

void renderer_render(Renderer* self) {
    for (int i= 0; i < list_len(&self->renderObjects); i++) {
        (*(RenderFunction*) list_getPtr(&self->renderFunctions, i))(*(void**) list_getPtr(&self->renderObjects, i)); // Call each render function
    }
}
