#include "DataTypes.hpp"
#include <iostream>
#include <sstream>
#include "BitmapFont.hpp"
#include "VertexArray.hpp"
#include "StringTokens.hpp"
#include "GameData.hpp"
#include "Application.hpp"
#include "Entity.hpp"

namespace Omnia
{
	void StringInfo::create(String _string, BitmapFont& font, Color color)
	{
		if (_string.trim() == "")
		{
			//GameData::instance().errorQueue.push(Error::EMPTY_STRING, ERROR_DATA());
			return;
		}
		m_string = _string;
		m_bitmapFont = &font;

		m_string = String(" ").add(m_string); //TODO: Hardcoded space at beginning becouse of a bug where the first character appears with a few seconds delay
		for (uint32 i = 0; i < m_string.length(); i++)
			m_data.push_back(font.getChar(m_string.at(i), color));

		setValid(true);
	}

	RealTime::RealTime(void)
	{
		minutes = 0;
		hours = 0;
		days = 0;
		months = 0;
		years = 2020;
		m_timeOfDay = 0.0f;
		m_totalSeconds = 0.0f;
	}

	const float& RealTime::start(void)
	{
		m_rtClock.restart();
		m_timeOfDay = CAP((1.0f / ((float)(TM_G_MINUTES_FOR_G_HOUR * TM_G_HOURS_FOR_G_DAY))) * ((hours * TM_G_MINUTES_FOR_G_HOUR) + (minutes)), 1.0f);
		return m_timeOfDay;
	}

	String RealTime::asString(void)
	{
		std::ostringstream ss;
		ss << "Time: " << getFormattedTime().cpp() << " - ";
		ss << (int)(days + 1) << " " << convertMonth().cpp() << " " << (int)(years);
		return String(ss.str());
	}

	void RealTime::update(void)
	{
		long int elapsed = m_rtClock.getElapsedTime().asSeconds();
		if (hours == 255)
			hours = TM_G_HOURS_FOR_G_DAY - 1;
		else if (hours >= TM_G_HOURS_FOR_G_DAY)
			hours = 0;
		if (elapsed >= TM_R_SECONDS_FOR_G_MINUTE)
		{
			minutes++;
			if (minutes >= TM_G_MINUTES_FOR_G_HOUR)
			{
				hours++;
				if (hours >= TM_G_HOURS_FOR_G_DAY)
				{
					days++;
					if ((months == (uint8)eMonths::January || months == (uint8)eMonths::March ||
						 months == (uint8)eMonths::May || months == (uint8)eMonths::July ||
						 months == (uint8)eMonths::August || months == (uint8)eMonths::October ||
						 months == (uint8)eMonths::December) && days >= TM_G_DAYS_FOR_G_LONG_MONTH)
					{
						months++;
						if (months > (uint8)eMonths::December)
						{
							years++;
							months = (uint8)eMonths::January;
						}
						days = 0;
					}
					else if ((months == (uint8)eMonths::April || months == (uint8)eMonths::June ||
							  months == (uint8)eMonths::September || months == (uint8)eMonths::November) && days >= TM_G_DAYS_FOR_G_MEDIUM_MONTH)
					{
						months++;
						days = 0;
					}
					else if (months == (uint8)eMonths::February)
					{
						if ((years % 4 == 0 && days >= TM_G_DAYS_FOR_G_SHORT_MONTH + 1) ||
							(years % 4 != 0 && days >= TM_G_DAYS_FOR_G_SHORT_MONTH))
						{
							months++;
							days = 0;
						}
					}
					hours = 0;
				}
				minutes = 0;
			}
			m_totalSeconds += elapsed;
			m_rtClock.restart();
			m_timeOfDay = CAP((1.0f / ((float)(TM_G_MINUTES_FOR_G_HOUR * TM_G_HOURS_FOR_G_DAY))) * ((hours * TM_G_MINUTES_FOR_G_HOUR) + (minutes)), 1.0f);
		}
	}

