#include "GuiComponents.hpp"
#include "GameData.hpp"

namespace Omnia
{
    Themes::SubWindowTheme Themes::SubWindowTheme::Default = Themes::SubWindowTheme();

    void GuiComponent::create(GuiComponent& parent, String name)
    {
        m_parent = &parent;
        setID(GuiComponent::s_nextID++);
        if (name.trim() == "")
            m_name = String("Component_").add(String::intToStr(getID()));
        else
            m_name = name;
        setMemberNamespace(m_name);
        setBounds(AUTO__GUI_COMPONENT_BOUNDS);
        setVisible(true);
        setValid(true);
    }

    void GuiComponent::update(GameData& gdata)
    {
        if (isInvalid()) return;
        onUpdate(gdata);
        for (auto& gc : m_components)
            gc->update(gdata);
    }

    void GuiComponent::render(GameData& gdata)
    {
        if (isInvalid()) return;
        onRender(gdata);
        for (auto& gc : m_components)
            gc->render(gdata);
    }

    int32 GuiComponent::addComponent(GuiComponent& component) //TODO: Shared layers?
    {
        if (isInvalid()) return -1;
        auto ptr = &component;
        ptr->setEventParent(ptr);
        m_components.push_back(ptr);
        ptr->m_parent = this;
        return ptr->getID();
    }

    bool GuiComponent::removeComponent(int32 cid)
    {
        if (isInvalid()) return false;
        for (uint16 i = 0; i < m_components.size(); i++)
        {
            if (m_components[i]->getID() == cid)
            {
                m_components[i]->clearParent();
                m_components.erase(m_components.begin() + i);
                return true;
            }
        }
        return false;
    }

    void GuiComponent::setGuiFocusManagerRef(GuiFocusManager& gfm)
    {
        if (isInvalid()) return;
        m_gfm = &gfm;
        for (auto& gc : m_components)
            gc->setGuiFocusManagerRef(gfm);
    }

    bool GuiComponent::requestFocus(bool focus)
    {
        if (isInvalid() || m_gfm == nullptr) return false;
        if (focus)
            return m_gfm->requestFocus(*this);
        unlockEvents();
        m_focused = false;
        componentFocusLost(GameData::instance(), *this);
        return true;
    }

    String GuiComponent::toString(void)
    {
        String str = m_name;
        str.add(" (ID=");
        str.addInt(getID());
        str.add(", LOCK=");
        str.addInt(m_eventLock.getID());
        str.add(") cur_lock=");
        str.addInt(EventLock::s_lockedID);
        return str;
    }

    void Themes::SubWindowTheme::setDefault(void)
    {
        backgroundColor = Color(0, 20, 25, 220);
        borderColor = Color(0, 40, 120, 200);
        titleBarFocusedColor = Color(0, 100, 255, 200);
        titleBarUnfocusedColor = Color(100, 120, 125, 200);
        titleColor = Color(200, 200, 255, 255);
        textColor = Color(0, 180, 255, 255);
        titleBarheight = 25;
        borderWidth = 1;
        titleCharHeight = 20;
        titlePadding = UI8Point(10, 4);
        textCharHeight = 20;
    }
    
    void SubWindow::onRender(GameData& gdata) //TODO: Add custom onRender method
    {
        if (isInvalid()) return;
        if (boxLayer().isInvalid() || textLayer().isInvalid()) return;
        FRect b = clientBounds();
        FRect border = globalBounds();
        border.x -= m_theme.borderWidth;
        border.y -= m_theme.borderWidth;
        border.w += m_theme.borderWidth * 2;
        border.h += m_theme.borderWidth * 2;
        
        boxLayer().get().addQuad(FRect(border.x + m_theme.borderWidth, border.y, globalBounds().w + m_theme.borderWidth, m_theme.borderWidth), m_theme.borderColor);
        boxLayer().get().addQuad(FRect(border.x, border.y, m_theme.borderWidth, globalBounds().h + (m_theme.borderWidth * 2)), m_theme.borderColor);
        boxLayer().get().addQuad(FRect(globalBounds().w + globalBounds().x, border.y + m_theme.borderWidth, m_theme.borderWidth, globalBounds().h + m_theme.borderWidth), m_theme.borderColor);
        boxLayer().get().addQuad(FRect(border.x, globalBounds().y + globalBounds().h, globalBounds().w + m_theme.borderWidth, m_theme.borderWidth), m_theme.borderColor);

        boxLayer().get().addQuad(b, m_theme.backgroundColor);
        boxLayer().get().addQuad(titlebarBounds(), (isFocused() ? m_theme.titleBarFocusedColor : m_theme.titleBarUnfocusedColor));
        
        boxLayer().get().addQuad(FRect(globalX(0), globalY(titlebarBounds().h), getw(), 2), Color(0, 0, 0, 120));
        
        textLayer().renderString(getTitle(), FPoint(globalX(m_theme.titlePadding.x), globalY(m_theme.titlePadding.y)),m_theme.titleColor, m_theme.titleCharHeight);
    }

