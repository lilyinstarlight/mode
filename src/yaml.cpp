#include "yaml.h"

Yaml::Yaml(const std::string & filename) : path("data"), file(path + "/" + filename), parent(nullptr), root(YAML::LoadFile(file)) {}

Yaml::Yaml(const Yaml & yaml) : path(yaml.path), file(yaml.file), parent(yaml.parent), root(yaml.root) {}

Yaml::Yaml(const Yaml & yaml, YAML::Node & base) : path(yaml.path), file(yaml.file), parent(&yaml), root(base) {}

const Yaml & Yaml::operator[](const unsigned int idx) const {
	return Yaml(*this, root[idx]);
}

Yaml & Yaml::operator[](const unsigned int idx) {
	return Yaml(*this, root[idx]);
}

const std::string & Yaml::operator[](const std::string & key) {
	return root[idx];
}

const Yaml & Yaml::operator[](const std::string & key) const {
	return Yaml(*this, root[key]);
}

Yaml & Yaml::operator[](const std::string & key) {
	return Yaml(*this, root[key]);
}

std::vector<const std::string &> Yaml::get_keys() const {
	std::vector<const std::string &> vec;

	for (const std::pair<const std::string &, YAML::Node> & item : root)
		vec.push_back(item.first);

	return vec;
}

std::vector<const Yaml> Yaml::get_values() const {
	std::vector<const Yaml> vec;

	for (const YAML::Node & base : root)
		vec.push_back(Yaml(*this, base));

	return vec;
}

std::vector<Yaml> Yaml::get_values() {
	std::vector<Yaml> vec;

	for (YAML::Node & base : root)
		vec.push_back(Yaml(*this, base));

	return vec;
}

std::map<std::string, const Yaml> Yaml::get_items() const {
	std::map<std::string, const Yaml> map;

	for (const std::pair<const std::string &, YAML::Node> & item : root)
		map[item.first] = Yaml(*this, item.second);

	return map;
}

std::map<std::string, Yaml> Yaml::get_items() {
	std::map<std::string, Yaml> map;

	for (std::pair<const std::string &, YAML::Node> & item : root)
		map[item.first] = Yaml(*this, item.second);

	return map;
}

bool Yaml::check() const {
	return root;
}

bool Yaml::get_bool() const {
	return root.as<bool>();
}

int Yaml::get_int() const {
	return root.as<int>();
}

float Yaml::get_float() const {
	return root.as<float>();
}

const std::string & Yaml::get_str() const {
	return root.as<const std::string &>();
}

const Vector2f & Yaml::get_vec() const {
	return root.as<const Vector2f &>();
}

const SDL_Color & Yaml::get_color() const {
	return root.as<const SDL_Color &>();
}

void Yaml::set_bool(bool val) {
	root = val;
}

void Yaml::set_int(int val) {
	root = val;
}

void Yaml::set_float(float val) {
	root = val;
}

void Yaml::set_str(const std::string & val) {
	root = val;
}

void Yaml::set_vec(const Vector2f & vec) {
	root = vec;
}

void Yaml::set_color(const SDL_Color & color) {
	root = color;
}
