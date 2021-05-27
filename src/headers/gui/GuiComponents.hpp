#ifndef __GUI_COMPONENTS__HPP__
#define __GUI_COMPONENTS__HPP__

#include "DataTypes.hpp"
#include "Drawable.hpp"
#include "EventListener.hpp"
#include "LayerRenderer.hpp"

namespace Omnia
{
    class GuiFocusManager;
    class GameData;
    class GuiComponent : public Object, public GuiEventListener
    {
        public:
            inline GuiComponent(void) : GuiEventListener() { create(INVALID_REF(GuiComponent)); }
            inline GuiComponent(GuiComponent& parent, String name = "") : GuiEventListener() { create(parent, name); }
            void create(GuiComponent& parent, String name = "");
            int32 addComponent(GuiComponent& component);
            bool removeComponent(int32 cid);
            inline void clearParent(void) { m_parent = &(INVALID_REF(GuiComponent)); }
            inline GuiComponent& getParent(void) { return *m_parent; }
            inline bool isVisible(void) { return m_visible; }
            inline void setVisible(bool visible) { m_visible = visible; }
            inline float globalX(float offset = 0) { return getx() + (getParent().isValid() ? getParent().globalX() : offset); }
            inline float globalY(float offset = 0) { return gety() + (getParent().isValid() ? getParent().globalY() : offset); }
            inline FRect globalBounds(void) { return FRect(globalX(), globalY(), getw(), geth()); }
            bool requestFocus(bool focus = true);
            virtual String toString(void);
            inline void setName(String name) { m_name = name; }
            inline String getName(void) { return m_name; }
            inline Layer& textLayer(void) { return m_textLayer; }
            inline Layer& boxLayer(void) { return m_boxLayer; }
            void update(GameData& gdata);
            void render(GameData& gdata);

        protected:
            inline virtual void onRender(GameData& gdata) {  }
            inline virtual void onUpdate(GameData& gdata) {  }
            inline void setParent(GuiComponent& parent) { m_parent = &parent; }
            void setGuiFocusManagerRef(GuiFocusManager& gfm);

        protected:
            GuiComponent* m_parent;
            std::vector<GuiComponent*> m_components;
            bool m_visible;
            String m_name;
            Layer m_boxLayer;
            Layer m_textLayer;


        protected:
            inline static int32 s_nextID = 0;

            friend class GuiComponentHolder;
            friend class GuiFocusManager;
            friend class GuiEventListener;
    };

    class Themes
    {
        public:
            class SubWindowTheme
            {
                public:
                    Color backgroundColor;
                    Color titleBarFocusedColor;
                    Color titleBarUnfocusedColor;
                    Color titleColor;
                    Color textColor;
                    Color borderColor;
                    uint8 titleBarheight;
                    uint8 borderWidth;
                    uint8 titleCharHeight;
                    uint8 textCharHeight;
                    UI8Point titlePadding;
                    inline SubWindowTheme(void) { setDefault(); }
                    void setDefault(void);
                    static SubWindowTheme Default;
            };
    };

    class SubWindow : public GuiComponent
    {
        public:
            inline SubWindow(void) : GuiComponent(INVALID_REF(GuiComponent)), m_theme(Themes::SubWindowTheme::Default), m_clickedInside(false) { m_title = String("Window_").addInt(getID()); setValid(false); m_theme.setDefault(); }
            virtual void onRender(GameData& gdata);
            inline virtual void onUpdate(GameData& gdata) {  }
            virtual void onMouseDragged(sf::Event& evt, GameData& gdata);
            virtual void onMousePressed(sf::Event& evt, GameData& gdata);
            inline float clientX(float offset = 0) { return globalX() + offset; }
            inline float clientY(float offset = 0) { return globalY(offset) + m_theme.titleBarheight; }
            inline FRect clientBounds(void) { return FRect(clientX(), clientY(), getw(), geth() - m_theme.titleBarheight); }
            inline String getTitle(void) { return m_title; }
            inline void setTitle(String title) { m_title = title; }
            inline Themes::SubWindowTheme& getTheme(void) { return m_theme; }
            inline Themes::SubWindowTheme getThemeCopy(void) { return m_theme; }
            inline void setTheme(Themes::SubWindowTheme theme) { m_theme = theme; }

        protected:
            FRect titlebarBounds(void);

        private:
			IPoint m_mouseClickDiff;
			IPoint m_mouseClick;
			bool m_clickedInside;
            String m_title;
            Themes::SubWindowTheme m_theme;

            friend class GuiComponentHolder;
    };

    class GuiComponentHolder;
    class GuiFocusManager
    {
        public:
            GuiComponent* currentFocused;
            GuiComponentHolder* guiComponentHolder;
		#ifdef __DEBUG__
		#ifdef __CREATE_DEFAULT_DBG_VIEW__
            uint16 dbgPtr;
		#endif
		#endif

		#ifdef __DEBUG__
		#ifdef __CREATE_DEFAULT_DBG_VIEW__
            inline GuiFocusManager(void) { dbgPtr = -1; currentFocused = nullptr; guiComponentHolder = nullptr; }
        #else
            inline GuiFocusManager(void) { currentFocused = nullptr; guiComponentHolder = nullptr; }
		#endif
		#endif
            bool requestFocus(GuiComponent& component);
            inline bool hasFocused(void)
            {
                return currentFocused != nullptr;
            }
    };

    class BitmapFont;
    class GuiComponentHolder : public Object, public Drawable, public EventListener
    {
        public:
            GuiComponentHolder(void);
            int32 addComponent(GuiComponent& component);
            //TODO: Implement Remove component

            void render(GameData& gdata);
            void update(GameData& gdata);
            virtual void onEvent(sf::Event& evt, GameData& gdata);
            eGuiComponentOrderCompare compare(GuiComponent& gc1, GuiComponent& gc2);
            void setFont(BitmapFont& font);
            inline LayerRenderer& layers(void) { return m_layers; }
            

        private:
            void arrangeComponents(void);

        private:
            std::vector<GuiComponent*> m_components;
            GuiFocusManager m_gfm;
            BitmapFont* m_font;
            LayerRenderer m_layers;

            friend class GuiFocusManager;
            friend class GuiComponent;
    };


}

#endif