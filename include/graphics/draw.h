#pragma once

#include "RenderObjects.h"

void s0_draw(const Framebuffer &f, const Shader &s, const Geometry &g);

void setUniform(const Shader &shader, int location, float value);
void setUniform(const Shader &shader, int location, int value);
void setUniform(const Shader &shader, int location, const Texture &value, unsigned slot);


void clearFrameBuffer(const Framebuffer & fb);