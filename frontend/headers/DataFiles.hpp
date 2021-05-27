#ifndef __LS_DATA_FILES_HPP__
#define __LS_DATA_FILES_HPP__

#include "DataFile.hpp"
#include "LSComponents.hpp"

namespace ls
{
	class LSComponentSheetFile : public DataFile
	{
		public:
			inline LSComponentSheetFile(void) : m_logicSHeet(nullptr) { invalidate(); }
			inline LSComponentSheetFile(String path, GameData& gdata, Application& app, AnimationSheetFile& logicSheet) { load(path, gdata, app); m_logicSHeet = &logicSheet; }
			void setup(std::vector<LSChip>& cat);
			static bool comparePins(LSPin p1, LSPin p2);
			virtual void onCustomInstruction(String instr, DFD_LIST data, int paramCount);
			virtual void onCustomAction(String action, DFD_LIST params);

		private:
			LSChip m_chip;
			AnimationSheetFile* m_logicSHeet;
	};

	class LSCLibrary : public DataFile
	{
		public:
			class LSCCategory : public Object
			{

				public:
					std::vector<LSChip> chips;
					String name;
					String description;

					friend class LSCLibrary;
			};

		public:
			inline LSCLibrary(void) { invalidate(); }
			inline LSCLibrary(String path, GameData& gdata, Application& app) { load(path, gdata, app); }
			virtual void onCustomInstruction(String instr, DFD_LIST data, int paramCount);
			virtual void onCustomAction(String action, DFD_LIST params);
			sh_ptr(LSChip) newChip(String name);
			inline ComponentSheetFile& getCSF(void) { return m_csf; }

		private:
			std::vector<LSCCategory> m_categories;
			ComponentSheetFile m_csf;
	};
}


#endif