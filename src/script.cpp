#include <fstream>

#include "background.h"
#include "clock.h"
#include "collision.h"
#include "engine.h"
#include "hud.h"
#include "player.h"
#include "spec.h"
#include "sprite.h"
#include "vector2f.h"
#include "viewport.h"
#include "world.h"

#include "script.h"

class VectorAPI : public Script::API {
	public:
		VectorAPI() : x(0), y(0), def(nullptr), vector(nullptr) {}
		VectorAPI(float vx, float vy) : x(vx), y(vy), def(new Vector2f(x, y)), vector(def) {}
		VectorAPI(const VectorAPI & vapi) : x(vapi.x), y(vapi.y), def(nullptr), vector(vapi.vector) {};

		virtual ~VectorAPI() {
			if (def)
				delete def;
		}

		const VectorAPI & operator=(const VectorAPI & vapi) {
			x = vapi.x;
			y = vapi.y;

			vector = vapi.vector;

			return *this;
		}

		void bind(Vector2f & vec) {
			vector = &vec;
		}

		Vector2f & get() {
			return *vector;
		}

		void load() {
			if (!vector)
				throw std::runtime_error("Uninitialized VectorAPI");

			x = (*vector)[0];
			y = (*vector)[1];
		}

		void write() {
			(*vector)[0] = x;
			(*vector)[1] = y;
		}

		float x, y;

	private:
		Vector2f * def;
		Vector2f * vector;
};

class SpriteAPI : public Script::API {
	public:
		SpriteAPI() : state(""), pos_x(0), pos_y(0), rot(0), vel_x(0), vel_y(0), scale(0), idx(0), def(nullptr), sprite(nullptr) {}
		SpriteAPI(const std::string & name) : state(""), pos_x(0), pos_y(0), rot(0), vel_x(0), vel_y(0), scale(0), idx(0), def(new Sprite(name)), sprite(def) {}
		SpriteAPI(const SpriteAPI & sapi) : state(sapi.state), pos_x(sapi.pos_x), pos_y(sapi.pos_y), rot(sapi.rot), vel_x(sapi.vel_x), vel_y(sapi.vel_y), scale(sapi.scale), idx(sapi.idx), def(nullptr), sprite(sapi.sprite) {};

		virtual ~SpriteAPI() {
			if (def)
				delete def;
		}

		const SpriteAPI & operator=(const SpriteAPI & sapi) {
			state = sapi.state;

			pos_x = sapi.pos_x;
			pos_y = sapi.pos_y;
			rot = sapi.rot;
			vel_x = sapi.vel_x;
			vel_y = sapi.vel_y;
			scale = sapi.scale;
			idx = sapi.idx;

			sprite = sapi.sprite;

			return *this;
		}

		void bind(Sprite & s) {
			sprite = &s;
		}

		Sprite & get() {
			return *sprite;
		}

		void load() {
			if (!sprite)
				throw std::runtime_error("Uninitialized SpriteAPI");

			state = sprite->get_state();

			pos_x = sprite->get_x();
			pos_y = sprite->get_y();
			rot = sprite->get_rotation();
			vel_x = sprite->get_velocity_x();
			vel_y = sprite->get_velocity_y();
			scale = sprite->get_scale();
			idx = sprite->get_index();
		}

		void write() {
			sprite->set_state(state);

			sprite->set_x(pos_x);
			sprite->set_y(pos_y);
			sprite->set_rotation(rot);
			sprite->set_velocity_x(vel_x);
			sprite->set_velocity_y(vel_y);
			sprite->set_scale(scale);
			sprite->set_index(idx);
		}

		void inject() {
			sprite->inject();
		}

		std::string state;

		float pos_x;
		float pos_y;
		float rot;
		float vel_x;
		float vel_y;
		float scale;
		int idx;

	private:
		Sprite * def;
		Sprite * sprite;
};

// TODO: load other api stuff

Script::Script(const std::string & name, Sprite & s) : path("behaviours"), script(""), lua(), apis{}, sprite(&s) {
	// load file
	load_file(path + "/" + name + ".lua");

	// prepare environment
	load_api();

	// run script
	load();
	lua.script(script);
	write();
}

Script::Script(const std::string & command) : path("behaviours"), script(command), lua(), apis{}, sprite(nullptr) {
	// prepare environment
	load_api();

	// run command
	load();
	lua.script(command);
	write();
}

Script::Script(const Script & s) : path(s.path), script(s.script), lua(), apis{}, sprite(s.sprite) {
	// prepare environment
	load_api();

	// run file
	load();
	lua.script(script);
	write();
}

Script::~Script() {
	for (API * api : apis)
		delete api;
}

void Script::load_api() {
	lua.open_libraries(sol::lib::base);

	// create Vector data type
	lua.new_userdata<VectorAPI>("Vector",
			sol::constructors<sol::types<float, float>>(),

			"x", &VectorAPI::x,
			"y", &VectorAPI::y
	);

	// create Sprite data type
	lua.new_userdata<SpriteAPI>("Sprite",
			sol::constructors<sol::types<std::string>>(),

			"inject", &SpriteAPI::inject,

			"state", &SpriteAPI::state,

			"pos_x", &SpriteAPI::pos_x,
			"pos_y", &SpriteAPI::pos_y,
			"rot", &SpriteAPI::rot,
			"vel_x", &SpriteAPI::vel_x,
			"vel_y", &SpriteAPI::vel_y,
			"scale", &SpriteAPI::scale,
			"idx", &SpriteAPI::idx
	);

	// set sprite as current sprite
	SpriteAPI * sapi = new SpriteAPI();
	sapi->bind(*sprite);

	lua["sprite"] = sapi;
	apis.push_back(sapi);
}

void Script::load_file(const std::string & filename) {
	// load file contents
	std::ifstream file(filename);
	if (!file)
		throw std::runtime_error("Failed to load script " + filename);
	script = std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
}

void Script::load() {
	for (API * api : apis)
		api->load();
}

void Script::write() {
	for (API * api : apis)
		api->write();
}
