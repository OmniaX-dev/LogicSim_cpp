#ifndef __DATATYPES__H__
#define __DATATYPES__H__

#include "Defines.hpp"
#include "Enums.hpp"
#include "String.hpp"
#include "Color.hpp"
#include "Object.hpp"
#include <SFML/Graphics.hpp>
#include <memory>

namespace Omnia
{
	template<class T>
	class Point
	{
		public:
			T x;
			T y;

		public:
			inline Point(void) : x(0), y(0) {}
			inline Point(T xx, T yy) : x(xx), y(yy) {}
			template <class T2> inline Point(Point<T2> copy)
			{
				x = (T2)(copy.x);
				y = (T2)(copy.y);
			}
			inline void print(bool nl = true) { std::cout << "X: " << (T)(x) << " - Y: " << (T)(y) << (nl ? "\n" : ""); }
	};

	template <class T>
	class Rect
	{
		public:
			T x;
			T y;
			T w;
			T h;

		public:
			inline Rect(void) : x(0), y(0), w(0), h(0) {}
			inline Rect(T xx, T yy, T ww, T hh) : x(xx), y(yy), w(ww), h(hh) {}
			template <class T2> inline Rect(Rect<T2> copy)
			{
				x = (T2)(copy.x);
				y = (T2)(copy.y);
				w = (T2)(copy.w);
				h = (T2)(copy.h);
			}
			inline bool intersects(Rect<T> rect, bool includeBounds = true)
			{
				if (includeBounds)
				{
					if (x + w <= rect.x || x >= rect.x + rect.w)
						return false;
					if (y + h <= rect.y || y >= rect.y + rect.h)
						return false;
				}
				else
				{
					if (x + w < rect.x || x > rect.x + rect.w)
						return false;
					if (y + h < rect.y || y > rect.y + rect.h)
						return false;
				}
				return true;
			}
			inline Point<T> getIntersection(Rect<T> rect, bool includeBounds = true)
			{
				if (!intersects(rect, includeBounds))
					return Point<T>();
				T leftX   = MAX(x, rect.x);
				T rightX  = MIN(x + w, rect.x + rect.w);
				T topY    = MAX(y, rect.y);
				T bottomY = MIN(y + h, rect.y + rect.h);
				Rect<T> intersection(leftX, topY, rightX - leftX, bottomY - topY);
				return intersection.pair2();
			}
			inline bool contains(Point<T> p, bool includeBounds = false)
			{
				if (includeBounds)
					return p.x >= x && p.y >= y & p.x <= x + w && p.y <= y + h;
				else
					return p.x > x && p.y > y & p.x < x + w && p.y < y + h;
			}
			inline Point<T> getCenter(void)
			{
				Point<T> c;
				c.x = x + (w / 2);
				c.y = y + (h / 2);
				return c;
			}
			inline Point<T> pair1(void) { return Point<T>(x, y); }
			inline Point<T> pair2(void) { return Point<T>(w, h); }
			inline static Rect<T> combine(Point<T> p1, Point<T> p2) { return Rect<T>(p1.x, p1.y, p2.x, p2.y); }
			inline void print(bool nl = true)
			{
				std::cout << "X: " << (T)(x) << " - Y: " << (T)(y) << " - W: " << (T)(w) << " - H: " << (T)(h) << (nl ? "\n" : "");
			}
	};

	typedef unsigned char uint8;
	typedef signed char int8;
	typedef unsigned short uint16;
	typedef signed short int16;
	typedef unsigned int uint32;
	typedef signed int int32;
	typedef unsigned long int uint64;
	typedef signed long int int64;

	typedef Point<float> FPoint;
	typedef Point<double> DPoint;
	typedef Point<uint32> UIPoint;
	typedef Point<uint64> UI64Point;
	typedef Point<uint16> UI16Point;
	typedef Point<uint8> UI8Point;
	typedef Point<int32> IPoint;
	typedef Point<int64> I64Point;
	typedef Point<int16> I16Point;
	typedef Point<int8> I8Point;