	String RealTime::getFormattedTime(void)
	{
		bool zh = (int)(hours / 10) < 1;
		bool zm = (int)(minutes / 10) < 1;
		std::ostringstream ss;
		ss << (zh ? "0" : "") << (int)hours << ":" << (zm ? "0" : "") << (int)minutes;
		return String(ss.str());
	}

	String RealTime::convertMonth(void)
	{
		switch (months)
		{
			case (uint8)eMonths::January:
				return "January";
			case (uint8)eMonths::February:
				return "February";
			case (uint8)eMonths::March:
				return "March";
			case (uint8)eMonths::April:
				return "April";
			case (uint8)eMonths::May:
				return "May";
			case (uint8)eMonths::June:
				return "June";
			case (uint8)eMonths::July:
				return "July";
			case (uint8)eMonths::August:
				return "August";
			case (uint8)eMonths::September:
				return "September";
			case (uint8)eMonths::October:
				return "October";
			case (uint8)eMonths::November:
				return "November";
			case (uint8)eMonths::December:
				return "December";
			default:
				break;
		}
		return "_MONTH_";
	}

	DataBlock::DataBlock(void)
	{
		type = eDataBlockType::Invalid;
		name = "";
		value = "null";
	}

	int DataBlock::asInt(void)
	{
		if (type != eDataBlockType::Int)
			return 0;
		return value.toInt();
	}

	float DataBlock::asFloat(void)
	{
		if (type != eDataBlockType::Float)
			return 0;
		return value.toFloat();
	}

	String DataBlock::asString(void)
	{
		return value.trim();
	}

	std::vector<int> DataBlock::asIntArray(void)
	{
		std::vector<int> arr;
		if (type != eDataBlockType::IntArray || value.contains("\""))
		{
			GameData::instance().errorQueue.push(Error::INVALID_DATABLOCK_TYPE, ERROR_DATA());
			return arr;
		}
		if (!value.trim().startsWith("(") || !value.trim().endsWith(")"))
		{
			GameData::instance().errorQueue.push(Error::INVALID_DATABLOCK_TYPE, ERROR_DATA());
			return arr;
		}
		value = value.substr(1, value.length() - 1);
		StringTokens st = value.tokenize(",");
		while (st.hasNext())
			arr.push_back(st.next().toInt());
		return arr;
	}

	std::vector<String> DataBlock::asStringArray(void)
	{
		std::vector<String> arr;
		if (type != eDataBlockType::StringArray || !value.contains("\""))
		{
			GameData::instance().errorQueue.push(Error::INVALID_DATABLOCK_TYPE, ERROR_DATA());
			return arr;
		}
		if (!value.trim().startsWith("(") || !value.trim().endsWith(")"))
		{
			GameData::instance().errorQueue.push(Error::INVALID_DATABLOCK_TYPE, ERROR_DATA());
			return arr;
		}
		value = value.substr(1, value.length() - 1);
		StringTokens st = value.tokenize(",");
		String token = "";
		while (st.hasNext())
		{
			token = st.next();
			if (!token.startsWith("\"") || !token.endsWith("\""))
			{
				GameData::instance().errorQueue.push(Error::INVALID_DATABLOCK_TYPE, ERROR_DATA());
				continue;
			}
			token = token.substr(1, token.length() - 1);
			arr.push_back(token);
		}
		return arr;
	}

	DataBlock DataBlock::Invalid = DataBlock(__INVALID_OBJ);

	String DataBlock::asLiteral(void)
	{
		if (type != eDataBlockType::Literal)
		{
			GameData::instance().errorQueue.push(Error::INVALID_DATABLOCK_TYPE, ERROR_DATA());
			return "";
		}
		return value.trim();
	}

	Color DataBlock::asColor(void)
	{
		if (type != eDataBlockType::IntArray)
		{
			GameData::instance().errorQueue.push(Error::INVALID_DATABLOCK_TYPE, ERROR_DATA());
			return Color::Transparent;
		}
		std::vector<int> v = asIntArray();
		if (v.size() != 4)
		{
			GameData::instance().errorQueue.push(Error::INVALID_ARRAY_SIZE, ERROR_DATA());
			return Color::Transparent;
		}
		return Color(v[0], v[1], v[2], v[3]);
	}

