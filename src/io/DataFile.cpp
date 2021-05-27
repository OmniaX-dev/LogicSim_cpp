#include "DataFile.hpp"
#include "StringTokens.hpp"
#include "FileReader.hpp"
#include "GameData.hpp"
#include "Flags.hpp"
#include "Application.hpp"

namespace Omnia
{
	void DataFile::load(String filePath, GameData& gdata, Application& app)
	{
		m_app = &app;
		if (filePath.trim() == "")
		{
			gdata.errorQueue.push(Error::INVALID_DATAFILE_PATH, ERROR_DATA());
			invalidate();
			return;
		}
		setValid(true);
		m_dataFilePath = filePath;
		m_gdata = &gdata;
		FileReader file(filePath);
		m_rawContent = file.content();
		loadDefines(m_rawContent);
		String newData = replaceDefines(m_rawContent);
		StringTokens st = newData.tokenize("]");
		String token = "";
		while (st.hasNext()) //Priority round
		{
			token = st.next();
			if (!token.startsWith("["))
			{
				gdata.errorQueue.push(Error::INVALID_DATAFILE_BLOCK, ERROR_DATA());
				continue;
			}
			token = token.substr(1).trim();
			if (token.startsWith("*"))
			{
				token = token.substr(1).trim();
				if (!token.contains(":"))
					continue;
				String inst = token.substr(0, token.indexOf(":")).trim();
				String data = token.substr(token.indexOf(":") + 1).trim();
				DFD_LIST paramList;
				StringTokens st2 = data.tokenize(";");
				String param = "";
				while (st2.hasNext())
				{
					param = st2.next();
					DataBlock p = parseParameter(param);
					if (p.isValid())
						paramList.push_back(p);
					else
						gdata.errorQueue.push(Error::INVALID_DATABLOCK, ERROR_DATA());
				}
				onCustomInstruction(inst.toLowerCase(), paramList, paramList.size());
			}
		}
		st.cycle();
		while (st.hasNext())
		{
			token = st.next();
			if (!token.startsWith("["))
			{
				gdata.errorQueue.push(Error::INVALID_DATAFILE_BLOCK, ERROR_DATA());
				continue;
			}
			token = token.substr(1).trim();
			if (token.startsWith("$") || token.startsWith("*") || token.startsWith("#") || token.startsWith("@") || token.startsWith("!"))
				continue;
			else if (token.startsWith(">"))
			{
				if (Flags::isset(FLG_DEBUG))
				{
					token = token.substr(1).trim();
					std::cout << token.cpp() << "\n";
				}
				continue;
			}
			else
			{
				if (!token.contains(":"))
					continue;
				String inst = token.substr(0, token.indexOf(":")).trim();
				String data = token.substr(token.indexOf(":") + 1).trim();
				DFD_LIST paramList;
				StringTokens st2 = data.tokenize(";");
				String param = "";
				while (st2.hasNext())
				{
					param = st2.next();
					DataBlock p = parseParameter(param);
					if (p.isValid())
						paramList.push_back(p);
					else
						gdata.errorQueue.push(Error::INVALID_DATABLOCK, ERROR_DATA());
				}
				onCustomInstruction(inst.toLowerCase(), paramList, paramList.size());
			}
		}
		st.cycle();
		while (st.hasNext()) //Action round
		{
			token = st.next();
			if (!token.startsWith("["))
			{
				gdata.errorQueue.push(Error::INVALID_DATAFILE_BLOCK, ERROR_DATA());
				continue;
			}
			token = token.substr(1).trim();
			if (token.startsWith("$"))
			{
				token = token.substr(1).trim();
				String action = token.trim();
				DFD_LIST params;
				if (action.contains("{") && action.contains("}")) //TODO: Add error for matching parenthesis
				{
					String p = action.substr(action.indexOf("{") + 1, action.indexOf("}"));
					action = action.substr(0, action.indexOf("{")).toLowerCase().trim();
					StringTokens st2 = p.tokenize(";");
					while (st2.hasNext())
						params.push_back(parseParameter(st2.next()));
				}
				else
					action = action.toLowerCase();
				onCustomAction(action, params);
			}
		}
	}

