#pragma once

#include <string>
#include <array>
#include <optional>

class ScoresManager
{
public:
	ScoresManager();


	bool load(const std::string& fileName);
	void save(const std::string& fileName);
	
	struct Entry
	{
		Entry() = default;
		Entry(const char* inits, int s) : score(s)
		{ strcpy_s(initials, 4, inits); }
		char initials[4];
		int score;
	};

	void addScore(const Entry& entry, int difficulty);
	const Entry& getScore(int index, int difficulty) const;
	const Entry& getLowest(int difficulty) const;

	const std::array<const char*, 26> namePool =
	{   "ACE", "BAD", "CAT", "DOT", "END", "FAB", "GUN", "HOT", "ICE", "JET", "KID", "LOW", "MAX",
		"NIL", "ODD", "PRO", "QUE", "RAD", "SLY", "TOP", "UNO", "VOX", "WIZ", "XOR", "YOU", "ZIP"
	};

private:
	void generateNew();

private:
	std::string m_appDataPath;
	std::array<Entry, 12 * 3> m_highScores = {};
	std::string m_thisHash;
	std::optional<std::string> m_storedHash;
};