    void SubWindow::onMouseDragged(sf::Event& evt, GameData& gdata)
    {
        if (isInvalid() || !isVisible()) return;
        if (boxLayer().isInvalid() || textLayer().isInvalid()) return;
        if (getMousePressedCode() != SF_MBTN(Left) || !m_clickedInside)
            return;
        setPosition(evt.mouseMove.x - m_mouseClickDiff.x, evt.mouseMove.y - m_mouseClickDiff.y);
    }

    void SubWindow::onMousePressed(sf::Event& evt, GameData& gdata)
    {
        if (boxLayer().isInvalid() || textLayer().isInvalid() || !isVisible()) return;
        m_mouseClick = IPoint(evt.mouseButton.x, evt.mouseButton.y);
        m_mouseClickDiff = IPoint(evt.mouseButton.x - globalX(), evt.mouseButton.y - globalY());
        m_clickedInside = titlebarBounds().contains(FPoint(m_mouseClick.x, m_mouseClick.y));
    }

    FRect SubWindow::titlebarBounds(void)
    {
        FRect b = globalBounds();
        b.h = m_theme.titleBarheight;
        return b;
    }

    bool GuiFocusManager::requestFocus(GuiComponent& component)
    {
        if (component.isInvalid()) return false;
        bool _return = false;
        for (auto& gc : guiComponentHolder->m_components)
        {
            if (!gc->isMouseInside()) continue;
            if (guiComponentHolder->compare(component, *gc) == eGuiComponentOrderCompare::Under)
                return false;
        }
        if (!hasFocused() || currentFocused->requestFocus(false))
        {
            component.m_focused = true;
            currentFocused = &component;
            if (guiComponentHolder != nullptr)
                guiComponentHolder->arrangeComponents();
            component.componentFocusGained(GameData::instance(), component);
            _return = true;
        }
        #ifdef __DEBUG__
        #ifdef __CREATE_DEFAULT_DBG_VIEW__
            if (dbgPtr != -1)
                GameData::instance().debugView.removePtr(dbgPtr);
            if (currentFocused != nullptr)
                dbgPtr = GameData::instance().debugView.attachPtr(*currentFocused, "Gui_Focus:", GameData::instance().debugView.getTheme().textColor, 20);
        #endif
        #endif
        return _return;
    }

    GuiComponentHolder::GuiComponentHolder(void)
    {
        setValid(true);
        m_font = nullptr;
        m_gfm.guiComponentHolder = this;
    }

    int32 GuiComponentHolder::addComponent(GuiComponent& component)
    {
        if (isInvalid()) return -1;
        component.setValid(true);
        component.setGuiFocusManagerRef(m_gfm);
        component.setEventParent(&component);
        component.m_boxLayer = layers().addLayer(AUTO__LAYER_RENDER_INDEX, true, false);
        if (m_font != nullptr)
            component.m_textLayer = layers().addTextLayer(*m_font, AUTO__LAYER_RENDER_INDEX, true, false);
        else
            component.m_textLayer = layers().addTextLayer(INVALID_REF(BitmapFont), AUTO__LAYER_RENDER_INDEX, true, false);
        
        auto ptr = &component;
        m_components.push_back(ptr);
        ptr->clearParent();
        arrangeComponents();
        if (m_font != nullptr) //TODO: I don't like this
            m_gfm.requestFocus(component); 
        return ptr->getID();
    }

    void GuiComponentHolder::arrangeComponents(void)
    {
        bool hf = m_gfm.hasFocused();
        if (hf)
        {
            GuiComponent* foc = m_gfm.currentFocused;
            for (uint16 i = 0; i < m_components.size(); i++)
            {
                if (m_components[i]->getID() == foc->getID())
                {
                    STDVEC_REMOVE(m_components, i);
                    break;
                }
            }
            m_components.push_back(foc);
        }
    }

    void GuiComponentHolder::render(GameData& gdata)
    {
        if (isInvalid()) return;
        for (auto& gc : m_components)
            gc->render(gdata);
        for (auto& gc : m_components)
        {
            if (gc->isVisible())
            {
                gdata.renderer.drawVertexArray(gc->m_boxLayer.get());
                gdata.renderer.drawVertexArray(gc->m_textLayer.get());
            }
            gc->m_textLayer.get().clear();
            gc->m_boxLayer.get().clear();
        }

    }

    void GuiComponentHolder::update(GameData& gdata)
    {
        if (isInvalid()) return; 
        for (auto& gc : m_components)
            gc->update(gdata);
    }
    
    void GuiComponentHolder::onEvent(sf::Event& evt, GameData& gdata)
    {
        if (isInvalid()) return;
        for (auto& gc : m_components)
            gc->handleEvents(evt, gdata);
    }

    eGuiComponentOrderCompare GuiComponentHolder::compare(GuiComponent& gc1, GuiComponent& gc2)
    {
        if (gc1.getID() == gc2.getID()) return eGuiComponentOrderCompare::Same;
        for (auto& gc : m_components)
        {
            if (gc->getID() == gc1.getID())
                return eGuiComponentOrderCompare::Under;
            if (gc->getID() == gc2.getID())
                return eGuiComponentOrderCompare::Over;
        }
        return eGuiComponentOrderCompare::Invalid;
    }

    void GuiComponentHolder::setFont(BitmapFont& font)
    {
        m_font = &font;
        for (auto& gc : m_components)
            gc->m_textLayer.get().setBitmapFont(font);
    }
}