	void DataFile::addLocal(String name, int value)
	{
		DataBlock fv;
		fv.type = eDataBlockType::Int;
		fv.name = name;
		fv.value = String::intToStr(value);
		m_localDefs.push_back(fv);
	#ifdef __DEBUG__
		GameData::instance().debugConsole.system(StringBuilder("add_local: ").add(name).add("=").add(value).get());
	#endif
	}

	void DataFile::addLocal(String name, String value, bool literal)
	{
		DataBlock fv;
		fv.type = (literal ? eDataBlockType::Literal : eDataBlockType::String);
		fv.name = name;
		fv.value = value;
		m_localDefs.push_back(fv);
	#ifdef __DEBUG__
		GameData::instance().debugConsole.system(StringBuilder("add_local: ").add(name).add("=").add(value).add(" - LITERAL").get());
	#endif
	}

	const DataBlock DataFile::getLocal(String name)
	{
		for (auto& fv : m_localDefs)
		{
			if (fv.name.trim() == name.trim())
				return std::as_const(fv);
		}
		DataBlock fv;
		m_gdata->errorQueue.push(Error::LOCAL_DATABLOCK_NOT_FOUND, ERROR_DATA());
		return std::as_const(fv);
	}

	void DataFile::addGlobal(String name, int value)
	{
		DataBlock fv;
		fv.type = eDataBlockType::Int;
		fv.name = name;
		fv.value = String::intToStr(value);
		m_globalDefs.push_back(fv);
	#ifdef __DEBUG__
		GameData::instance().debugConsole.system(StringBuilder("add_global: ").add(name).add("=").add(value).get());
	#endif
	}

	void DataFile::addGlobal(String name, String value, bool literal)
	{
		DataBlock fv;
		fv.type = (literal ? eDataBlockType::Literal : eDataBlockType::String);
		fv.name = name;
		fv.value = value;
		m_globalDefs.push_back(fv);
	#ifdef __DEBUG__
		GameData::instance().debugConsole.system(StringBuilder("add_global: ").add(name).add("=").add(value).add(" - LITERAL").get());
	#endif
	}

	DataBlock DataFile::getGlobal(String name)
	{
		for (auto& fv : m_globalDefs)
		{
			if (fv.name.trim() == name.trim())
				return std::as_const(fv);
		}
		DataBlock fv;
		GameData::instance().errorQueue.push(Error::GLOBAL_DATABLOCK_NOT_FOUND, ERROR_DATA());
		return fv;//std::as_const(fv);
	}

	void DataFile::loadDefines(String data)
	{
		StringTokens st = data.tokenize("]");
		String token = "";
		while (st.hasNext())
		{
			token = st.next();
			if (!token.startsWith("["))
			{
				m_gdata->errorQueue.push(Error::INVALID_DATAFILE_BLOCK, ERROR_DATA());
				continue;
			}
			token = token.substr(1).trim();
			if (token.startsWith("#")) //Local var
			{
				if (!token.contains("="))
				{
					m_gdata->errorQueue.push(Error::INVALID_DATABLOCK_DEFINE_VALUE, ERROR_DATA());
					continue;
				}
				String name = "", value = "";
				name = token.substr(1, token.indexOf("=")).trim();
				value = token.substr(token.indexOf("=") + 1).trim();
				if (value.startsWith("%"))
				{
					value = value.substr(1).trim();
					addLocal(name, value, true);
				}
				else if (!value.startsWith("\"") && !value.endsWith("\""))
					addLocal(name, value.toInt());
				else if (value.startsWith("\"") && value.endsWith("\""))
				{
					value = value.substr(1, value.length() - 1);
					addLocal(name, value);
				}
				else
				{
					m_gdata->errorQueue.push(Error::INVALID_DATABLOCK_DEFINE_VALUE, ERROR_DATA());
					continue;
				}
			}
			else if (token.startsWith("@")) //Global var
			{
				if (!token.contains("="))
				{
					m_gdata->errorQueue.push(Error::INVALID_DATABLOCK_DEFINE_VALUE, ERROR_DATA());
					continue;
				}
				String name = "", value = "";
				name = token.substr(1, token.indexOf("=")).trim();
				value = token.substr(token.indexOf("=") + 1).trim();
				if (value.startsWith("%"))
				{
					value = value.substr(1).trim();
					DataFile::addGlobal(name, value, true);
				}
				else if (!value.startsWith("\"") && !value.endsWith("\""))
					DataFile::addGlobal(name, value.toInt());
				else if (value.startsWith("\"") && value.endsWith("\""))
				{
					value = value.substr(1, value.length() - 1);
					DataFile::addGlobal(name, value);
				}
				else
				{
					m_gdata->errorQueue.push(Error::INVALID_DATABLOCK_DEFINE_VALUE, ERROR_DATA());
					continue;
				}
			}
		}
	}

