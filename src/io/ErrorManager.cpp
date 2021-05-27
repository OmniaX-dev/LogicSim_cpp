#include "ErrorManager.hpp"
#include "Flags.hpp"
#include "console.hpp"

namespace Omnia
{
	Error Error::NO_ERROR_PRESENT = Error(
		0,
		eErrorLevel::NoError,
		"No error.",
		"No error occurred."
	);
	Error Error::DEACTIVATING_STATE_WITHOUT_SUBSTITUTE = Error(
		1,
		eErrorLevel::Warning,
		"Trying to deactivate a state without a substitute.",
		"A GameState object can't be set to an active state, unless a valid (active) GameState is given as a substitute."
	);
	Error Error::FLAG_INDEX_OUT_OF_BOUNDS = Error(
		2,
		eErrorLevel::Error,
		"Trying to access a nonexistent flag.",
		"The flag you access must have an index in the range from 0 to __MAX_FLAGS."
	);
	Error Error::GAME_STATE_NOT_FOUND = Error(
		3,
		eErrorLevel::Warning,
		"Trying to set a nonexistent GameState as active.",
		"The GameState you set as active, must be found inside of Runnable::m_gameStates."
	);
	Error Error::NULL_RENDER_TARGET = Error(
		4,
		eErrorLevel::Critical,
		"RenderTarget can't be nullptr.",
		"The m_renderTarget member of the Renderer class cannot be set to nullptr; it must be set either to the main window or to a second RenderTarget object."
	);
	Error Error::MAX_ERRORS_REACHED = Error(
		5,
		eErrorLevel::Critical,
		"Maximum number of errors reached.",
		"The maximum number of queued errors reached, a call to ErrorManager::dump() is needed."
	);
	Error Error::FAILED_TO_OPEN_FILE = Error(
		6,
		eErrorLevel::Critical,
		"Failed to open file.",
		"Failed to open the specified file. Maybe the path is incorrect?"
	);
	Error Error::FAILED_TO_READ_FROM_FILE = Error(
		7,
		eErrorLevel::Critical,
		"Failed to read from file.",
		"Failed to read from the specified file. Maybe the file is corrupted?"
	);
	Error Error::FAILED_TO_ALLOCATE_MEMORY = Error(
		8,
		eErrorLevel::Critical,
		"Failed to allocate memory for file.",
		"Failed to allocate memory for the specified file."
	);
	Error Error::MAX_FONTS_REACHED = Error(
		9,
		eErrorLevel::Warning,
		"Maximum number of fonts reached.",
		"The Maximum numebr of fonts in the Fonts class has been reached."
	);
	Error Error::FONT_NOT_FOUND = Error(
		10,
		eErrorLevel::Error,
		"Font not found.",
		"The found could not be located in the list of fonts inside the Fonts class."
	);
	Error Error::FAILED_TO_LOAD_IMAGE = Error(
		11,
		eErrorLevel::Error,
		"Failed to load image.",
		"Failed to load the image file in Image class. Maybe the path is incorrect?"
	);
	Error Error::ANIMATION_NOT_FOUND = Error(
		13,
		eErrorLevel::Error,
		"Animation not found.",
		"An animation could not be found with the specified ID."
	);
	Error Error::INVALID_IMAGE_PATH = Error(
		14,
		eErrorLevel::Error,
		"Image not found.",
		"The Image path specified for the BitmapFont is not valid."
	);
	Error Error::MAX_LAYERS_REACHED = Error(
		15,
		eErrorLevel::Warning,
		"Maximum number of Layers reached.",
		"The maximum number of Layers that a LayerRenderer can store, has been reached."
	);
	Error Error::LAYER_DRAW_INDEX_OUT_OF_BOUNDS = Error(
		16,
		eErrorLevel::Error,
		"Layer draw_index out of bounds.",
		"The draw_index specified for the Layer you are trying to add must be between 0 and the current numebr of layers."
	);
	Error Error::LAYER_NOT_FOUND = Error(
		17,
		eErrorLevel::Error,
		"Layer not found.",
		"The specified ID doesn't match with any of the layers present."
	);
	Error Error::NULL_VERTEX_ARRAY_POINTER = Error(
		18,
		eErrorLevel::Critical,
		"VertexArray pointer is nullptr.",
		"The Layer object must have a valid VertexArray pointer before it can be retrieved."
	);
	Error Error::INVALID_OR_NON_TEXT_LAYER = Error(
		19,
		eErrorLevel::Warning,
		"Invalid or non-text Layer.",
		"The Layer used to call the makeString(...) method must be valid and set as text layer."
	);
	Error Error::INVALID_ENTITY_COMPONENT = Error(
		20,
		eErrorLevel::Warning,
		"Invalid EntityComponent instance.",
		"The EntityComponent instance you are trying to access is invalid."
	);
	Error Error::NULL_ANIMATION_POINTER = Error(
		21,
		eErrorLevel::Warning,
		"Animation pointer is nullptr.",
		"The Animation pointer of the EntityComponent instance you are accessing is nullptr."
	);
	Error Error::INVALID_LAYER = Error(
		22,
		eErrorLevel::Error,
		"Invalid Layer.",
		"The Layer you are trying to draw to is invalid."
	);
	Error Error::EMPTY_STRING = Error(
		23,
		eErrorLevel::Warning,
		"The String is empty.",
		"The String you passed is empty or white-space only."
	);
	Error Error::INVALID_DATABLOCK_TYPE = Error(
		24,
		eErrorLevel::Error,
		"Invalid DataBlock type.",
		"The DataBlock evaluated type is incorrect."
	);
	Error Error::INVALID_ARRAY_SIZE = Error(
		25,
		eErrorLevel::Error,
		"Invalid array size.",
		"The size of the array specifying a color in a DataBlock must be equal to 4 (1 for each component of the color)."
	);
	Error Error::INVALID_DATAFILE_PATH = Error(
		26,
		eErrorLevel::Error,
		"Invalid DataFile path.",
		"The path you are trying to load a DataFile from is invalid."
	);
	Error Error::INVALID_DATAFILE_BLOCK = Error(
		27,
		eErrorLevel::Error,
		"Invalid DataFile block.",
		"A block inside the DataFile you are loading is Invalid. All Blocks inside a DatFile must be enclosed between [ and ]."
	);
	Error Error::INVALID_DATABLOCK = Error(
		28,
		eErrorLevel::Error,
		"Invalid DataBlock.",
		"One of the parameters in one of the blocks inside of the DataFile you are loading is invalid."
	);
	Error Error::LOCAL_DATABLOCK_NOT_FOUND = Error(
		29,
		eErrorLevel::Error,
		"Local DataBlock not found.",
		"The requested local DataBlock cannot be found in the list. Maybe it's not defined?"
	);
	Error Error::GLOBAL_DATABLOCK_NOT_FOUND = Error(
		30,
		eErrorLevel::Error,
		"Global DataBlock not found.",
		"The requested global DataBlock cannot be found in the list. Maybe it's not defined?"
	);
	Error Error::INVALID_DATABLOCK_DEFINE_VALUE = Error(
		31,
		eErrorLevel::Error,
		"Invalid DataBlock define value.",
		"A define statement in a DataFile has an unknown type. Allowed types are: Integer, String (surrounded by quotes) and Literals (with the literal operator '%' before the value)."
	);
	Error Error::DATABLOCK_PARAM_ASSING_MISSING = Error(
		32,
		eErrorLevel::Error,
		"Invalid assignment in DataBlock.",
		"A parameter of a Datablock in a DataFile is missing an assignment value."
	);
	Error Error::UNMATCHED_QUOTE_IN_PARAM_VALUE = Error(
		33,
		eErrorLevel::Error,
		"Unmatched quote in parameter value.",
		"A parameter of a DataBlock in a DataFile has an unmatched double quote."
	);
	Error Error::UNMATCHED_PARENTHESIS_IN_PARAM_VALUE = Error(
		34,
		eErrorLevel::Error,
		"Unmatched parenthesis in parameter value.",
		"A parameter of a DataBlock in a DataFile has an unmatched parenthesis."
	);
	Error Error::INVALID_PARAM_ARRAY_SIZE = Error(
		35,
		eErrorLevel::Error,
		"Invalid parameter array size.",
		"An array in a DataBlock in a DataFile has an incorrect amount of items."
	);
	Error Error::ANIMATION_NOT_FOUND_IN_LIST = Error(
		36,
		eErrorLevel::Error,
		"Animation not found.",
		"The requested animation could not be found in the list of Animations."
	);
	Error Error::SPRITE_NOT_FOUND_IN_LIST = Error(
		37,
		eErrorLevel::Error,
		"Sprite not found.",
		"The requested sprite could not be found in the list of sprites."
	);
	Error Error::BITMAP_FONT_NOT_FOUND = Error(
		38,
		eErrorLevel::Error,
		"Bitmap font not found.",
		"The requested Bitmap font could not be found in the list of fonts."
	);
	Error Error::UNKNOWN_CONTROL_TYPE = Error(
		39,
		eErrorLevel::Warning,
		"Unknown control type.",
		"The control you are trying to check events for, has an unknown type."
	);
	Error Error::INVALID_MAP_FILE_PATH = Error(
		40,
		eErrorLevel::Error,
		"Invalid Map file path.",
		"The path specified for loading a Map is invalid."
	);
	Error Error::MAP_NOT_FOUND_IN_LIST = Error(
		41,
		eErrorLevel::Error,
		"Map not found.",
		"The requested map could not be found in the list of maps."
	);
	Error Error::TILESET_NOT_LOADED = Error(
		42,
		eErrorLevel::Warning,
		"Tileset not loaded.",
		"The Tileset you are trying to load in a WorldFile could not be found."
	);
	Error Error::TILESET_NOT_FOUND = Error(
		43,
		eErrorLevel::Error,
		"Tileset not found.",
		"The requested tileset could not be found in the library."
	);

