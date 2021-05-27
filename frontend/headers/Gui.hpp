#ifndef __LSGUI_HPP__
#define __LSGUI_HPP__

#include "LSDefines.hpp"
#include "Drawable.hpp"
#include "Object.hpp"
#include "LayerRenderer.hpp"
#include "GameData.hpp"
#include "EventListener.hpp"
#include "LSGraphics.hpp"
#include "LSColors.hpp"

namespace ls
{
	typedef void(*LSGuiCallback)(LSRuntimeData&, Object&, Object&);

	namespace gui
	{
		enum class eTooltipAnim
		{
			Show = 0,
			Hide
		};

		class BaseWidget : public Drawable, public EventListener, public Animator, public LSComplexDrawable
		{
			public:
				inline float globx(float offset = 0) { return getx() + offset; }
				inline float globy(float offset = 0) { return gety() + offset; }
				inline void set_id(int32 id) { setID(id); }

		};

		class Tooltip2 : public BaseWidget
		{
			public:
				inline Tooltip2(void) { create(); }
				void create(String text = "");
				void onRender(LSRuntimeData& rtdata);
				void update(LSRuntimeData& rtdata);
				void onAnimationUpdate(LSRuntimeData& rtdata);
				void onAnimationEnd(void);
				inline void setText(String text) { m_text = text; m_updateSize = true; }
				inline String getText(void) { return m_text; }
				void show(String text = "", FPoint position = FPoint(-262144, -262144));
				void hide(void) { m_anim = eTooltipAnim::Hide; restartAnimation(); }

			public:
				String m_text;
				bool m_updateSize;
				FPoint m_padding;
				uint8 m_textSize;
				Color m_borderColor;
				Color m_backgroundColor;
				Color m_textColor;
				float m_borderWidth;
				eTooltipAnim m_anim;
		};

		class Tooltip : public Drawable, public Object
		{
			public:
				inline Tooltip(void) { invalidate(); }
				inline Tooltip(String text, FPoint position) { setup(text, position); }
				void setup(String text, FPoint position);
				void render(LSRuntimeData& rtdata, Layer& textLayer, Layer& geoLayer);
				void update(LSRuntimeData& rtdata, Layer& textLayer);

			public:
				String text;
				FPoint padding;
				float borderWidth;
				float textSize;
		};

		class ContextMenu;
		class ContextMenuItem : public Drawable, public Object, public Animator
		{
			public:
				inline ContextMenuItem(void) { invalidate(); }
				inline ContextMenuItem(int32 id) { setID(id); invalidate(); }
				inline ContextMenuItem(String text, LSGuiCallback callback) { setup(text, callback); }
				void setup(String text, LSGuiCallback callback);
				void render(LSRuntimeData& rtdata, Layer& textLayer, Layer& geoLayer);
				void update(LSRuntimeData& rtdata);
				void onAnimationUpdate(LSRuntimeData& rtdata);

			public:
				String text;
				LSGuiCallback callback;
				uint16 textSize;
				uint8 borderWidth;
				ContextMenu* parent;
				Object* data;
				bool hover;
				float animStep;
				float animCurrentShade;
				bool enabled;
				bool* bool_ptr;
				bool checkbox;
		};

		class ContextMenuTemplate : public Object, public Drawable
		{
			public:
				inline ContextMenuTemplate(void) { menuItemHeight = 25; setValid(true); }
				ContextMenuItem& addMenuItem(String text, LSGuiCallback callback, int32 id = 0, Object& data = Object::Invalid, bool checkbox = false, bool* bool_ptr = nullptr, bool enabled = true);

			public:
				std::vector<ContextMenuItem> menuItems;
				float menuItemHeight;

		};

		class ContextMenu : public BaseWidget
		{
			public:
				ContextMenu(void);
				void mousePressed(LSEvent& evt, LSRuntimeData& rtdata);
				void mouseReleased(LSEvent& evt, LSRuntimeData& rtdata);
				void mouseMoved(LSEvent& evt, LSRuntimeData& rtdata);
				void update(LSRuntimeData& rtdata);
				void onAnimationUpdate(LSRuntimeData& rtdata);
				void onRender(LSRuntimeData& rtdata);
				void setMenuPosition(FPoint pos);
				void show(void);
				void hide(void);
				void display(FPoint position, ContextMenuTemplate& cmt);
				void setMenuTemplate(ContextMenuTemplate& cmt);
				void clearMenuTemplate(void);
				inline bool hasMenuTemplate(void) { return menuTemplate != nullptr && menuTemplate->isValid() && menuTemplate->menuItems.size() > 0; }
				inline ContextMenuTemplate& menu(void) { return (hasMenuTemplate() ? *menuTemplate : INVALID_REF(ContextMenuTemplate)); }
				inline void reset(void) { seth(0); setVisible(false); stopAnimaation(); resetAnimation(); }

			public:
				ContextMenuTemplate* menuTemplate;
				uint8 animNext;
		};

		class Button : public BaseWidget
		{
			public:
				inline Button(void) { create(); }
				inline Button(LSComplexDrawable& base, bool hasText) { create(base, hasText); }
				void create(LSComplexDrawable& base = INVALID_REF(LSComplexDrawable), bool hasText = true);
				void onRender(LSRuntimeData& rtdata);
				void update(LSRuntimeData& rtdata);
				void onAnimationUpdate(LSRuntimeData& rtdata);
				void mousePressed(LSEvent& evt, LSRuntimeData& rtdata);
				void mouseReleased(LSEvent& evt, LSRuntimeData& rtdata);
				void mouseMoved(LSEvent& evt, LSRuntimeData& rtdata);