	String DataFile::replaceDefines(String data)
	{
		String newData = data;
		for (auto& def : m_localDefs)
		{
			if (def.type == eDataBlockType::String)
				newData = newData.replaceAll(def.name, String("\"").add(def.value).add("\""));
			else if (def.type == eDataBlockType::Int)
				newData = newData.replaceAll(def.name, def.value);
			else if (def.type == eDataBlockType::Literal)
				newData = newData.replaceAll(def.name, def.value);
		}
		for (auto& def : m_globalDefs)
		{
			if (def.type == eDataBlockType::String)
				newData = newData.replaceAll(def.name, String("\"").add(def.value).add("\""));
			else if (def.type == eDataBlockType::Int)
				newData = newData.replaceAll(def.name, def.value);
			else if (def.type == eDataBlockType::Literal)
				newData = newData.replaceAll(def.name, def.value);
		}
		return newData;
	}

	DataBlock DataFile::parseParameter(String param)
	{
		DataBlock db;
		if (!param.contains("="))
		{
			db.invalidate();
			m_gdata->errorQueue.push(Error::DATABLOCK_PARAM_ASSING_MISSING, ERROR_DATA());
			return db;
		}
		param = param.trim();
		String name = "", value = "";
		name = param.substr(0, param.indexOf("=")).trim();
		value = param.substr(param.indexOf("=") + 1).trim();
		db.name = name.toLowerCase();
		db.value = value;
		if (value.startsWith("\""))
		{
			if (!value.endsWith("\""))
			{
				db.invalidate();
				m_gdata->errorQueue.push(Error::UNMATCHED_QUOTE_IN_PARAM_VALUE, ERROR_DATA());
				return db;
			}
			value = value.substr(1, value.length() - 1);
			db.type = eDataBlockType::String;
			db.value = value;
		}
		else if (value.startsWith("("))
		{
			if (!value.endsWith(")"))
			{
				db.invalidate();
				m_gdata->errorQueue.push(Error::UNMATCHED_PARENTHESIS_IN_PARAM_VALUE, ERROR_DATA());
				return db;
			}
			db.type = (value.contains("\"") ? eDataBlockType::StringArray : eDataBlockType::IntArray);
		}
		else if (value.toLowerCase().startsWith("f"))
		{
			value = value.substr(1).trim();
			db.value = value;
			db.type = eDataBlockType::Float;
		}
		else
			db.type = eDataBlockType::Int;
		return db;
	}

#ifdef __DEBUG__
	BitmapFont& FontFile::getDebugFont(void)
	{
		return (hasDebugFont() ? *m_dbgFont : INVALID_REF(BitmapFont));
	}

	bool FontFile::hasDebugFont(void)
	{
		return (m_dbgFont != nullptr ? (m_dbgFont->isValid() ? true : false) : false);
	}
#endif

	ttf::Font& FontFile::getDefaultTTF(void)
	{
		return (hasDefaultTTF() ? *m_defTTF : INVALID_REF(ttf::Font));
	}

	bool FontFile::hasDefaultTTF(void)
	{
		return (m_defTTF != nullptr ? true : false);
	}

	void FontFile::onCustomAction(String action, DFD_LIST params)
	{
	#ifdef __DEBUG__
		if (action == "load_dbg_font")
		{
			DataBlock dbgfnt = getLocal("DEBUG_FONT");
			m_dbgFont = &font(dbgfnt.value);
		}
		else if (action == "load_def_ttf")
	#else
		if (action == "load_def_ttf")
	#endif
		{
			DataBlock defttf = getLocal("DEFAULT_TTF");
			m_defTTF = &(ttf::Fonts::getFont(defttf.value));
		}
	}

