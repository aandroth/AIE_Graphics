#pragma once

#include "RenderObjects.h"

Texture loadTexture(const char *path);

Geometry loadGeometry(const char *path);

Shader loadShader(const char *vpath, const char *fpath);