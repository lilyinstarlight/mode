#include <iostream>
#include <string>

#include "engine.h"

int main(int, char * []) {
#ifdef NDEBUG
	try {
#endif
		// run engine
		Engine::get_instance().run();
#ifdef NDEBUG
	}
	catch (std::runtime_error & err) {
		std::cerr << err.what() << std::endl;

		return 1;
	}
#endif

	return 0;
}