	void FontFile::onCustomInstruction(String instr, DFD_LIST data, int paramCount)
	{
		std::vector<int> tmpArr;
		if (instr == "bitmapfont")
		{
			String imgPath = "", name = "Unnamed font";
			UI8Point charSize = UI8Point(AUTO__BITMAP_FONT_CHAR_WIDTH, AUTO__BITMAP_FONT_CHAR_HEIGHT);
			I8Point padding = I8Point(0, 0);
			for (auto& dat : data)
			{
				if (dat.name == "imagepath")
					imgPath = dat.value;
				else if (dat.name == "name")
					name = dat.value;
				else if (dat.name == "charsize")
				{
					tmpArr = dat.asIntArray();
					if (tmpArr.size() != 2)
					{
						m_gdata->errorQueue.push(Error::INVALID_PARAM_ARRAY_SIZE, ERROR_DATA());
						continue;
					}
					charSize = UI8Point(tmpArr[0], tmpArr[1]);
				}
				else if (dat.name == "padding")
				{
					tmpArr = dat.asIntArray();
					if (tmpArr.size() != 2)
					{
						m_gdata->errorQueue.push(Error::INVALID_PARAM_ARRAY_SIZE, ERROR_DATA());
						continue;
					}
					padding = I8Point(tmpArr[0], tmpArr[1]);
				}
			}
			BitmapFont font(imgPath, charSize, padding);
			font.setName(name);
			m_fontList.push_back(font);
		}
		else if (instr == "truetypefont")
		{
			String fontPath = "", name = "Unnamed font";
			for (auto& dat : data)
			{
				if (dat.name == "fontpath")
					fontPath = dat.value;
				else if (dat.name == "name")
					name = dat.value;
			}
			ttf::Fonts::addFont(fontPath, name);
		}
	}

	BitmapFont& FontFile::font(String name)
	{
		for (auto& font : m_fontList)
		{
			if (font.getName().trim() == name.trim())
				return font;
		}
		m_gdata->errorQueue.push(Error::BITMAP_FONT_NOT_FOUND, ERROR_DATA());
		return INVALID_REF(BitmapFont);
	}

	BitmapFont& FontFile::font(int32 id)
	{
		for (auto& font : m_fontList)
		{
			if (font.getID() == id)
				return font;
		}
		m_gdata->errorQueue.push(Error::BITMAP_FONT_NOT_FOUND, ERROR_DATA());
		return INVALID_REF(BitmapFont);
	}

	ttf::Font& FontFile::ttf(String name)
	{
		return ttf::Fonts::getFont(name);
	}

	ttf::Font& FontFile::ttf(int32 id)
	{
		return ttf::Fonts::getFont(id);
	}

