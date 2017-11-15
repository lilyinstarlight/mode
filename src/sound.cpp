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

Sound::Sound() : path("audio"), active{}, chunks{} {
	// open font
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) < 0)
		throw std::string("Failed to initialize mixer");

	play("theme");
}

void Sound::play(const std::string & name, int loops) {
	std::unordered_map<std::string, Mix_Chunk *>::const_iterator pos = chunks.find(name);
	if (pos == chunks.end()) {
		Mix_Chunk * chunk = Mix_LoadWAV((path + "/" + name + ".wav").c_str());

		if (!chunk)
			throw std::string("Failed to load audio file");

		chunks[name] = chunk;
	}

	if (loops < 0)
		active = name;

	if (Mix_PlayChannel(-1, chunks[name], loops) < 0)
		throw std::string("Failed to play audio file: " + std::string(Mix_GetError()));
}
