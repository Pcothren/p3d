#include "mvViewport.h"
#include "mvContext.h"

namespace DearPy3D {

	static void EnableCursor()
	{
		GContext->viewport.cursorEnabled = true;
		glfwSetInputMode(GContext->viewport.handle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	static void DisableCursor()
	{
		GContext->viewport.cursorEnabled = false;
		glfwSetInputMode(GContext->viewport.handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		{
			if (GContext->viewport.cursorEnabled)
				DisableCursor();
			else
				EnableCursor();
		}
	}

	static void mouse_callback(GLFWwindow* window, double xpos, double ypos)
	{
		float xoffset = xpos - GContext->viewport.lastX;
		float yoffset = ypos - GContext->viewport.lastY;
		GContext->viewport.deltaX = xoffset;
		GContext->viewport.deltaY = yoffset;
		GContext->viewport.lastX = xpos;
		GContext->viewport.lastY = ypos;
	}

	static void framebufferResizeCallback(GLFWwindow* window, int width, int height)
	{
		GContext->viewport.resized = true;
	}

	void mvInitializeViewport(int width, int height)
	{
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		//glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		GContext->viewport.handle = glfwCreateWindow(width, height, "Dear Py3D", nullptr, nullptr);
		glfwSetCursorPosCallback(GContext->viewport.handle, mouse_callback);
		glfwSetKeyCallback(GContext->viewport.handle, key_callback);
		glfwSetWindowSizeCallback(GContext->viewport.handle, framebufferResizeCallback);

		GContext->viewport.width = width;
		GContext->viewport.height = height;
		GContext->viewport.running = true;
	}

	void mvProcessViewportEvents()
	{
		GContext->viewport.deltaX = 0.0;
		GContext->viewport.deltaY = 0.0;
		GContext->viewport.running = !glfwWindowShouldClose(GContext->viewport.handle);
		glfwPollEvents();
	}



}