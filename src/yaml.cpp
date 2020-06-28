#include <fstream>
#include <sstream>

#include "yaml.h"

Yaml::Yaml(const std::string & filename, const std::string & path) : _file(), _root(), _path(path) {
	load(filename);
}

Yaml::Yaml(const Yaml & yaml) : _file(yaml._file), _root(yaml._root), _path(yaml._path) {}

void Yaml::load(const std::string & filename) {
	_file = _path + "/" + filename + ".yaml";

	std::ifstream fin(_file);
	if (fin)
		_root = YAML::Load(fin);

	if (!_root.IsMap())
		_root = YAML::Node(YAML::NodeType::Map);
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

std::string Yaml::parent(const std::string & yaml_path) const {
	std::string::size_type pos = yaml_path.rfind('/');
	if (pos == std::string::npos)
		return "";

	return yaml_path.substr(0, pos);
}

std::string Yaml::leaf(const std::string & yaml_path) const {
	std::string::size_type pos = yaml_path.rfind('/');
	if (pos == std::string::npos)
		return yaml_path;

	return yaml_path.substr(pos + 1);
}

YAML::Node Yaml::resolve(const std::string & yaml_path) const {
	YAML::Node node = _root;

	std::stringstream ss(yaml_path);
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
	std::ofstream out(_file);
	if (_root.size() > 0)
		out << _root << std::endl;
}
