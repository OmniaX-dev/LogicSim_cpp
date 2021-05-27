#ifndef __LS_COLORS_HPP__
#define __LS_COLORS_HPP__

#include "LSDefines.hpp"

namespace ls
{
	class LSColors
	{
		public:
			inline static Color Background = Color(0, 10, 15);
			inline static Color Grid = Color(0, 50, 80, 80);
			inline static Color ComponentImage = Color(70, 70, 40, 210);
			inline static Color ComponentBorder = Color(35, 35, 35, 210);
			inline static Color ComponentBorder_sel = Color(0, 40, 140, 220);
			inline static Color GridHighlight = Color(35, 0, 80, 80);
			inline static Color ComponentBackground = Color(0, 0, 0, 180);
			inline static Color LSCPowerPin = Color(150, 0, 0, 210);
			inline static Color LSCGroundPin = Color(140, 70, 0, 210);
			inline static Color LSCOutputPin = Color(90, 90, 90, 210);
			inline static Color LSCInputPin = Color(140, 140, 140, 210);
			inline static Color LSCPin_sel = Color(0, 255, 100, 50);
			inline static Color DefaultWire = Color(10, 150, 120, 210);
			inline static Color DefaultWire_high = Color(0, 180, 10, 210);
			inline static Color LS7SDisplaySegment_low = Color(30, 0, 0, 210);
			inline static Color LS7SDisplaySegment_high = Color(210, 0, 0, 210);
			//inline static Color WireCreation = Color(255, 0, 0, 210);
			inline static Color ComponentName = Color(100, 100, 100, 210);

			inline static Color TooltipBackground = Color(20, 20, 20, 240);
			inline static Color TooltipText = Color(210, 210, 210, 210);
			inline static Color TooltipBorder = Color(10, 40, 210, 210);

			inline static Color ContextMenuItemBorder = Color(30, 30, 30, 230);
			inline static Color ContextMenuItemBackground = Color(10, 10, 10, 245);
			inline static Color ContextMenuItemText = Color(200, 200, 200, 210);
			inline static Color ContextMenuItemHover = Color(50, 140, 230, 210);
			inline static Color ContextMenuItemDisabled = Color(50, 50, 100, 210);
			inline static Color ContextMenuItemCheckBorder = Color(30, 30, 255, 230);
			inline static Color ContextMenuItemCheckFill = Color(40, 100, 255, 230);

			inline static Color DebugConsoleBackground = Color(10, 10, 10, 230);
			inline static Color DebugConsoleBorder = Color(80, 80, 120, 230);
			inline static Color DebugConsoleText = Color(10, 230, 50, 230);

			inline static Color StatusBarBackground = Color(10, 10, 10, 250);
			inline static Color StatusBarBorder = Color(80, 80, 120, 230);
			inline static Color StatusBarText = Color(160, 160, 255, 230);
			inline static Color StatusBarSeparator = Color(40, 40, 40, 230);
			inline static Color StatusBarButtonBorder = Color(80, 80, 150, 80);

			inline static Color MenuBarBackground = Color(10, 10, 10, 250);
			inline static Color MenuBarBorder = Color(80, 80, 120, 230);
			inline static Color MenuBarText = Color(160, 160, 255, 230);
			inline static Color MenuBarHover = Color(50, 140, 230, 210);

			inline static Color ButtonBackground = Color(10, 10, 10, 250);
			inline static Color ButtonBorder = Color(80, 80, 120, 230);
			inline static Color ButtonText = Color(160, 160, 255, 230);
			inline static Color ButtonHover = Color(220, 220, 255, 210);
			inline static Color ButtonMouseDown = Color(40, 40, 200, 210);
			inline static Color ButtonIcon = Color(50, 140, 230, 210);
			inline static Color ButtonBackgroundHover = Color(50, 50, 80, 210);

			inline static Color ToolBarBackground = Color(10, 10, 10, 250);
			inline static Color ToolBarBorder = Color(80, 80, 120, 230);
			inline static Color ToolBarText = Color(160, 160, 255, 230);
			inline static Color ToolBarHover = Color(50, 140, 230, 210);

			inline static Color SideBarBackground = Color(10, 10, 10, 250);
			inline static Color SideBarBorder = Color(80, 80, 120, 230);
			inline static Color SideBarText = Color(160, 160, 255, 230);
			inline static Color SideBarHover = Color(50, 140, 230, 210);
	};
}

#endif