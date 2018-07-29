#ifndef SAVE_H
#define SAVE_H
#include "yaml.h"

class Save : public Yaml {
	public:
		static Save & get_instance() {
			static Save save;
			return save;
		}

		~Save() {}

		Save(const Save &) = delete;
		Save & operator=(const Save &) = delete;

	private:
		Save() : Yaml("save.yaml") {}
};
#endif