				inline void setCallbackFunction(LSGuiCallback callback) { m_callback = callback; }
				inline bool hasText(void) { return m_hasText && m_text.length() > 0; }
				inline void showText(bool st) { m_hasText = st; useTextLayer(st); m_updateWidth = true; }
				inline void setText(String text) { m_text = text; m_updateWidth = true; }
				inline String getText(void) { return m_text; }
				inline Animation& getIcon(void) { return m_icon; }
				inline bool isBorderDrawn(void) { return m_drawBorder; }
				inline void drawBorder(bool db) { m_drawBorder = db; }
				inline void setTooltipText(String text = "") { m_tooltip.setText(text); }
				void setIcon(Animation icon);
				void setIconSize(FPoint is);
				inline FPoint getIconSize(void) { return m_iconSize; }
				inline Color getCurrentColor(void) { return (m_hover ? (m_mouseDown ? LSColors::ButtonMouseDown : LSColors::ButtonHover) : LSColors::ButtonIcon); }


			public:
				LSGuiCallback m_callback;
				FPoint m_defaultSize;
				Animation m_icon;
				bool m_hasText;
				String m_text;
				FPoint m_iconSize;
				bool m_updateWidth;
				float m_padding;
				uint8 m_textSize;
				float m_minWidth;
				Color m_borderColor;
				Color m_backgroundColor;
				Color m_textColor;
				float m_borderWidth;
				bool m_drawBorder;
				bool m_hover;
				bool m_mouseDown;
				Object* m_evtData;
				Tooltip2 m_tooltip;
				bool m_autoDrawTooltip;
		};

		class MenuBar : public BaseWidget
		{
			public: class Menu : public Drawable
			{
				public:
					inline Menu(void) { text = ""; hover = false; menu = nullptr; }
					ContextMenuTemplate* menu;
					String text;
					bool hover;
			};
			public:
				inline MenuBar(void) { create(); }
				void create(void);
				void addMenu(String name, ContextMenuTemplate& cmt);
				void onRender(LSRuntimeData& rtdata);
				void update(LSRuntimeData& rtdata);
				void onEvent(LSEvent& evt, LSRuntimeData& rtdata);
				void mouseMoved(LSEvent& evt, LSRuntimeData& rtdata);
				void mousePressed(LSEvent& evt, LSRuntimeData& rtdata);

			public:
				std::vector<Menu> m_menuList;
				float m_borderWidth;
				float m_height;
				uint16 m_textSize;
				float m_nextMenuX;
				Menu* m_hoverMenu;
				bool m_menuVisible;
				
		};

		class ToolBar : public BaseWidget
		{
			public:
				inline ToolBar(void) { create(); }
				void create(String iconsFile = "");
				Button& addButton(LSGuiCallback callback, String iconName = "", bool drawLabel = false, String label = "", int32 id = 0, Object& data = Object::Invalid, bool enabled = true, String tooltip = "");
				inline void addButton(Button btn) { m_buttons.push_back(btn); }

				void onRender(LSRuntimeData& rtdata);
				void update(LSRuntimeData& rtdata);
				void onEvent(LSEvent& evt, LSRuntimeData& rtdata);
				void mouseMoved(LSEvent& evt, LSRuntimeData& rtdata);
				void mousePressed(LSEvent& evt, LSRuntimeData& rtdata);
				

			public:
				std::vector<Button> m_buttons;
				float m_borderWidth;
				float m_height;
				uint16 m_textSize;
				float m_nextBtnX;
				Button* m_hoverBtn;
				AnimationSheetFile iconFile;
				FPoint m_iconSize;
		};

		class SideBar : public BaseWidget
		{
			public:
				inline SideBar(void) { create(); }
				void create(void);

				void onRender(LSRuntimeData& rtdata);
				void update(LSRuntimeData& rtdata);
				void onEvent(LSEvent& evt, LSRuntimeData& rtdata);			

			public:
				float m_borderWidth;
				float m_width;
		};

		class StatusBar : public BaseWidget
		{
			public: struct PtrHolder
			{
				public:
					Object* ptr;
					String text;
					Color color;
					uint16 charHeight;
					inline PtrHolder(void) { ptr = nullptr; }
					inline PtrHolder(Object* pointer) { ptr = pointer; }
			};
			public:
				inline StatusBar(void) { create(); }
				void create(void);
				void onRender(LSRuntimeData& rtdata);
				void update(LSRuntimeData& rtdata);
				uint16 attachPtr(Object& variable, String text = "", Color color = Color::White, uint16 chHeight = 20);
				bool removePtr(uint16 index);
				inline float clientX(float offset = 0) { return getx() + 1 + offset; }
				inline float clientY(float offset = 0) { return gety() + 1 + offset; }
				void onEvent(LSEvent& evt, LSRuntimeData& rtdata);
				void windowResized(LSEvent& evt, LSRuntimeData& rtdata);
				Button& addButton(LSGuiCallback callback, String iconName = "", bool drawLabel = false, String label = "", int32 id = 0, Object& data = Object::Invalid, bool enabled = true, String tooltip = "");

			public:
				float m_borderWidth;
				float m_height;
				float m_separatorWidth;
				std::vector<PtrHolder> m_pointers;
				float m_nextBtnX;
				FPoint m_iconSize;
				ToolBar m_iconBar;
		};
	}
}

#endif