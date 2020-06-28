#ifndef SOUND_H
#define SOUND_H
#include <stack>
#include <string>
#include <unordered_map>

#include <SDL_mixer.h>

class Sound {
	public:
		static Sound & get_instance();

		~Sound();

		Sound(const Sound &) = delete;
		const Sound & operator=(const Sound &) = delete;

		void reload();

		void play(const std::string & name, int loops = 0);
		std::string check() const { return _active; }

	private:
		Sound();

		std::string _path;

		std::string _active;
		int _active_channel;
		std::unordered_map<std::string, Mix_Chunk *> _chunks;

		int _fade;
};
#endif