	bool DataFunctionHeader::matches(String name, DFD_LIST params)
	{
		if (name.trim().toLowerCase() != funcName || params.size() != paramDefs.size())
			return false;
		int count = params.size();
		bool keepGoing = false;
		while (count > 0)
		{
			keepGoing = false;
			for (uint16 i = 0; i < paramDefs.size(); i++)
			{
				if (paramDefs[i].name.toLowerCase().trim() == params[count - 1].name.toLowerCase().trim() && paramDefs[i].type == params[count - 1].type)
				{
					count--;
					keepGoing = true;
					break;
				}
			}
			if (!keepGoing)
				return false;
		}
		return true;
	}

	DataBlock& DataFunctionHeader::get(String name, DFD_LIST params)
	{
		for (auto& db : params)
		{
			if (db.name.trim().toLowerCase() == name.toLowerCase().trim())
				return db;
		}
		return DataBlock::Invalid;
	}

	Color PixelRect::at(uint32 x, uint32 y)
	{
		int32 ox = rect.x + x;
		int32 oy = rect.y + y;
		if (pixelSize.x == -1 || pixelSize.y == -1 || arraySize == -1)
		{
			//TODO: Warning for unset array size
			return Color::Transparent;
		}
		uint32 i = CONVERT_2D_1D(ox, oy, pixelSize.x) * 4;
		if (!rect.contains(UIPoint(ox, oy), true) || i >= arraySize)
		{
			//TODO: Warning for out of bounds
			return Color::Transparent;
		}
		return Color(pixels[i], pixels[i + 1], pixels[i + 2], pixels[i + 3]);
	}

	bool PixelRect::intersects(UIRect rect2, Camera& camera)
	{
		return UIRect(rect2.x + camera.getHorizontal(), rect2.y + camera.getVertical(), rect.w, rect.h).intersects(rect2);
	}

	void LightSource::create(FPoint source, FPoint size, eLightType type)
	{
		this->source = source;
		this->type = type;
		this->size = size;
		setID(LightSource::s_nextLightID++);
		textureRect = FRect(0, 0, 32, 32);
		state = eLightState::On;
		attachedEntity = nullptr;
		m_glowDelay = 10;
		offOnTime = true;
		m_time = 0;
		m_glowSteps = 10;
		m_currentGlowStep = 0;
		m_glowReverse = false;
		m_colored = false;
		name = "";
		optimize = false;
	}

	void LightSource::update(void)
	{
		if (type == eLightType::Glowing)
		{
			if (m_time++ >= m_glowDelay)
			{
				m_time = 0;
				if (!m_glowReverse)
				{
					size.x -= 1;
					size.y -= 1;
					source.x += 1;
					source.y += 1;
					if (m_currentGlowStep++ >= m_glowSteps)
						m_glowReverse = true;
				}
				else
				{
					size.x += 1;
					size.y += 1;
					source.x -= 1;
					source.y -= 1;
					if (m_currentGlowStep-- <= 0)
						m_glowReverse = false;
				}
			}
		}
		if (attachedEntity == nullptr)
			return;
		source.x = attachedEntity->getx();
		source.y = attachedEntity->gety();
	}

	bool LightSource::isOnScreen(FPoint screenSize)
	{
		Camera& cam = GameData::instance().camera;
		FRect screen(cam.getHorizontal(), cam.getVertical(), cam.getHorizontal() + screenSize.x, cam.getVertical() + screenSize.y);
		FPoint a = source;
		FPoint b(source.x + size.x, source.y);
		FPoint c(source.x + size.x, source.y + size.y);
		FPoint d(source.x, source.y + size.y);
		return screen.contains(a) || screen.contains(b) || screen.contains(c) || screen.contains(d);
	}
}
