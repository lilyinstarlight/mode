#ifndef SOUND_H
#define SOUND_H
#include <stack>
#include <string>
#include <unordered_map>

#include <SDL_mixer.h>

class Sound {
	public:
		static Sound & get_instance();

		Sound(const Sound &) = delete;
		const Sound & operator=(const Sound &) = delete;

		void play(const std::string & name, int loops = -1);
		std::string check() const { return active; }
	private:
		Sound();
		~Sound();

		std::string path;

		std::string active;
		int active_channel;
		std::unordered_map<std::string, Mix_Chunk *> chunks;

		int fade;
};
#endif