	void AnimationSheetFile::onCustomInstruction(String instr, DFD_LIST data, int paramCount)
	{
		std::vector<int> tmpArr;
		if (instr == "spritesheet")
		{
			for (auto& dat : data)
			{
				if (dat.name == "imagepath")
					m_texture.load(dat.value);
				else if (dat.name == "name")
					m_name = dat.value;
				else if (dat.name == "numsprites")
				{
					tmpArr = dat.asIntArray();
					if (tmpArr.size() != 2)
					{
						m_gdata->errorQueue.push(Error::INVALID_PARAM_ARRAY_SIZE, ERROR_DATA());
						continue;
					}
					m_animCount = UI16Point(tmpArr[0], tmpArr[1]);
				}
				else if (dat.name == "spritesize")
				{
					tmpArr = dat.asIntArray();
					if (tmpArr.size() != 2)
					{
						m_gdata->errorQueue.push(Error::INVALID_PARAM_ARRAY_SIZE, ERROR_DATA());
						continue;
					}
					m_animSize = UI8Point(tmpArr[0], tmpArr[1]);
				}
			}
		}
		else if (instr == "animation")
		{
			AnimationData ad;
			Animation anim;
			String name;
			for (auto& dat : data)
			{
				if (dat.name == "name")
					name = dat.value;
				else if (dat.name == "framecount")
					ad.frameCount = dat.asInt();
				else if (dat.name == "delay")
					ad.delay = dat.asInt();
				else if (dat.name == "stillframe")
					ad.stillFrame = dat.asInt();
				else if (dat.name == "grid")
				{
					tmpArr = dat.asIntArray();
					if (tmpArr.size() != 2)
					{
						m_gdata->errorQueue.push(Error::INVALID_PARAM_ARRAY_SIZE, ERROR_DATA());
						continue;
					}
					ad.columns = tmpArr[0];
					ad.rows = tmpArr[1];
				}
				else if (dat.name == "offset")
				{
					tmpArr = dat.asIntArray();
					if (tmpArr.size() != 2)
					{
						m_gdata->errorQueue.push(Error::INVALID_PARAM_ARRAY_SIZE, ERROR_DATA());
						continue;
					}
					ad.generalOffsetX = tmpArr[0];
					ad.generalOffsetY = tmpArr[1];
				}
				else if (dat.name == "frameoffset")
				{
					tmpArr = dat.asIntArray();
					if (tmpArr.size() != 2)
					{
						m_gdata->errorQueue.push(Error::INVALID_PARAM_ARRAY_SIZE, ERROR_DATA());
						continue;
					}
					ad.columnOffset = tmpArr[0];
					ad.rowOffset = tmpArr[1];
				}
				else if (dat.name == "framesize")
				{
					tmpArr = dat.asIntArray();
					if (tmpArr.size() != 2)
					{
						m_gdata->errorQueue.push(Error::INVALID_PARAM_ARRAY_SIZE, ERROR_DATA());
						continue;
					}
					ad.frameWidth = tmpArr[0];
					ad.frameHeight = tmpArr[1];
				}
				else if (dat.name == "backwards")
					ad.playBackwards = INT_BOOL(dat.asInt());
				else if (dat.name == "randomframes")
					ad.randomFrames = INT_BOOL(dat.asInt());
				else if (dat.name == "still")
					ad.isStill = INT_BOOL(dat.asInt());
				else if (dat.name == "spritefomatter")
				{
					tmpArr = dat.asIntArray();
					if (tmpArr.size() != 4)
					{
						m_gdata->errorQueue.push(Error::INVALID_PARAM_ARRAY_SIZE, ERROR_DATA());
						continue;
					}
					ad.sf.up = tmpArr[0];
					ad.sf.down = tmpArr[1];
					ad.sf.left = tmpArr[2];
					ad.sf.right = tmpArr[3];
				}
			}
			anim.create(ad);
			anim.setName(name);
			m_animList.push_back(anim);
		}
	}

	Animation AnimationSheetFile::animation(int32 id)
	{
		for (auto& anim : m_animList)
		{
			if (anim.getID() == id)
				return anim;
		}
		m_gdata->errorQueue.push(Error::ANIMATION_NOT_FOUND, ERROR_DATA());
		return INVALID_OBJ(Animation);
	}

	Animation AnimationSheetFile::animation(String name)
	{
		for (auto& anim : m_animList)
		{
			if (anim.getName().trim() == name.trim())
				return anim;
		}
		m_gdata->errorQueue.push(Error::ANIMATION_NOT_FOUND, ERROR_DATA());
		return INVALID_OBJ(Animation);
	}

	Sprite TilesetFile::sprite(int32 tid)
	{
		for (auto& spr : m_spriteList)
		{
			if (spr.getID() == tid)
				return spr;
		}
		m_gdata->errorQueue.push(Error::SPRITE_NOT_FOUND_IN_LIST, ERROR_DATA());
		return INVALID_OBJ(Sprite);
	}

	Sprite TilesetFile::sprite(String name)
	{
		for (auto& spr : m_spriteList)
		{
			if (spr.getName().trim() == name.trim())
				return spr;
		}
		m_gdata->errorQueue.push(Error::SPRITE_NOT_FOUND_IN_LIST, ERROR_DATA());
		return INVALID_OBJ(Sprite);
	}

	Sprite TilesetFile::sprite(Color color)
	{
		for (auto& spr : m_spriteList)
		{
			if (spr.getMapColor() == color)
				return spr;
		}
		m_gdata->errorQueue.push(Error::SPRITE_NOT_FOUND_IN_LIST, ERROR_DATA());
		return INVALID_OBJ(Sprite);
	}

