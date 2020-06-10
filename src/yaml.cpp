#include <fstream>
#include <sstream>

#include "yaml.h"

Yaml::Yaml(const std::string & filename, const std::string & path) : path(path), file(), root() {
	load(filename);
}

Yaml::Yaml(const Yaml & yaml) : path(yaml.path), file(yaml.file), root(yaml.root) {}

void Yaml::load(const std::string & filename) {
	file = path + "/" + filename + ".yaml";

	std::ifstream fin(file);
	if (fin)
		root = YAML::Load(fin);

	if (!root.IsMap())
		root = YAML::Node(YAML::NodeType::Map);
}

std::vector<std::string> Yaml::get_keys(const std::string & key) const {
	YAML::Node node = resolve(key);

	std::vector<std::string> keys;
	keys.reserve(node.size());

	for (YAML::const_iterator it = node.begin(); it != node.end(); ++it)
		keys.push_back(it->first.as<std::string>());

	return keys;
}

bool Yaml::check(const std::string & key) const {
	YAML::Node node = resolve(key);

	return node.IsDefined() && !node.IsNull();
}

bool Yaml::get_bool(const std::string & key) const {
	return resolve(key).as<bool>();
}

int Yaml::get_int(const std::string & key) const {
	return resolve(key).as<int>();
}

float Yaml::get_float(const std::string & key) const {
	return resolve(key).as<float>();
}

std::string Yaml::get_str(const std::string & key) const {
	return resolve(key).as<std::string>();
}

std::string Yaml::parent(const std::string & path) const {
	std::string::size_type pos = path.rfind('/');
	if (pos == std::string::npos)
		return "";

	return path.substr(0, pos);
}

std::string Yaml::leaf(const std::string & path) const {
	std::string::size_type pos = path.rfind('/');
	if (pos == std::string::npos)
		return path;

	return path.substr(pos + 1);
}

YAML::Node Yaml::resolve(const std::string & path) const {
	YAML::Node node = root;

	std::stringstream ss(path);
	std::string key;
	while (std::getline(ss, key, '/')) {
		if (!node.IsMap())
			return YAML::Node();

		node.reset(node[key]);
	}

	return node;
}

ModifiableYaml::ModifiableYaml(const std::string & filename, const std::string & path) : Yaml(filename, path) {}

ModifiableYaml::ModifiableYaml(const ModifiableYaml & yaml) : Yaml(yaml) {}

void ModifiableYaml::remove(const std::string & key) {
	resolve(parent(key)).remove(leaf(key));
}

void ModifiableYaml::set_bool(const std::string & key, bool val) {
	resolve(key) = val;
}

void ModifiableYaml::set_int(const std::string & key, int val) {
	resolve(key) = val;
}

void ModifiableYaml::set_float(const std::string & key, float val) {
	resolve(key) = val;
}

void ModifiableYaml::set_str(const std::string & key, const std::string & val) {
	resolve(key) = val;
}

void ModifiableYaml::write() const {
	std::ofstream out(file);
	if (root.size() > 0)
		out << root << std::endl;
}
