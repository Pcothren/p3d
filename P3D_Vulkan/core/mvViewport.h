#pragma once

#include <GLFW/glfw3.h>

namespace DearPy3D {

	void mvInitializeViewport(int width, int height);
	void mvProcessViewportEvents();

	struct mvViewport
	{
		int         width         = 500;
		int         height        = 500;
		GLFWwindow* handle        = nullptr;
		bool        running       = false;
		bool        cursorEnabled = true;
		double      lastX         = 0.0;
		double      lastY         = 0.0;
		double      deltaX        = 0.0;
		double      deltaY        = 0.0;
		bool        resized       = false;
	};

}