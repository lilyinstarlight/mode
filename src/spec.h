#ifndef SPEC_H
#define SPEC_H
#include "yaml.h"

class Spec : public Yaml {
	public:
		static Spec & get_instance() {
			static Spec spec;
			return spec;
		}

		virtual ~Spec() {}

		Spec(const Spec &) = delete;
		const Spec & operator=(const Spec &) = delete;

	private:
		Spec() : Yaml("game") {}
};
#endif
