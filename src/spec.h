#ifndef SPEC_H
#define SPEC_H
#include "yaml.h"

class Spec : public Yaml {
	public:
		static Spec & get_instance() {
			static Spec spec;
			return spec;
		}

		~Spec() {}

		Spec(const Spec &) = delete;
		Spec & operator=(const Spec &) = delete;

		void set_bool(bool val) = delete;
		void set_int(int val) = delete;
		void set_float(float val) = delete;
		void set_str(const std::string & val) = delete;

		void save() = delete;

	private:
		Spec() : Yaml("game") {}
};
#endif
