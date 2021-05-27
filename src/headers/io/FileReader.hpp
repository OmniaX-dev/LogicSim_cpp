#ifndef __FILEREADER__HPP__
#define __FILEREADER__HPP__

#include "String.hpp"

namespace Omnia
{
	class FileReader
	{
		public:
			FileReader(void);
			FileReader(String path);
			void create(String path);

			inline String ext(void) {return m_ext;}
			inline String name(void) {return m_name;}
			inline String fullName(void) {return m_fullName;}
			inline String path(void) {return m_path;}
			inline String dir(void) {return m_dir;}

			inline String content(void) {return m_content;}
			inline bool exists(void) {return m_exists;}

		private:
			void loadContent(void);

		private:
			String m_path;
			bool m_exists;
			String m_content;
			bool m_loaded;
			String m_fullName;
			String m_ext;
			String m_name;
			String m_dir;

			static char slash;
	};
}
#endif
