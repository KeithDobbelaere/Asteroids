#include "ScoresManager.h"

#include <SHA256/sha256.h>

#include "Registry.h"
#include "Defines.h"

#include <iostream>
#include <fstream>
#include <algorithm>
#include <cctype>
#include <filesystem>


char ScoresManager::playerInitials[Entry::STRING_SIZE]{};

ScoresManager::ScoresManager()
{
	if (!load()) {
		generateNew();
		save();
	}
}

bool ScoresManager::load()
{
	m_storedHash = Registry::GetStrVal(HKEY_CURRENT_USER, REGISTRY_KEY, "HighScoresHash");
	if (!m_storedHash)
	{
		return false;
	}
	std::array<Entry, 12 * 3> temp;
	read(0, temp);
	m_thisHash = sha256((const unsigned char*)temp.data(), sizeof(Entry) * 12U * 3U);
	if (m_storedHash == m_thisHash)
	{
		m_highScores = temp;
		return true;
	}
	else
	{
		std::cerr << "ERROR: App data file has been corrupted.\n";
		return false;
	}
}

void ScoresManager::save()
{
	Entry* e = m_highScores.data();
	m_thisHash = sha256((const unsigned char*)e, sizeof(Entry) * 12U * 3U);
	HKEY hKey = Registry::AccessKey(HKEY_CURRENT_USER, REGISTRY_KEY);
	Registry::SetStrVal(hKey, "HighScoresHash", m_thisHash);
	RegCloseKey(hKey);

	write(0, m_highScores);
}

void ScoresManager::addScore(const Entry& entry, Difficulty difficulty)
{
	int index = (static_cast<int>(difficulty) - 1) * 12;
	if (entry.score > m_highScores[index + 11].score)
	{
		m_highScores[index + 11] = entry;
		auto begin = m_highScores.begin() + index;
		auto end = begin + 12;
		std::stable_sort(begin, end,
			[](const Entry& a, const Entry& b) { 
				return a.score > b.score; 
			}
		);
		save();
	}
}

const ScoresManager::Entry& ScoresManager::getScore(int index, int difficultyIndex) const
{
	int first = difficultyIndex * 12;
	return m_highScores.at(first + index);
}

const ScoresManager::Entry& ScoresManager::getLowest(Difficulty difficulty) const
{
	int first = (static_cast<int>(difficulty) - 1) * 12;
	return m_highScores[first + 11];
}

void ScoresManager::generateNew()
{
	std::cout << "Generating High-Score file...\n";

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 12; j++)
		{
			m_highScores[i * 12 + j] = Entry(namePool[rand() % 26], 31000 - j * 1000);
		}
	}
}
