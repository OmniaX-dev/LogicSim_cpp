#ifndef __RUNTIME_HPP__
#define __RUNTINE_HPP__

#include "LSBoard.hpp"
#include "LSDebugConsole.hpp"
#include "Gui.hpp"

namespace ls
{
	class RuntimeState : public GameState
	{
		public:
			void onInit(LSRuntimeData& rtdata);
			void mouseDragged(LSEvent& evt, LSRuntimeData& rtdata);
			void onAnyEvent(LSEvent& evt, LSRuntimeData& rtdata);
			void onRender(LSRuntimeData& rtdata);
			void onUpdate(LSRuntimeData& rtdata);
			void onActivate(LSRuntimeData& rtdata);
			void keyPressed(LSEvent& evt, LSRuntimeData& rtdata);
			void windowResized(sf::Event& evt, GameData& gdata);

			inline static bool isReady(void) { return RuntimeState::m_ready; }
			
			static void test_btn_callback(LSRuntimeData& rtdata, Object& sender, Object& data);
			
			static void sb_terminal_btn_callback(LSRuntimeData& rtdata, Object& sender, Object& data);

		public:
			inline static LSBoard board;
			inline static LSDebugConsole cmd;
			inline static gui::StatusBar statusBar;
			inline static gui::MenuBar menuBar;
			inline static LSComplexDrawable guiBase;
			inline static gui::ContextMenu pinContextMenu;
			inline static gui::ToolBar mainToolBar;
			inline static gui::SideBar sideBar;

			gui::ContextMenuTemplate file;
			gui::ContextMenuTemplate edit;
			gui::ContextMenuTemplate component;
			gui::ContextMenuTemplate help;

		private:
			inline static bool m_ready = false;

	};

	class LogicSimApp : public Application
	{
		public:
			inline LogicSimApp(void) { rtsID = -1; }
			void onInit(void);

		private:
			RuntimeState rts;
			int rtsID;
	};

	class Utils
	{
		public:
			static String zeroPad(String base, uint16 newLen, char c = ' ');
	};
}

#endif