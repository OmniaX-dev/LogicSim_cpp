#ifndef __LS_DEBUG_CONSOLE_HPP__
#define __LS_DEBUG_CONSOLE_HPP__

#include "LSDefines.hpp"
#include "Drawable.hpp"
#include "Object.hpp"
#include "EventListener.hpp"
#include "LSGraphics.hpp"

namespace ls
{
	enum class eLSDebugConsoleAnim
	{
		Open = 0,
		Close
	};

	class LSDebugConsole : public Drawable, public EventListener, public LSComplexDrawable, public Animator
	{
		public:
			LSDebugConsole(void);
			void create(void);
			void onRender(LSRuntimeData& rtdata);
			void update(LSRuntimeData& rtdata);

			void log(String str, Prompt prompt = Prompt());
			void error(String err);
			void print(String msg);
			void system(String msg);
			void command(String com);
			void lineBreak(String line);

			void pushLine(String line, Prompt prompt);

			void onAnimationUpdate(LSRuntimeData& rtdata);

			void mouseScrolled(LSEvent& evt, LSRuntimeData& rtdata);
			void onEvent(LSEvent& evt, LSRuntimeData& rtdata);

			String getMember(String name, eNamespaceMemberType& outType);
			bool setMember(String name, String value);
			inline bool is_Visible(void) { return m__visible; }
			inline void set_Visible(bool v) { if (v) show(); else hide(); }
			inline float clientX(float offset = 0) { return getx() + 1 + offset; }
			inline float clientY(float offset = 0) { return gety() + 5 + offset; }
			inline bool isReady(void) { return m_ready; }
			void refreshSize(void);
			void initialize(void);
			void show(void);
			void hide(void);

		private:
			std::vector<String> m_buffer;
			std::vector<String> m_preBuffer;
			bool m_ready;
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
			bool m__visible;
			float m_fullY;
			eLSDebugConsoleAnim m_anim;
			uint16 m_textCharHeight;
			float m_borderWidth;

			friend class CommandInterpreter;
	};
}

#endif