	void Error::print(void)
	{
		fg_color(FG_COL_RED);
		std::cout << "\n\n================================ERROR================================\n\n";
		fg_color(FG_COL_DARK_GRAY);
		std::cout << "Level:  ";
		std::string lvl = parseLevel().cpp();
		std::cout << lvl;
		fg_color(FG_COL_DARK_GRAY);
		std::cout << "  Code:  ";
		fg_color(FG_COL_LIGHT_MAGENTA);
		std::cout << (int)code() << "\n";
		fg_color(FG_COL_DARK_GRAY);
		std::cout << "Source: ";

		fg_color(FG_COL_BROWN);
		std::cout << __file.cpp();
		fg_color(FG_COL_DARK_GRAY);
		std::cout << ":";
		fg_color(FG_COL_GREEN);
		std::cout << __line.cpp() << "\n\n";
		fg_color(FG_COL_LIGHT_RED);
		if (Flags::isunset(FLG_NO_MESSAGE_ON_ERROR_DUMP))
			std::cout << short_msg().cpp() << "\n";
		fg_color(FG_COL_RED);
		if (Flags::isunset(FLG_NO_MESSAGE_ON_ERROR_DUMP) && Flags::isset(FLG_LONG_MESSAGE_ON_ERROR_DUMP))
		{
			std::cout << "\n===============================MESSAGE===============================\n";
			fg_color(FG_COL_LIGHT_RED);
			std::cout << long_msg().cpp() << "\n";
		}
		fg_color(FG_COL_RED);
		std::cout << "=====================================================================\n\n";
		fg_color(FG_COL_LIGHT_GREEN);
	}

