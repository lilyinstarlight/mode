#ifndef SPEC_H
#define SPEC_H
#include <map>
#include <string>

#include "xml.h"

class Spec {
	public:
		static Spec & get_instance();

		Spec(const Spec &) = delete;
		Spec & operator=(const Spec &) = delete;

		bool Spec::check(const std::string & tag) const;

		bool Spec::get_bool(const std::string & tag) const;

		int Spec::get_int(const std::string & tag) const;

		float Spec::get_float(const std::string & tag) const;

		const std::string & Spec::get_str(const std::string & tag) const;

	private:
		Spec();
		~Spec();

		XML parser;

		const map<std::string, std::string> data;
};
#endif
