#ifndef YAML_H
#define YAML_H
#include <map>
#include <string>
#include <vector>

#include <yaml-cpp/yaml.h>
#include <SDL2/SDL.h>

#include "vector2f.h"

namespace YAML {
	template<>
	struct convert<Vector2f> {
	  static Node encode(const Vector2f & vec) {
		Node node;

		node["x"] = vec.get_x();
		node["y"] = vec.get_y();

		return node;
	  }

	  static bool decode(const Node& node, Vector2f & vec) {
		if(!node.IsMap() || node.size() != 2)
		  return false;

		vec.set_x(node["x"].as<float>());
		vec.set_y(node["y"].as<float>());

		return true;
	  }
	};

	template<>
	struct convert<SDL_Color> {
	  static Node encode(const SDL_Color & color) {
		Node node;

		node["r"] = color.r
		node["b"] = color.g
		node["g"] = color.b
		node["a"] = color.a

		return node;
	  }

	  static bool decode(const Node & node, SDL_Color & color) {
		if(!node.IsMap() || node.size() != 4)
		  return false;

		color.r = node["r"].as<Uint8>();
		color.g = node["g"].as<Uint8>();
		color.b = node["b"].as<Uint8>();
		color.a = node["a"].as<Uint8>();

		return true;
	  }
	};
}

class Yaml {
	public:
		static Yaml & get_instance();

		Yaml(const std::string & filename);
		Yaml(const Yaml & yaml);
		~Yaml() {}

		Yaml & operator=(const Yaml &) = delete;

		const Yaml & operator[](const unsigned int idx) const;
		Yaml & operator[](const unsigned int idx);

		const Yaml & operator[](const std::string & key) const;
		Yaml & operator[](const std::string & key);

		std::vector<const std::string &> get_keys() const;

		std::vector<const Yaml> get_values() const;
		std::vector<Yaml> get_values();

		std::map<std::string, const Yaml> get_items() const;
		std::map<std::string, Yaml> get_items();

		bool check() const;

		bool get_bool() const;
		int get_int() const;
		float get_float() const;
		const std::string & get_str() const;

		const Vector2f & get_vec() const;
		const SDL_Color & get_color() const;

		void set_bool(bool val);
		void set_int(int val);
		void set_float(float val);
		void set_str(const std::string & val);

		void set_vec(const Vector2f & vec);
		void set_color(const SDL_Color & color);

		void save() const;

	private:
		Yaml(Yaml & parent, YAML::Node & base);

		std::string path;

		std::string file;

		Yaml * parent;

		YAML::Node root;
};
#endif
