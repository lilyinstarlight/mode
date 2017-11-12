#include <iostream>
#include <string>

#include "engine.h"

int main(int, char * []) {
	try {
		Engine engine;

		engine.run();
	}
	catch (const std::string & msg) {
		std::cerr << msg << std::endl;

		return 1;
	}

	return 0;
}
