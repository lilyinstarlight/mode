#ifndef SPEC_H
#define SPEC_H
#include <unordered_map>
#include <string>
#include <vector>

class Spec {
	public:
		static Spec & get_instance();

		~Spec() {}

		Spec(const Spec &) = delete;
		Spec & operator=(const Spec &) = delete;

		void load(const std::string & world);

		bool check(const std::string & tag) const;

		std::vector<std::string> get_tops() const;
		std::vector<std::string> get_subs(const std::string & tag) const;

		bool get_bool(const std::string & tag) const;
		int get_int(const std::string & tag) const;
		float get_float(const std::string & tag) const;
		const std::string & get_str(const std::string & tag) const;

	private:
		Spec();

		std::string path;

		std::unordered_map<std::string, std::string> data;
};
#endif