	void TilesetFile::onCustomInstruction(String instr, DFD_LIST data, int paramCount)
	{
		std::vector<int> tmpArr;
		if (instr == "tileset")
		{
			for (auto& dat : data)
			{
				if (dat.name == "imagepath")
					m_texture.load(dat.value);
				else if (dat.name == "name")
					m_name = dat.value;
				else if (dat.name == "numtiles")
				{
					tmpArr = dat.asIntArray();
					if (tmpArr.size() != 2)
					{
						m_gdata->errorQueue.push(Error::INVALID_PARAM_ARRAY_SIZE, ERROR_DATA());
						continue;
					}
					m_spriteCount = UI16Point(tmpArr[0], tmpArr[1]);
				}
				else if (dat.name == "tilesize")
				{
					tmpArr = dat.asIntArray();
					if (tmpArr.size() != 2)
					{
						m_gdata->errorQueue.push(Error::INVALID_PARAM_ARRAY_SIZE, ERROR_DATA());
						continue;
					}
					m_spriteSize = UI8Point(tmpArr[0], tmpArr[1]);
				}
			}
		}
		else if (instr == "tile")
		{
			Sprite spr;
			Color mapColor;
			SpriteUnit tUnit;
			TexInfo texInfo;
			eSolidLevel solid = eSolidLevel::NoSolid;
			for (auto& dat : data)
			{
				if (dat.name == "name")
					spr.setName(dat.value);
				else if (dat.name == "type")
					tUnit.type = dat.asInt();
				else if (dat.name == "length")
					tUnit.frames = dat.asInt();
				else if (dat.name == "delay")
					tUnit.delay = dat.asInt();
				else if (dat.name == "start")
				{
					tmpArr = dat.asIntArray();
					if (tmpArr.size() != 2)
					{
						m_gdata->errorQueue.push(Error::INVALID_PARAM_ARRAY_SIZE, ERROR_DATA());
						continue;
					}
					tUnit.start = CONVERT_2D_1D(tmpArr[0], tmpArr[1], m_spriteCount.x);
				}
				else if (dat.name == "solid")
					solid = (eSolidLevel)dat.asInt();
				else if (dat.name == "lighttemplate")
					spr.setLightEmitter(dat.asInt());
				else if (dat.name == "texsize")
				{
					tmpArr = dat.asIntArray();
					if (tmpArr.size() != 2)
					{
						m_gdata->errorQueue.push(Error::INVALID_PARAM_ARRAY_SIZE, ERROR_DATA());
						continue;
					}
					texInfo.texSize = FPoint(tmpArr[0], tmpArr[1]);
				}
				else if (dat.name == "tintcolor")
					texInfo.tintColor = dat.asColor();
				else if (dat.name == "mapcolor")
					mapColor = dat.asColor();
				else if (dat.name == "shareddata")
				{
					if (dat.asInt() == 0)
						continue;
					spr.useSharedData(true);
				}
			}
			tUnit.encode();
			spr.set(tUnit, mapColor, texInfo, solid);
			if (spr.hasSharedData())
				spr.setSharedData(SpriteSharedData::addData(tUnit));
			m_spriteList.push_back(spr);
		}
	}

	WorldFile::WorldFile(String path, GameData& gdata, Application& app) : m_firstMap(nullptr)
	{
		m_enterMap.setName("enter_map");
		m_enterMap.addParamDef("name", eDataBlockType::String);
		load(path, gdata, app);
	}

	void WorldFile::onCustomAction(String action, DFD_LIST params)
	{
		if (m_enterMap.matches(action, params))
		{
			if (params.size() != 1) //TODO: Add warning
				return;
			DataBlock mapName = params[0];
			if (mapName.name != "name") //TODO: Add warning
				return;
			m_firstMap = &map(mapName.value);
		}
	}

	void WorldFile::onCustomInstruction(String instr, DFD_LIST data, int paramCount)
	{
		std::vector<int> tmpArr;
		if (instr == "map")
		{
			Map map;
			String mapPath = "", name = "";
			TilesetFile* tset = nullptr;
			UI16Point size(200, 200); //TODO: add AUTO__ macros for default values
			for (auto& dat : data)
			{
				if (dat.name == "filepath")
					mapPath = dat.value;
				else if (dat.name == "name")
					name = dat.value;
				else if (dat.name == "size")
				{
					tmpArr = dat.asIntArray();
					if (tmpArr.size() != 2)
					{
						m_gdata->errorQueue.push(Error::INVALID_PARAM_ARRAY_SIZE, ERROR_DATA());
						continue;
					}
					size = UI16Point(tmpArr[0], tmpArr[1]);
				}
				else if (dat.name == "tileset")
					tset = &(WorldFile::addTileset(dat.value, *m_app));
			}
			if (tset != nullptr)
			{
				map.loadFromFile(mapPath, *m_gdata, size, name, *tset);
				m_mapList.push_back(map);
			}
			else
				m_gdata->errorQueue.push(Error::TILESET_NOT_LOADED, ERROR_DATA());
		}
	}