	typedef Rect<float> FRect;
	typedef Rect<double> DRect;
	typedef Rect<uint32> UIRect;
	typedef Rect<uint64> UI64Rect;
	typedef Rect<uint16> UI16Rect;
	typedef Rect<uint8> UI8Rect;
	typedef Rect<int32> IRect;
	typedef Rect<int64> I64Rect;
	typedef Rect<int16> I16Rect;
	typedef Rect<int8> I8Rect;

	struct Triangle
	{
		FPoint A;
		FPoint B;
		FPoint C;
		inline Triangle(void) {}
		inline Triangle(FPoint a, FPoint b, FPoint c)
		{
			A = a;
			B = b;
			C = c;
		}
		inline Triangle(float ax, float ay, float bx, float by, float cx, float cy)
		{
			A.x = ax;
			A.y = ay;
			B.x = bx;
			B.y = by;
			C.x = cx;
			C.y = cy;
		}
	};

	struct SpriteFormatter
	{
		int up;
		int down;
		int left;
		int right;

		SpriteFormatter()
		{
			up = 3;
			down = 0;
			left = 1;
			right = 2;
		}

		SpriteFormatter(int u, int d, int l, int r)
		{
			up = u;
			down = d;
			left = l;
			right = r;
		}
	};

	class Image;
	struct AnimationData
	{
		int frameCount;
		int delay;
		int stillFrame;

		int rowOffset;
		int columnOffset;
		int generalOffsetX;
		int generalOffsetY;

		int rows;
		int columns;

		int frameWidth;
		int frameHeight;

		bool isStill;
		bool playBackwards;
		bool randomFrames;

		SpriteFormatter sf;

		AnimationData(void)
		{
			frameCount = 3;
			delay = 10;
			rowOffset = 0;
			columnOffset = 0;
			generalOffsetX = 0;
			generalOffsetY = 0;
			rows = 1;
			columns = 3;
			frameWidth = 32;
			frameHeight = 32;
			isStill = false;
			playBackwards = true;
			randomFrames = false;
			stillFrame = 1;
		}
	};

	struct TexInfo
	{
		FPoint texCoords;
		FPoint texSize;
		Color tintColor;

		TexInfo(FPoint coords = FPoint(-1, -1), FPoint size = FPoint(0, 0), Color tint = Color::White)
		{
			texCoords = coords;
			texSize = size;
			tintColor = tint;
		}
	};

	struct Stats
	{
		public:
			Stats(void)
			{
				HP = 50;
				MP = 100;
				maxHP = 50;
				maxMP = 100;
				speed = 3;
			}

		public:
			uint16 HP;
			uint16 MP;
			float speed;

			int maxHP;
			int maxMP;
	};

#ifdef __DEBUG__
#ifdef __TRACK_GAME_TIMING_INFO__
	struct GameTimes
	{
		public:
			int fps, ups, drawCalls;
			inline GameTimes(void) : fps(0), ups(0), drawCalls(0) {}
	};
#endif
#endif

	class VertexArray;
	class BitmapFont;
	class StringInfo : public Object
	{
		public:
			inline StringInfo(void) { m_string = ""; m_bitmapFont = nullptr; invalidate(); }
			void create(String _string, BitmapFont& font, Color color);
			inline std::vector<TexInfo> getData(void) { return m_data; }
			inline uint32 length(void) { return m_string.length(); }
			inline String getString(void) { return m_string; }
			inline char at(uint32 index) { return m_string[index]; }

		private:
			std::vector<TexInfo> m_data;
			String m_string;
			BitmapFont* m_bitmapFont;
	};

