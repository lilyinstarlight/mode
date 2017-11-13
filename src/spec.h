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

		bool check(const std::string & tag) const;

		bool get_bool(const std::string & tag) const;
		int get_int(const std::string & tag) const;
		float get_float(const std::string & tag) const;
		const std::string & get_str(const std::string & tag) const;

	private:
		Spec();
		~Spec() {}

		std::string path;

		XML parser;

		const std::map<std::string, std::string> data;
};
#endif
