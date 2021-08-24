#include "FileManager.h"

#include <fstream>
#include "Defines.h"

#include <iostream>
#include <vector>


std::string FileManager::m_filePath{};
std::fstream FileManager::m_file{};

FileManager::FileManager()
{
	if (m_filePath.empty()) {
		char* buf = nullptr;
		size_t sz = 0;
		if (_dupenv_s(&buf, &sz, "appdata") == 0 && buf != nullptr)
		{
			m_filePath = buf;
			free(buf);
		}
		else
			std::cerr << "ERROR: Problem accessing environment variable %appdata%.\n";
		m_filePath += APP_DATA_PATH;
	if (!open())
		generateNew();
	}
}

FileManager::~FileManager()
{
	if (m_file.is_open()) close();
}


void FileManager::generateNew()
{
	if (m_file.is_open()) {
		std::vector<char> empty(APP_DATA_FILE_SIZE, 0);
		m_file.write(empty.data(), empty.size());
	}
}

bool FileManager::open()
{
	m_file.open(m_filePath.c_str(), std::ios::in | std::ios::out | std::ios::binary | std::ios::ate);
	if (m_file.is_open())
	{
		std::streamsize fileSize = m_file.tellg();
		if (fileSize == APP_DATA_FILE_SIZE)
			return true;
		else {
			std::cout << "File '" << m_filePath << "' has been corrupted.\n";
		}
	}
	else
	{
		std::cout << "File '" << m_filePath << "' does not exist.\n";
		m_file.open(m_filePath.c_str(), std::ios::in | std::ios::out | std::ios::binary | std::ios::trunc);
		if (!m_file.is_open())
		{
			std::string errorMsg = "ERROR: Could not create file '" + m_filePath +
				"' - Game data will not be saved";
			perror(errorMsg.c_str());
		}
	}
	return false;
}

void FileManager::close()
{
	m_file.close();
}
