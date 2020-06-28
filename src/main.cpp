#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <string>

#include "engine.h"

#define MACRO_STRING(str) STRING(str)
#define STRING(str) #str

#define RESOURCE_STR MACRO_STRING(RESOURCE)

int main(int argc, char * argv[]) {
	try {
		if (argc == 1) {
			if constexpr ((sizeof RESOURCE_STR) > 1) {
				if constexpr (RESOURCE_STR[0] == '/')
					std::filesystem::current_path(std::filesystem::path(RESOURCE_STR));
				else
					std::filesystem::current_path(std::filesystem::canonical(std::filesystem::path(argv[0])).parent_path() / std::filesystem::path(RESOURCE_STR));
			}
		}
		else if (argc == 2) {
			std::filesystem::current_path(std::filesystem::path(argv[1]));
		}
		else {
			std::cerr << "Usage: " << std::filesystem::path(argv[0]).filename().string() << " [gamedir]" << std::endl;

			return 1;
		}
	}
	catch (std::filesystem::filesystem_error & err) {
		std::cerr << err.what() << std::endl;

		return 1;
	}

#ifdef NDEBUG
	try {
#endif
		// load level 1 world
		Engine::get_instance().load("game");

		// run engine
		Engine::get_instance().start();
#ifdef NDEBUG
	}
	catch (std::runtime_error & err) {
		std::cerr << err.what() << std::endl;

		return 1;
	}
#endif

	return 0;
}
