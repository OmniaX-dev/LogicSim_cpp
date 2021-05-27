#ifndef __LS_DEFINES_HPP__
#define __LS_DEFINES_HPP__

#include "Omnia.hpp"

#define GRID_SIZE_PX 11

#define ls_mainBoard ls::RuntimeState::board
#define ls_cmd ls::RuntimeState::cmd
#define ls_rtdata ls::LSRuntimeData::instance()
#define ls_statusBar ls::RuntimeState::statusBar
#define ls_menuBar ls::RuntimeState::menuBar
#define ls_guiBase ls::RuntimeState::guiBase
#define ls_pinMenu ls::RuntimeState::pinContextMenu
#define ls_runtime (*ls::LSRuntimeData::instance().application)
#define ls_toolBar ls::RuntimeState::mainToolBar
#define ls_sideBar ls::RuntimeState::sideBar

#define CMI_Pin_ClearConnections 10
#define CMI_Pin_Signal 11
#define CMI_Component_Test 12

namespace ls
{
	using namespace Omnia;

	typedef sf::Event LSEvent;
	typedef GameData LSRuntimeData;
	typedef AnimationSheetFile ComponentSheetFile;

	enum class eLSCPinEdge
	{
		Left = 1,
		Right,
		Top,
		Bottom
	};

	enum class eLSCPinType
	{
		Power = 1,
		Ground = 2,
		Input = 3,
		Output = 4
	};

	enum class eLSCSIgnalType
	{
		ActiveLow = 1,
		ActiveHigh = 2
	};

	enum class eLSCLogicOp
	{
		AND = 0,
		OR,
		NOT,
		XOR,

		NAND,
		NOR,
		XNOR,

		O_PARENTH = 125,
		C_PARENTH = 126,
		NONE = 127
	};

	enum class eLSCID
	{
		BasicChip = 0,
		_7SDisplayCC
	};

	enum class eLS7SDSeg
	{
		A = 0,
		B,
		C,
		D,
		E,
		F,
		G,
		DP
	};

	enum class eLSOrientation
	{
		Horizontal = 0,
		Vertical
	};
	
	class FrameDelay
	{
		public:
			inline FrameDelay(void) { m_current = 0; m_time = 10; }
			inline FrameDelay(uint16 delay) { m_current = 0; m_time = delay; }
			inline bool tick(void)
			{
				if (m_current++ >= m_time)
				{
					m_current = 0;
					return true;
				}
				return false;
			}
			inline void setDelay(uint16 delay) { m_time = delay; }
			inline uint16 getDelay(void) { return m_time; }

		private:
			uint16 m_current;
			uint16 m_time;
	};
}

#endif