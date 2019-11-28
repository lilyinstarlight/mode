#ifndef YAML_H
#define YAML_H
#include <string>
#include <vector>

#include <yaml-cpp/yaml.h>

class Yaml {
	public:
		Yaml(const std::string & filename);
		Yaml(const Yaml & yaml);
		virtual ~Yaml() {}

		Yaml & operator=(const Yaml & other);

		void load(const std::string & filename);

		std::vector<std::string> get_keys(const std::string & key = "") const;

		bool check(const std::string & key) const;

		bool get_bool(const std::string & key) const;
		int get_int(const std::string & key) const;
		float get_float(const std::string & key) const;
		std::string get_str(const std::string & key) const;

	private:
		std::string path;

	protected:
		std::string parent(const std::string & path) const;
		std::string leaf(const std::string & path) const;

		// Note: YAML::Node types are reference types that cannot be const
		//       therefore the return value of this function can mutate the
		//       document but this is not a huge concern given this is an
		//       internal API
		YAML::Node resolve(const std::string & path) const;

		std::string file;

		YAML::Node root;
};

class ModifiableYaml : public Yaml {
	public:
		ModifiableYaml(const std::string & filename);
		ModifiableYaml(const Yaml & yaml);
		virtual ~ModifiableYaml() {}

		ModifiableYaml & operator=(const ModifiableYaml & other);

		void remove(const std::string & key);

		void set_bool(const std::string & key, bool val);
		void set_int(const std::string & key, int val);
		void set_float(const std::string & key, float val);
		void set_str(const std::string & key, const std::string & val);

		void write() const;

	protected:
		// Note: See note above for why this works
		YAML::Node resolve(const std::string & path) { return Yaml::resolve(path); }
};
#endif
