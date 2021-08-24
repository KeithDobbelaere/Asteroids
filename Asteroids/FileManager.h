#pragma once

#include <fstream>
#include <iostream>
#include <array>
#include <string>


class FileManager {
public:
	FileManager();
	~FileManager();

	template<typename T, int Size>
	void read(int offset, std::array<T, Size>& data);
	template<typename T, int Size>
	void write(int offset, const std::array<T, Size>& data);

	const char* getFilePath() { return m_filePath.c_str(); }

private:
	inline bool open();
	inline void close();
	inline void generateNew();

	static std::string m_filePath;
	static std::fstream m_file;

};

template<typename T, int Size>
inline void FileManager::read(int offset, std::array<T, Size>& data)
{
	if (m_file.is_open()) {
		m_file.seekg(offset, std::ios::beg);
		m_file.read((char*)data.data(), sizeof(T) * Size);
	}
}

template<typename T, int Size>
inline void FileManager::write(int offset, const std::array<T, Size>& data)
{
	if (m_file.is_open()) {
		m_file.seekg(offset, std::ios::beg);
		m_file.write((char*)data.data(), sizeof(T) * Size);
		m_file.flush();
	}
}
