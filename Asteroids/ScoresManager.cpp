#include "ScoresManager.h"

#include <SHA256/sha256.h>

#include <iostream>
#include <fstream>
#include <algorithm>
#include <cctype>
#include <filesystem>

#include "Registry.h"
#include "Defines.h"


ScoresManager::ScoresManager()
{
	char* buf = nullptr;
	size_t sz = 0;
	if (_dupenv_s(&buf, &sz, "appdata") == 0 && buf != nullptr)
	{
		m_appDataPath = buf;
		free(buf);
	}
	m_appDataPath += APP_DATA_PATH;
	if (!load(m_appDataPath))
	{
		generateNew();
		save(m_appDataPath);
	}
}

bool ScoresManager::load(const std::string& fileName)
{
	m_storedHash = Registry::GetStrVal(HKEY_CURRENT_USER, REGISTRY_KEY, "HighScoresHash");
	if (!m_storedHash)
	{
		std::cout << "Generating High-Score Files...\n";
		return false;
	}
	bool success;
	Entry e[12 * 3];
	std::ifstream file(fileName.c_str(), std::ios::in | std::ios::binary | std::ios::ate);
	if (file.is_open())
	{
		std::streamsize fileSize = file.tellg();
		std::streamsize dataSize = sizeof(Entry) * 12 * 3;
		if (fileSize == dataSize)
		{
			file.seekg(0, std::ios::beg);
			file.read((char*)e, dataSize);
			m_thisHash = sha256((const unsigned char*)e, sizeof(Entry) * 12U * 3U);
			if (m_storedHash == m_thisHash)
			{
				std::copy(std::begin(e), std::end(e), m_highScores.begin());
				success = true;
			}
			else
			{
				std::cerr << "ERROR: File '" << fileName << "' has been corrupted.  Regenerating files...\n";
				success = false;
			}
		}
		else
		{
			std::cout << "Problem reading '" << fileName << ".'  Regenerating files...\n";
			success = false;
		}
		file.close();
	}
	else
	{
		std::string errorMsg = "ERROR: Could not open file '" + fileName + "' for reading";
		perror(errorMsg.c_str());
		std::cout << "Building files...\n";
		success = false;
	}
	return success;
}

void ScoresManager::save(const std::string& fileName)
{
	Entry* e = m_highScores.data();
	m_thisHash = sha256((const unsigned char*)e, sizeof(Entry) * 12U * 3U);
	HKEY hKey = Registry::AccessKey(HKEY_CURRENT_USER, REGISTRY_KEY);
	Registry::SetStrVal(hKey, "HighScoresHash", m_thisHash);
	RegCloseKey(hKey);

	std::ofstream file(fileName.c_str(), std::ios::out | std::ios::binary);
	if (file.is_open())
	{
		file.write((char*)e, sizeof(Entry) * 12 * 3);
		file.close();
	}
	else
	{
		std::string errorMsg = "ERROR: Could not open/create file '" + fileName + "' for writing";
		perror(errorMsg.c_str());
	}
}

void ScoresManager::addScore(const Entry& entry, int difficulty)
{
	difficulty -= 1;
	if (entry.score > m_highScores[difficulty * 12 + 11].score)
	{
		m_highScores[difficulty * 12 + 11] = entry;
		auto begin = m_highScores.begin() + difficulty * 12;
		auto end = begin + 12;
		std::stable_sort(begin, end,
			[](const Entry& a, const Entry& b) { 
				return a.score > b.score; 
			}
		);
		save(m_appDataPath);
	}
}

const ScoresManager::Entry& ScoresManager::getScore(int index, int difficulty) const
{
	return m_highScores.at((difficulty - 1) * 12 + index);
}

const ScoresManager::Entry & ScoresManager::getLowest(int difficulty) const
{
	return m_highScores[(difficulty - 1) * 12 + 11];
}

void ScoresManager::generateNew()
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 12; j++)
		{
			m_highScores[i * 12 + j] = Entry(namePool[rand() % 26], 31000 - j * 1000);
		}
	}
}
