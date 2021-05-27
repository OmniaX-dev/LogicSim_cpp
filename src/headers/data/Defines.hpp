#ifndef __DEFINES__H__
#define __DEFINES__H__

//TODO: Hardcoded __DEBUG__ and __GOD_MODE__ bypass
#ifndef __DEBUG__ 
	#define __DEBUG__
#endif
//-------------------------------------------------


#ifdef __DEBUG__
	#define __TRACK_GAME_TIMING_INFO__
	#define __CREATE_DEFAULT_DBG_VIEW__
	#define __HIDE_SPL_SCR__ 
	#define __CONSOLE_OBSOLETE_HEADER__
#endif

#ifdef _WIN32
	#define WINDOWS_OS
#else
	#define LINUX_OS
#endif

//Basic constants
#if !defined(__BUILD_NUMBER__)
	#define __BUILD_NUMBER__ 0
#endif
#if !defined(NULL)
	#define NULL 0
#endif
#define __INT_CMP_VAL -32768
#define PI 3.1415926535898
#define __INVALID_OBJ true
#define DFD_LIST std::vector<DataBlock>
#define DATABLOCK_FALSE 0
#define DATABLOCK_TRUE 1

//SpriteUnit format
#define __TUINT_PARAM_MASK 1000
#define __TUNIT_PARAM_COUNT 4
#define __TUNIT_DEFAULT_DELAY 14
#define __TUINT_MASK __TUINT_PARAM_MASK * (__TUNIT_PARAM_COUNT - 1)

//Generic directions
#define E_NORTH 0
#define E_SOUTH 1
#define E_EAST 2
#define E_WEST 3

//Default values
#define AUTO__LAYER_RENDER_INDEX -1
#define AUTO__MAX_RENDER_LAYERS 100
#define AUTO__BITMAP_FONT_CHAR_WIDTH 40
#define AUTO__BITMAP_FONT_CHAR_HEIGHT 40
#define AUTO__LAYER_RENDER_STRING_FONT_HEIGHT 255
#define AUTO__MAX_FLAGS 100
#define AUTO__MAX_UPS 120.f
#define AUTO__MAX_FPS 60.f
#define AUTO__MAX_ERRORS 200
#define AUTO__MAX_FONTS 10
#define AUTO__NO_LGT_TMPLT -1
#define AUTO__MAX_CONTROLS_IN_GROUP 10
#define AUTO__CAM_MAX_X 20000
#define AUTO__CAM_MAX_Y 20000
#define AUTO__GUI_COMPONENT_BOUNDS FRect(0, 0, 200, 100)
#define AUTO__EVENTS_UNLOCKED -1

//Namespace constants
#define NS_MEMBER_NOT_FOUND "NS_MNF"
#define NS_GET_METHOD_NOT_OVERLOADED "NS_GMNO"
#define NS_NOT_FOUND "NS_NF"

//Game Time constants
#define TM_R_SECONDS_FOR_G_MINUTE 2
#define TM_G_MINUTES_FOR_G_HOUR 60
#define TM_G_HOURS_FOR_G_DAY 24
#define TM_G_DAYS_FOR_G_LONG_MONTH 31
#define TM_G_DAYS_FOR_G_MEDIUM_MONTH 30
#define TM_G_DAYS_FOR_G_SHORT_MONTH 28

//Flag constants
#define FLG_IGNORE_NEXT_ACTIVE_STATE_CONTROL 0
#define FLG_AUTO_DUMP_ERROR_QUEUE_WHEN_FULL 1
#define FLG_AUTO_DUMP_EVERY_ERROR 2
#define FLG_LONG_MESSAGE_ON_ERROR_DUMP 3
#define FLG_NO_MESSAGE_ON_ERROR_DUMP 4
#define FLG_SHOW_SPLASH_SCREEN 5
#define FLG_DEBUG 6
#define FLG_SKIP_NEXT_EVENT_ROUND 7
#define FLG_SHOW_COLLISION_RECTS 8
#define FLG_SHOW_DEBUG_VIEW 9
#define FLG_EVENTS_LOCKED 10

//Macro functions
#define ERROR_DATA() String(CPP_STR(__LINE__)), String(__FILE__)
#define CPP_STR(val) String::intToStr(val).cpp()
#define STR_BOOL(b) (b ? "true" : "false")
#define INT_BOOL(i) (i == 0 ? false : true)
#define ZERO(n) (n > 0 ? n : 0)
#define FRAND() ((float)(rand() % 10000)) / 10000.0f
#define RANDOM(min, max) rand() % (max - min + 1) + min
#define LERP(n1, n2, f) (n2 - n1) * f + n1
#define CAP(n, max) (n > max ? max : n)
#define CAPD(n, min) (n < min ? min : n)
#define CAPB(n, min, max) (n < min ? min : (n > max ? max : n))
#define MAX(n1, n2) (n1 > n2 ? n1 : n2)
#define MIN(n1, n2) (n1 < n2 ? n1 : n2)
#define INVALID_OBJ(deriv) *((deriv*)(&Object::Invalid))
#define INVALID_REF(deriv) ((deriv&)(Object::Invalid))
#define CONVERT_1D_2D(i, width) IPoint(i % width, i / width)
#define CONVERT_2D_1D(x, y, width) (x + width * y)
#define PROPORTION(w, x, y) ((x * w) / y)
#define SF_KEY(key) sf::Keyboard::key
#define SF_MBTN(btn) sf::Mouse::btn
#define PRINT(data) std::cout << data
#define PRINTLN(data) std::cout << data << "\n"
#define NEWLN() std::cout << "\n"
#define GET_PARAM(param, dfd_list) DataFunctionHeader::get(param, dfd_list)
#define EVT_LOCK_CHECK if (Flags::isset(FLG_EVENTS_LOCKED) && !isLockedEvent()) return
#define STDVEC_REMOVE(vec, index) vec.erase(vec.begin() + index)

//Memory management macros
#define new_sh(type) std::make_shared<type>
#define sh_ptr(type) std::shared_ptr<type>
#define new_un(type) std::make_unique<type>
#define un_ptr(type) std::unique_ptr<type>

#endif