	String Error::parseLevel(void)
	{
		switch (level())
		{
			case eErrorLevel::NoError:
				fg_color(FG_COL_LIGHT_GREEN);
				return "NO_ERROR";
			case eErrorLevel::Info:
				fg_color(FG_COL_BLUE);
				return "INFO";
			case eErrorLevel::Suggestion:
				fg_color(FG_COL_GREEN);
				return "SUGGESTION";
			case eErrorLevel::Warning:
				fg_color(FG_COL_YELLOW);
				return "WARNING";
			case eErrorLevel::Error:
				fg_color(FG_COL_RED);
				return "ERROR";
			case eErrorLevel::Critical:
				fg_color(FG_COL_LIGHT_RED);
				return "CRITICAL";
			default:
				fg_color(FG_COL_WHITE);
				return "UNKNOWN_LEVEL";
		}
	}

	CustomError::CustomError(eErrorLevel level, String title, String description)
	{
		m_code = CustomError::CURRENT_CUSTOM_ERROR_CODE_BASE++;
		m_level = level;
		m_short_msg = title;
		m_long_msg = description;
	}

	ErrorManager::ErrorManager(void)
	{
		m_count = 0;
		for (unsigned int i = 0; i < AUTO__MAX_ERRORS; i++)
			m_queue[i] = nullptr;
	}

	void ErrorManager::push(Error& error, String __line, String __file)
	{
		error.__line = __line;
		error.__file = __file;
		if (Flags::isset(FLG_AUTO_DUMP_EVERY_ERROR))
		{
			error.print();
			return;
		}
		if (m_count < AUTO__MAX_ERRORS)
			m_queue[m_count++] = &error;
		else if (Flags::isset(FLG_AUTO_DUMP_ERROR_QUEUE_WHEN_FULL))
		{
			for (unsigned int i = m_count - 1; i >= 0; i--)
			{
				Error& err = *m_queue[i];
				err.print();
				m_queue[i] = nullptr;
			}
			m_count = 0;
		}
		else
			m_queue[m_count - 1] = &Error::MAX_ERRORS_REACHED;
	}

	Error& ErrorManager::pop(void)
	{
		if (m_count <= 0)
			return Error::NO_ERROR_PRESENT;
		Error& error = *m_queue[0];
		for (unsigned int i = 1; i < m_count; i++)
			m_queue[i - 1] = m_queue[i];
		m_count--;
		return error;
	}

	std::vector<Error> ErrorManager::dump(void)
	{
		std::vector<Error> list;
		for (unsigned int i = 0; i < m_count; i++)
			list.push_back(*m_queue[i]);
		m_count = 0;
		return list;
	}
}