	class StringBuilder
	{
		public:
			inline StringBuilder(void) { m_data = ""; }
			inline StringBuilder(String str) { m_data = str; }
			inline StringBuilder(int i) { m_data = String::intToStr(i); }
			inline StringBuilder(long int i) { m_data = String::intToStr(i); }
			inline StringBuilder(float f) { m_data = String::floatToStr(f); }
			inline StringBuilder(Object& o) { m_data = o.toString(); }
			inline StringBuilder(StringBuilder& sb) { m_data = sb.get(); }
			
			inline StringBuilder& add(String str) { m_data = m_data.add(str); return *this; }
			inline StringBuilder& add(int i) { m_data = m_data.addInt(i); return *this; }
			inline StringBuilder& add(long int i) { m_data = m_data.addInt(i); return *this; }
			inline StringBuilder& add(float f) { m_data = m_data.add(String::floatToStr(f)); return *this; }
			inline StringBuilder& add(Object& o) { m_data = m_data.add(o.toString()); return *this; }
			inline StringBuilder& add(StringBuilder sb) { m_data = sb.get(); return *this; }

			inline String get(void) { return m_data; }

		private:
			String m_data;
	};

	struct RealTime
	{
		public:
			RealTime(void);
			const float& start(void);
			inline const float& getTimeOfDay(void) const { return m_timeOfDay; }
			String asString(void);
			void update(void);

		private:
			String getFormattedTime(void);
			String convertMonth(void);

		public:
			uint8 minutes;
			uint8 hours;
			uint16 days;
			uint8 months;
			uint16 years;

		private:
			sf::Clock m_rtClock;
			float m_timeOfDay;
			float m_totalSeconds;
	};

	struct DataBlock : public Validable
	{
		public:
			DataBlock(void);
			int asInt(void);
			float asFloat(void);
			inline bool asBool(void) { return asInt() == DATABLOCK_TRUE; }
			String asString(void);
			std::vector<int> asIntArray(void);
			std::vector<String> asStringArray(void);
			String asLiteral(void);
			Color asColor(void);

		private:
			inline DataBlock(bool __invalid) { if (__invalid) invalidate(); }

		public:
			eDataBlockType type;
			String name;
			String value;

			static DataBlock Invalid;
	};

	struct DataFuncParamDef
	{
		public:
			String name;
			eDataBlockType type;
			DataFuncParamDef(void) { name = ""; type = eDataBlockType::Invalid; }
			DataFuncParamDef(String n, eDataBlockType t) { name = n; type = t; }
	};

	struct DataFunctionHeader
	{
		public:
			inline DataFunctionHeader(void) { funcName = ""; }
			inline DataFunctionHeader(String name) { funcName = name.toLowerCase().trim(); }
			inline void setName(String name) { funcName = name.toLowerCase().trim(); }
			bool matches(String name, DFD_LIST params);
			static DataBlock& get(String name, DFD_LIST params);
			inline void addParamDef(String name, eDataBlockType type) { paramDefs.push_back(DataFuncParamDef(name.toLowerCase().trim(), type)); }
			inline void addParamDef(DataFuncParamDef dfpd) { paramDefs.push_back(dfpd); }

		private:
			String funcName;
			std::vector<DataFuncParamDef> paramDefs;
	};

	class Camera;
	class PixelRect
	{
		public:
			inline PixelRect(void) { pixels = nullptr; pixelSize = UIPoint(-1, -1); arraySize = -1; }
			inline void setRect(uint32 x, uint32 y, uint32 w, uint32 h) { rect = UIRect(x, y, w, h); }
			inline void setPixelGrid(uint32 width, uint32 height) { pixelSize = UIPoint(width, height); }
			Color at(uint32 x, uint32 y);
			bool intersects(UIRect rect2, Camera& camera);

		public:
			const uint8* pixels;
			UIRect rect;
			UIPoint pixelSize;
			uint32 arraySize;
	};

	class Entity;
	class ICollideCondition
	{
		public:
			inline virtual bool collides(eCollisionLevel cl, Entity& parent) { return false; };
	};

