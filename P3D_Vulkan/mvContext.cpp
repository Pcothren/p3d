#include "mvContext.h"

namespace DearPy3D {

	extern mvContext* GContext = nullptr;

	void CreateContext()
	{
		GContext = new mvContext();
	}

	void DestroyContext()
	{
		glfwTerminate();
		delete GContext;
		GContext = nullptr;
	}

}