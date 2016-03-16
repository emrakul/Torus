#ifndef _MAIN_H
#define _MAIN_H


#include <gl/glew.h>
#include <gl/wglew.h>


#include <gl/gl.h>
#include <gl/glext.h>
#include <gl/glu.h>

#include <GLFW/glfw3.h>

#include "Utility.h"

#include "Shader.h"
#include "Quad.h"
#include "Camera.h"
#include "ControlPoint.h"

#if defined(_WIN32)
#include <Windows.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#elif defined(__linux__)
#include <X11/X.h>
#include <X11/extensions/Xrandr.h>
#define GLFW_EXPOSE_NATIVE_X11
#define GLFW_EXPOSE_NATIVE_GLX
#endif

#if !defined(__APPLE__)
#include <GLFW/glfw3native.h>
#endif

#endif

