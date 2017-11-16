#include "spec.h"

#include "sound.h"

Sound & Sound::get_instance() {
	static Sound sound;
	return sound;
}

Sound::~Sound() {
	for (std::pair<std::string, Mix_Chunk *> chunk : chunks)
		Mix_FreeChunk(chunk.second);

	Mix_CloseAudio();
}

Sound::Sound() : path("sounds"), active{}, chunks{} {
	// open font
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) < 0)
		throw std::runtime_error("Failed to initialize mixer");
}

void Sound::play(const std::string & name, int loops) {
	std::unordered_map<std::string, Mix_Chunk *>::const_iterator pos = chunks.find(name);
	if (pos == chunks.end()) {
		Mix_Chunk * chunk = Mix_LoadWAV((path + "/" + name + ".wav").c_str());

		if (!chunk)
			throw std::runtime_error("Failed to load audio file " + path + "/" + name + ".wav");

		chunks[name] = chunk;
	}

	if (loops < 0)
		active = name;

	if (Mix_PlayChannel(-1, chunks[name], loops) < 0)
		throw std::runtime_error("Failed to play audio file " + path + "/" + name + ".wav: " + std::string(Mix_GetError()));
}
