#include "AssetManager.h"

#include <iostream>

bool AssetManager::loadTexture(const char* name, const char* filePath)
{
	if (m_textures.count(name) == 0)
	{
		sf::Texture tex;
		if (tex.loadFromFile(filePath))
		{
			m_textures[name] = tex;
			return true;
		}
		std::cerr << "ERROR: Unable to open file \"" << filePath << "\" for [" << name << "] texture.\n";
		return false;
	}
	return true;
}

sf::Texture& AssetManager::getTexture(const char* name)
{
	return m_textures.at(name);
}

bool AssetManager::loadFont(const char* name, const char* filePath)
{
	if (m_fonts.count(name) == 0)
	{
		sf::Font font;
		if (font.loadFromFile(filePath))
		{
			m_fonts[name] = font;
			return true;
		}
		std::cerr << "ERROR: Unable to open file \"" << filePath << "\" for [" << name << "] font.\n";
		return false;
	}
	return true;
}

sf::Font* AssetManager::getFont(const char* name)
{
	return &m_fonts.at(name);
}

bool AssetManager::loadSoundBuffer(const char * name, const char * filePath)
{
	if (m_soundBuffers.count(name) == 0)
	{
		sf::SoundBuffer sound;
		if (sound.loadFromFile(filePath))
		{
			m_soundBuffers[name] = sound;
			return true;
		}
		std::cerr << "ERROR: Unable to open file \"" << filePath << "\" for [" << name << "] sound.\n";
		return false;
	}
	return true;
}

sf::SoundBuffer & AssetManager::getSoundBuffer(const char * name)
{
	return m_soundBuffers.at(name);
}

SoundRef AssetManager::linkSoundRef(const char * bufferName, float volume)
{
	if (m_soundBuffers.count(bufferName) > 0)
	{
		sf::Sound tempSound(getSoundBuffer(bufferName));
		tempSound.setVolume(volume);
		SoundRef newSound = std::make_shared<sf::Sound>(tempSound);
		m_soundRefs.emplace_back(newSound, volume);
		return newSound;
	}
	return SoundRef();
}

void AssetManager::adjustSoundVolume(float vol)
{
	for (auto& sound : m_soundRefs)
	{
		sound.first->setVolume(sound.second * vol);
	}
}
