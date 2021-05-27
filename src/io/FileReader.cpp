#include "FileReader.hpp"
#include <stdio.h>
#include <stdlib.h>
#include "GameData.hpp"

namespace Omnia
{
#ifdef WINDOWS_OS
	char FileReader::slash = '\\';
#else
	char FileReader::slash = '/';
#endif

	FileReader::FileReader(void)
	{
		m_exists = false;
		m_loaded = false;
	}

	FileReader::FileReader(String path)
	{
		create(path);
	}

	void FileReader::create(String path)
	{
		m_path = path;
		loadContent();
	}

	void FileReader::loadContent(void)
	{
		m_exists = false;
		char* buffer = nullptr;
		long length;
		FILE* f = fopen(m_path.c_str(), "rb");

		if (f)
		{
			fseek(f, 0, SEEK_END);
			length = ftell(f);
			fseek(f, 0, SEEK_SET);
			buffer = (char*)malloc(length + 1);
			if (buffer)
			{
				if (!fread(buffer, 1, length, f))
					GameData::instance().errorQueue.push(Error::FAILED_TO_READ_FROM_FILE, ERROR_DATA());
				else
					m_exists = true;
			}
			else
				GameData::instance().errorQueue.push(Error::FAILED_TO_ALLOCATE_MEMORY, ERROR_DATA());
		}
		else
		{
			GameData::instance().errorQueue.push(Error::FAILED_TO_OPEN_FILE, ERROR_DATA());
			std::cout << m_path.cpp() << "\n";
		}
		fclose(f);

		if (!m_exists)
			return;

		if (buffer)
		{
			buffer[length] = '\0';
			m_content = buffer;
			m_loaded = true;
			m_exists = true;
			if (m_path.contains(FileReader::slash))
				m_fullName = m_path.substr(m_path.lastIndexOf(slash) + 1).trim();
			else
				m_fullName = m_path;
			m_name = m_fullName.substr(0, m_fullName.lastIndexOf('.')).trim();
			m_ext = m_fullName.substr(m_fullName.lastIndexOf('.') + 1).trim();
			if (m_path.contains(FileReader::slash))
				m_dir = m_path.substr(0, m_path.lastIndexOf(slash) + 1).trim();
			else
				m_dir = String(".").add(FileReader::slash);
			m_path = m_path.trim();
		}
	}
}
