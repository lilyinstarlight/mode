#include <stdexcept>

#include "spec.h"

#include "sound.h"

Sound & Sound::get_instance() {
	static Sound sound;
	return sound;
}

Sound::~Sound() {
	for (std::pair<std::string, Mix_Chunk *> chunk : _chunks)
		Mix_FreeChunk(chunk.second);

	Mix_CloseAudio();
}

Sound::Sound() : _path("sounds"), _active(), _active_channel(-1), _chunks{}, _fade(1000) {
	// open mixer
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) < 0)
		throw std::runtime_error("Failed to initialize mixer");
}

void Sound::reload() {
	if (Spec::get_instance().check("sound/initial"))
		play("sound/initial", -1);
}

void Sound::play(const std::string & name, int loops) {
	std::unordered_map<std::string, Mix_Chunk *>::const_iterator pos = _chunks.find(name);
	if (pos == _chunks.end()) {
		std::string filename = _path + "/" + Spec::get_instance().get_str(name + "/file");

		Mix_Chunk * chunk = Mix_LoadWAV(filename.c_str());

		if (!chunk)
			throw std::runtime_error("Failed to load audio file " + filename);

		_chunks[name] = chunk;
	}

	if (loops < 0) {
		_active = name;

		if (_active_channel < 0) {
			_active_channel = Mix_PlayChannel(-1, _chunks[name], loops);
		}
		else {
			Mix_FadeOutChannel(_active_channel, _fade);

			_active_channel = Mix_FadeInChannel(-1, _chunks[name], loops, _fade);
		}

		if (_active_channel < 0)
			throw std::runtime_error("Failed to play audio " + name + ": " + std::string(Mix_GetError()));
	}
	else {
		if (Mix_PlayChannel(-1, _chunks[name], loops) < 0)
			throw std::runtime_error("Failed to play audio " + name + ": " + std::string(Mix_GetError()));
	}

}
