#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <memory>
#include <string>
#include <map>


using SoundRef = std::shared_ptr <sf::Sound>;

class AssetManager
{
public:
	AssetManager() = default;
	~AssetManager() = default;

	sf::Texture& loadTexture(const char* name, const char* filePath);
	sf::Texture& getTexture(const char* name);
	bool loadFont(const char* name, const char* filePath);
	sf::Font* getFont(const char* name);
	bool loadSoundBuffer(const char* name, const char* filePath);
	sf::SoundBuffer& getSoundBuffer(const char* name);
	SoundRef linkSoundRef(const char* bufferName, float volume = 30.0f);
	void adjustSoundVolume(float vol);

private:
	struct BaseVolume {
		SoundRef reference;
		float volume;
	};
	std::map<std::string, BaseVolume> m_soundVolumes;
	std::map<std::string, sf::Texture> m_textures;
	std::map<std::string, sf::Font> m_fonts;
	std::map<std::string, sf::SoundBuffer> m_soundBuffers;
};