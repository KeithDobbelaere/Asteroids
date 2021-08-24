#pragma once

#include "FileManager.h"

#include <string>
#include <array>
#include <optional>


enum class Difficulty {
	Hard = 1,
	Medium,
	Easy
};

class ScoresManager : public FileManager
{
public:
	ScoresManager();


	bool load();
	void save();
	
	struct Entry
	{
		Entry() = default;
		Entry(const char* inits, int s) 
			: score(s)
		{ 
			strcpy_s(initials, STRING_SIZE, inits);
		}

		static const size_t STRING_SIZE = 4;
		char initials[STRING_SIZE];
		int score;
	};

	void addScore(const Entry& entry, Difficulty difficulty);
	const Entry& getScore(int index, int difficultyIndex) const;
	const Entry& getLowest(Difficulty difficulty) const;

	static char playerInitials[Entry::STRING_SIZE];

private:
	void generateNew();
	const std::array<const char*, 26> namePool =
	{   "ACE", "BAD", "CAT", "DOT", "END", "FAB", "GUN", "HOT", "ICE", "JET", "KID", "LOW", "MAX",
		"NIL", "ODD", "PRO", "QUE", "RAD", "SLY", "TOP", "UNO", "VOX", "WIZ", "XOR", "YOU", "ZIP"
	};
	std::array<Entry, 12 * 3> m_highScores = {};
	std::string m_thisHash;
	std::optional<std::string> m_storedHash;
};
