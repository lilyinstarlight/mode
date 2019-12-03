#ifndef SAVE_H
#define SAVE_H
#include "yaml.h"

class Save : public ModifiableYaml {
	public:
		static Save & get_instance() {
			static Save save;
			return save;
		}

		virtual ~Save() {}

		Save(const Save &) = delete;
		Save & operator=(const Save &) = delete;

	private:
		Save() : ModifiableYaml("save") {}
};
#endif
