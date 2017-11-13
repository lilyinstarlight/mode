#include <sstream>
#include <string>

#include "spec.h"

Spec & Spec::get_instance() {
	static Spec gamedata;
	return gamedata;
}

Spec::Spec() : path("xml"), parser(path + "/spec.xml"), data(parser.get_data()) {}

bool Spec::check(const std::string & tag) const {
	return data.count(tag) != 0;
}

bool Spec::get_bool(const std::string & tag) const {
	std::map<std::string, std::string>::const_iterator pos = data.find(tag);

	if (pos == data.end())
		throw "Boolean tag `" + tag + "' not found";

	return pos->second == "true";
}

int Spec::get_int(const std::string & tag) const {
	std::map<std::string, std::string>::const_iterator pos = data.find(tag);

	if (pos == data.end())
		throw "Integer tag `" + tag + "' not found";

	int data;

	std::stringstream ss;
	ss << pos->second;
	ss >> data;

	return data;
}

float Spec::get_float(const std::string & tag) const {
	std::map<std::string, std::string>::const_iterator pos = data.find(tag);

	if (pos == data.end())
		throw "Float tag `" + tag + "' not found";

	float data;

	std::stringstream ss;
	ss << pos->second;
	ss >> data;

	return data;
}

const std::string & Spec::get_str(const std::string & tag) const {
	std::map<std::string, std::string>::const_iterator pos = data.find(tag);

	if (pos == data.end())
		throw "String tag `" + tag + "' not found";

	return pos->second;
}
