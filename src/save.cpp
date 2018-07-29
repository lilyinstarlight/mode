#include "save.h"

void Save::save() const {
	if (parent) {
		parent->save();
	}
	else {
		std::ofstream out(file);
		out << root;
	}
}
