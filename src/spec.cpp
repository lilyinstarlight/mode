#include <sstream>
#include <string>
#include <unordered_set>

#include "spec.h"

Spec & Spec::get_instance() {
	static Spec spec;
	return spec;
}

Spec::Spec() : path("spec"), parser(path + "/game.xml"), data(parser.get_data()) {}

bool Spec::check(const std::string & tag) const {
	return data.find(tag) != data.end();
}

std::vector<std::string> Spec::get_tops() const {
	// find top level elements
	std::unordered_set<std::string> found;

	for (std::pair<std::string, std::string> element : data) {
		std::string::size_type slash = element.first.find("/");
		if (slash == std::string::npos)
			continue;

		found.insert(element.first.substr(0, slash));
	}

	return std::vector<std::string>(found.begin(), found.end());
}

std::vector<std::string> Spec::get_subs(const std::string & tag) const {
	// find top level subelements of given tag
	std::unordered_set<std::string> found;

	for (std::pair<std::string, std::string> element : data) {
		std::string needle = tag + "/";
		std::string::size_type pos = element.first.find(needle);
		if (pos == std::string::npos)
			continue;

		std::string::size_type begin = pos + needle.length();
		std::string::size_type slash = element.first.find("/", begin);
		if (slash == std::string::npos)
			continue;

		found.insert(element.first.substr(begin, slash - begin));
	}

	return std::vector<std::string>(found.begin(), found.end());
}

bool Spec::get_bool(const std::string & tag) const {
	std::unordered_map<std::string, std::string>::const_iterator pos = data.find(tag);

	if (pos == data.end())
		throw std::runtime_error("Boolean tag `" + tag + "' not found");

	return pos->second == "true";
}

int Spec::get_int(const std::string & tag) const {
	std::unordered_map<std::string, std::string>::const_iterator pos = data.find(tag);

	if (pos == data.end())
		throw std::runtime_error("Integer tag `" + tag + "' not found");

	int data;

	std::stringstream ss;
	ss << pos->second;
	ss >> data;

	return data;
}

float Spec::get_float(const std::string & tag) const {
	std::unordered_map<std::string, std::string>::const_iterator pos = data.find(tag);

	if (pos == data.end())
		throw std::runtime_error("Float tag `" + tag + "' not found");

	float data;

	std::stringstream ss;
	ss << pos->second;
	ss >> data;

	return data;
}

const std::string & Spec::get_str(const std::string & tag) const {
	std::unordered_map<std::string, std::string>::const_iterator pos = data.find(tag);

	if (pos == data.end())
		throw std::runtime_error("String tag `" + tag + "' not found");

	return pos->second;
}
