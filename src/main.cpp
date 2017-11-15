#include <iostream>
#include <string>

#include <sol/sol.hpp>

#include "engine.h"

int main(int, char * []) {
	try {
		// create engine
		Engine engine;

		// run engine
		engine.run();
	}
	catch (std::runtime_error & err) {
		std::cerr << err.what() << std::endl;

		return 1;
	}

	return 0;
}
