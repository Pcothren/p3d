#include "p3d.h"
#include <assert.h>

namespace p3d {

	extern cContext* GContext = nullptr;

	void cCreateContext()
	{
		if (GContext)
		{
			assert(false && "Context already created.");
			return;
		}

		GContext = new cContext();
	}

	void cDestroyContext()
	{
		if (GContext)
		{
			delete GContext;
			GContext = nullptr;
			return;
		}

		assert(false && "Context already destroyed.");
	}

}