	class Prompt
	{
		public:
			String message;
			String indicator;
			Color msgColor;
			Color indColor;
			Color commandColor;
			Color bracketColor;

			inline Prompt(void) : message(""), indicator(""), bracketColor(Color::Gray), indColor(Color::Blue), msgColor(Color::DarkGreen), commandColor(Color::Green) {  }
			inline Prompt& set(String msg, String ind, Color msgc, Color indc, Color comc = Color::Green, Color brc = Color::Gray)
			{
				message = msg;
				indicator = ind;
				msgColor = msgc;
				indColor = indc;
				commandColor = comc;
				bracketColor = brc;
				return *this;
			}

			static Prompt ErrorPrompt;
			static Prompt LogPrompt;
			static Prompt SystemPrompt;
			static Prompt CommandPrompt;
			static Prompt CmdOutPrompt;
			static Prompt InvisibleTextPrompt;
			static Prompt LineBreakPrompt;
	};

	class Application;
	class GameData;
	class CommandInterpreter //TODO: Move in own file
	{
		public:
			CommandInterpreter(void);
			eCommandResult run(String command, bool flushOut = true);
			inline std::vector<String>& output(void) { return m_out; }
			inline std::vector<Prompt>& outputPrompts(void) { return m_outPrompt; }
			inline int16 outputLength(void) { return (m_out.size() == m_outPrompt.size() ? m_out.size() : 0); }
			inline void flushOutput(void) { m_out.clear(); m_outPrompt.clear(); }

		private:
			inline void addOutput(String out, Prompt prompt) { m_out.push_back(out); m_outPrompt.push_back(prompt); }

		private:
			std::vector<String> m_out;
			std::vector<Prompt> m_outPrompt;
	};
	
	class Entity;
	struct LightSource : public Object
	{
		public:
			inline LightSource(void) { create(FPoint(0, 0), FPoint(0, 0), eLightType::Normal); }
			inline LightSource(float x, float y, float w, float h) { create(FPoint(x, y), FPoint(w, h), eLightType::Normal); }
			inline LightSource(FPoint source, FPoint size) { create(source, size, eLightType::Normal); }

			void create(FPoint source, FPoint size, eLightType type);
			inline bool isOn(void) { return state == eLightState::On; }
			inline void turnOn(void) { state = eLightState::On; }
			inline void turnOff(void) { state = eLightState::Off; }
			inline void setState(bool state) { this->state = (state ? eLightState::On : eLightState::Off); }
			inline void attachToEntity(Entity* e) { attachedEntity = e; }
			void update(void);
			inline uint8 getGlowStep(void) { return m_currentGlowStep; }
			bool isOnScreen(FPoint screenSize);

		public:
			FPoint source;
			FPoint size;
			FRect textureRect;
			eLightType type;
			eLightState state;
			Entity* attachedEntity;
			bool offOnTime;
			bool optimize;
			String name;

		private:
			uint8 m_glowDelay;
			uint8 m_time;
			uint8 m_glowSteps;
			int8 m_currentGlowStep;
			bool m_glowReverse;
			sf::Sprite m_lightColor;
			bool m_colored;
			inline static int s_nextLightID = 0;

		public:
			friend class LightBuilder;
	};

	struct LightTemplate
	{
		public:
			FPoint source;
			FPoint size;
			FRect textureRect;
			eLightType type;
			Entity* parent;
			bool offOnTime;
			int glowDelay;
			int glowSteps;
			Color color;
			bool colored;
			bool optimize;
			int id;

			LightTemplate(void)
			{
				source = FPoint(0, 0);
				size = FPoint(200, 200);
				textureRect = FRect(0, 0, 32, 32);
				type = eLightType::Normal;
				parent = NULL;
				offOnTime = true;
				glowDelay = 20;
				glowSteps = 20;
				color.setColor(0, 255, 0, 255);
				colored = false;
				optimize = false;
				id = -1;
			}
	};
}

#endif
