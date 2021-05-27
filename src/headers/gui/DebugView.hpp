#ifdef __DEBUG__
#ifndef __DEBUGVIEW__HPP__
#define __DEBUGVIEW__HPP__

#include "DataTypes.hpp"
#include "Drawable.hpp"
#include "EventListener.hpp"
#include "LayerRenderer.hpp"
#include "BitmapFont.hpp"
#include "GuiComponents.hpp"

namespace Omnia
{
	class Application;
	class GameData;
	class RichString : public Object, public Drawable
	{
		public:
			inline RichString(void) { ptr = nullptr; color = Color::Black; charHeight = 20; setValid(false); } //TODO: Add AUTO__ macro for default debug charHeight
			inline RichString(Object& obj, String t = "", Color col = Color::Black, uint16 chHeight = 20) { ptr = &obj; text = t; color = col;  charHeight = chHeight; setValid(true); }
			inline virtual String toString(void) { return (ptr == nullptr ? "Invalid" : ptr->toString()); }
			void render(GameData& gdata, Layer& layer);
			void update(GameData& gdata, BitmapFont& font);

		private:
			String createString(void);

		public:
			Object* ptr;
			String text;
			Color color;
			uint16 charHeight;
	};
	class DebugView;
	class Tooltip : public Object, public Drawable
	{
		public:
			Tooltip(bool atMousePos = true);
			void render(GameData& gdata);
			void update(GameData& gdata, FPoint position = FPoint(-1, -1));
			inline uint16 addPtr(Object& ptr, String text = "", Color color = Color::Black, uint16 charHeight = 20) { return addPtr(RichString(ptr, text, color, charHeight)); }
			inline uint16 addPtr(RichString rstr) { m_lines.push_back(rstr); return m_lines.size() - 1; }
			inline void setBackgroundColor(Color color) { m_bgColor = color; }
			inline Color getBackgroundColor(void) { return m_bgColor; }
			bool removePtr(uint16 index);
			inline bool attachedToMouse(void) { return m_atMousePosition; }
			inline void setAttachedToMouse(bool atm) { m_atMousePosition = atm; }
			inline bool isVisible(void) { return m_visible; }
			inline void setVisible(bool visible) { m_visible = visible; }
			inline I8Rect getPadding(void) { return m_padding; }
			inline void setPadding(I8Rect pad) { m_padding = pad; }
			inline void setOnKeyPress(bool okp, sf::Keyboard::Key key = SF_KEY(LControl)) { m_onKeyPress = okp; m_key = key; }
			inline bool isOnKeyPress(void) { return m_onKeyPress; }
			inline bool isKey(sf::Keyboard::Key key) { return (isOnKeyPress() ? key == m_key : true); }

		private:
			std::vector<RichString> m_lines;
			DebugView* m_dbgView;
			Color m_bgColor;
			bool m_atMousePosition;
			bool m_visible;
			bool m_onKeyPress;
			sf::Keyboard::Key m_key;
			I8Rect m_padding;
	};

	class DebugView : public SubWindow
	{
		private: struct PtrHolder
		{
			public:
				Object* ptr;
				String text;
				Color color;
				uint16 charHeight;
				inline PtrHolder(void) { ptr = NULL; }
				inline PtrHolder(Object* pointer) { ptr = pointer; }
		};
		public:
			DebugView(void);
			void create(void);
			uint16 attachPtr(Object& variable, String text = "", Color color = Color::White, uint16 chHeight = 20, bool asTooltip = false, bool atMousePosition = true); //TODO: add AUTO__
			bool removePtr(uint16 index);
			bool removeTooltp(uint16 index);
			void onRender(GameData& gdata);
			void onUpdate(GameData& gdata);
			inline uint16 getLineSpacing(void) { return m_lineSpace; }
			inline void setLineSpacing(uint16 ls) { m_lineSpace = ls; }

		private:
			void recalcBounds(void);

		private:
			std::vector<PtrHolder> m_pointers;
			std::vector<Tooltip> m_tooltips;
			uint16 m_nextTooltipYOffset;
			UIPoint m_textSize;
			uint16 m_lineSpace;
			UI16Point m_padding;

		#ifdef __GOD_MODE__
			int8 gmt;
			int8 gmd;
			bool gms;
		#endif

			friend class Application;
			friend class Tooltip;
	};

	class DebugConsole : public SubWindow
	{
		public:
			DebugConsole(void);
			void create(void);
			void onRender(GameData& gdata);
			void onUpdate(GameData& gdata);

			void log(String str, Prompt prompt = Prompt());
			void error(String err);
			void print(String msg);
			void system(String msg);
			void command(String com);

			void onMouseScrolled(sf::Event& evt, GameData& gdata);
			void onEvent(sf::Event& evt, GameData& gdata);

			String getMember(String name, eNamespaceMemberType& outType);
			bool setMember(String name, String value);

		private:
			std::vector<String> m_buffer;
			uint8 m_linesOnScreen;
			int8 m_textFiledHeight;
			uint32 m_currentScroll;
			String m_currentCommand;
			uint16 m_cursorPosition;
			uint8 m_cursorWidth;
			uint8 m_cursorDelay;
			uint8 m_cursorCurrent;
			bool m_cursorState;
			Color m_cursor2ndColor;
			std::vector<Prompt> m_prompts;
			std::vector<String> m_cmdHistory;
			int16 m_currCmdHistory;

			friend class CommandInterpreter;
	};
}

#endif
#endif