	Map& WorldFile::map(String name)
	{
		for (auto& map : m_mapList)
		{
			if (map.getName().trim() == name.trim())
				return map;
		}
		m_gdata->errorQueue.push(Error::MAP_NOT_FOUND_IN_LIST, ERROR_DATA());
		return INVALID_REF(Map);
	}

	Map& WorldFile::map(int32 id)
	{
		for (auto& map : m_mapList)
		{
			if (map.getID() == id)
				return map;
		}
		m_gdata->errorQueue.push(Error::MAP_NOT_FOUND_IN_LIST, ERROR_DATA());
		return INVALID_REF(Map);
	}

	bool WorldFile::isTilesetInLibrary(String name)
	{
		for (auto& tset : tilesetLib)
		{
			if (tset.getName().trim() == name.trim())
				return true;
		}
		return false;
	}

	TilesetFile& WorldFile::getTileset(String name)
	{
		for (auto& tset : tilesetLib)
		{
			if (tset.getName().trim() == name.trim())
				return tset;
		}
		GameData::instance().errorQueue.push(Error::TILESET_NOT_FOUND, ERROR_DATA());
		return INVALID_REF(TilesetFile);
	}
	
	TilesetFile& WorldFile::addTileset(String tsetDataPath, Application& app)
	{
		return addTileset(TilesetFile(tsetDataPath, GameData::instance(), app));
	}

	TilesetFile& WorldFile::addTileset(TilesetFile tset)
	{
		if (WorldFile::isTilesetInLibrary(tset.getName()))
			return WorldFile::getTileset(tset.getName());
		WorldFile::tilesetLib.push_back(tset);
		return WorldFile::tilesetLib[WorldFile::tilesetLib.size() - 1];
	}

	LoadScriptFile::LoadScriptFile(String path, GameData& gdata, Application& app) : m_app(&app)
	{
		m_loadWorldFile.setName("load_world_file");
		m_loadWorldFile.addParamDef("path", eDataBlockType::String);
		m_loadFontFile.setName("load_font_file");
		m_loadFontFile.addParamDef("path", eDataBlockType::String);
		m_loadTileData.setName("load_tile_data");
		m_loadTileData.addParamDef("size", eDataBlockType::IntArray);
		m_loadTileData.addParamDef("screen_tiles", eDataBlockType::IntArray);
		m_setupSplashScreen.setName("splash_screen_setup");
		m_setupSplashScreen.addParamDef("image_path", eDataBlockType::String);
		m_setupSplashScreen.addParamDef("duration", eDataBlockType::Int);
		m_setupSplashScreen.addParamDef("scale", eDataBlockType::Float);
		m_setupSplashScreen.addParamDef("show", eDataBlockType::Int);
		load(path, gdata, app);
	}

	void LoadScriptFile::onCustomAction(String action, DFD_LIST params)
	{
		if (m_loadFontFile.matches(action, params))
		{
			m_app->loadFonts(params[0].value);
		}
		else if (m_loadTileData.matches(action, params))
		{
			std::vector<int> tmp = GET_PARAM("size", params).asIntArray();
			m_gdata->tileSize = UI8Point(tmp[0], tmp[1]);
			tmp = GET_PARAM("screen_tiles", params).asIntArray();
			m_gdata->screenTiles = UI8Point(tmp[0], tmp[1]);
		}
		else if (m_loadWorldFile.matches(action, params))
		{
			m_app->loadWorld(params[0].value);
		}
		else if (m_setupSplashScreen.matches(action, params))
		{
			String img = GET_PARAM("image_path", params).value;
			int32 duration = GET_PARAM("duration", params).asInt();
			float scale = GET_PARAM("scale", params).asFloat();
			bool show = GET_PARAM("show", params).asBool();
			Flags::value(FLG_SHOW_SPLASH_SCREEN, show);
			m_app->splashScreenSetup(img, duration, scale);
		}
	}

	AnimationSheetFile::~AnimationSheetFile(void) = default;
	FontFile::~FontFile(void) = default;
	TilesetFile::~TilesetFile(void) = default;
	WorldFile::~WorldFile(void) = default;
	LoadScriptFile::~LoadScriptFile(void) = default;
}
