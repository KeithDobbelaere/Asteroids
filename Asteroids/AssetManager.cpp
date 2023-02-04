#include "AssetManager.h"

#include <iostream>
#include <algorithm>


sf::Texture& AssetManager::loadTexture(const char* name, const char* filePath)
{
	if (m_textures.count(name) == 0)
	{
		sf::Texture tex;
		if (tex.loadFromFile(filePath))
			m_textures[name] = tex;
		else
		{
			std::cerr << "ERROR: Unable to open file \"" << filePath << "\" for [" << name << "] texture.\n";
		}
	}
	return m_textures.at(name);
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
	SoundRef ret;
	if (m_soundVolumes.count(bufferName) == 0) {
		sf::Sound tempSound(getSoundBuffer(bufferName));
		tempSound.setVolume(volume);
		ret = std::make_shared<sf::Sound>(tempSound);
		BaseVolume tempAttribs;
		tempAttribs.reference = ret;
		tempAttribs.volume = volume;
		m_soundVolumes.insert_or_assign(bufferName, tempAttribs);
	}
	else {
		ret = m_soundVolumes.at(bufferName).reference;
	}
	return ret;
}

void AssetManager::adjustSoundVolume(float vol)
{
	for (auto& attrib : m_soundVolumes)
	{
		attrib.second.reference->setVolume(attrib.second.volume * vol);
	}
}
