#include <iostream>
#include <string>

#include <sol/sol.hpp>

#include "engine.h"

int main(int, char * []) {
	try {
		// run engine
		Engine::get_instance().run();
	}
	catch (std::runtime_error & err) {
		std::cerr << err.what() << std::endl;

		return 1;
	}

	return 0;
}
