#ifndef __ERRORMANAGER__HPP__
#define __ERRORMANAGER__HPP__

#include <vector>
#include <iostream>
#include "DataTypes.hpp"

namespace Omnia
{
	class Error
	{
	protected:
		int m_code;
		eErrorLevel m_level;
		String m_short_msg;
		String m_long_msg;

		String __line;
		String __file;

		friend class ErrorManager;

	public:
		static Error NO_ERROR_PRESENT;
		static Error DEACTIVATING_STATE_WITHOUT_SUBSTITUTE;
		static Error FLAG_INDEX_OUT_OF_BOUNDS;
		static Error GAME_STATE_NOT_FOUND;
		static Error NULL_RENDER_TARGET;
		static Error MAX_ERRORS_REACHED;
		static Error FAILED_TO_OPEN_FILE;
		static Error FAILED_TO_READ_FROM_FILE;
		static Error FAILED_TO_ALLOCATE_MEMORY;
		static Error MAX_FONTS_REACHED;
		static Error FONT_NOT_FOUND;
		static Error FAILED_TO_LOAD_IMAGE;
		static Error ANIMATION_NOT_FOUND;
		static Error INVALID_IMAGE_PATH;
		static Error MAX_LAYERS_REACHED;
		static Error LAYER_DRAW_INDEX_OUT_OF_BOUNDS;
		static Error LAYER_NOT_FOUND;
		static Error NULL_VERTEX_ARRAY_POINTER;
		static Error INVALID_OR_NON_TEXT_LAYER;
		static Error INVALID_ENTITY_COMPONENT;
		static Error NULL_ANIMATION_POINTER;
		static Error INVALID_LAYER;
		static Error EMPTY_STRING;
		static Error INVALID_DATABLOCK_TYPE;
		static Error INVALID_ARRAY_SIZE;
		static Error INVALID_DATAFILE_PATH;
		static Error INVALID_DATAFILE_BLOCK;
		static Error INVALID_DATABLOCK;
		static Error LOCAL_DATABLOCK_NOT_FOUND;
		static Error GLOBAL_DATABLOCK_NOT_FOUND;
		static Error INVALID_DATABLOCK_DEFINE_VALUE;
		static Error DATABLOCK_PARAM_ASSING_MISSING;
		static Error UNMATCHED_QUOTE_IN_PARAM_VALUE;
		static Error UNMATCHED_PARENTHESIS_IN_PARAM_VALUE;
		static Error INVALID_PARAM_ARRAY_SIZE;
		static Error ANIMATION_NOT_FOUND_IN_LIST;
		static Error SPRITE_NOT_FOUND_IN_LIST;
		static Error BITMAP_FONT_NOT_FOUND;
		static Error UNKNOWN_CONTROL_TYPE;
		static Error INVALID_MAP_FILE_PATH;
		static Error MAP_NOT_FOUND_IN_LIST;
		static Error TILESET_NOT_LOADED;
		static Error TILESET_NOT_FOUND;


	public:
		inline int code(void) { return m_code; }
		inline String short_msg(void) { return m_short_msg; }
		inline String long_msg(void) { return m_long_msg; }
		inline eErrorLevel level(void) { return m_level; }
		inline bool isNoError(void) { return this->level() == eErrorLevel::NoError; }
		inline bool isInfo(void) { return this->level() == eErrorLevel::Info; }
		inline bool isSuggestion(void) { return this->level() == eErrorLevel::Suggestion; }
		inline bool isWarning(void) { return this->level() == eErrorLevel::Warning; }
		inline bool isError(void) { return this->level() == eErrorLevel::Error; }
		inline bool isCritical(void) { return this->level() == eErrorLevel::Critical; }
		inline Error(void) : m_code(0), m_level(eErrorLevel::NoError), m_short_msg(""), m_long_msg(""), __line(""), __file("") {}
		inline Error(int code, eErrorLevel level, String short_msg, String long_msg) : m_code(code), m_level(level), m_short_msg(short_msg), m_long_msg(long_msg), __line(""), __file("") {}

		void print(void) ;
		String parseLevel(void) ;
	};

	class CustomError : public Error
	{
		public:
			CustomError(eErrorLevel level, String title, String description);
		private:
			inline static int CURRENT_CUSTOM_ERROR_CODE_BASE = 5000;
	};

	class ErrorManager
	{
		private:
			Error* m_queue[AUTO__MAX_ERRORS];
			unsigned int m_count;

		public:
			ErrorManager(void);
			void push(Error& error, String __line = "", String __file = "");
			Error& pop(void);
			std::vector<Error> dump(void);
			inline int count(void) { return m_count; }
	};
}
#endif
