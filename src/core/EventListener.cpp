#include "EventListener.hpp"
#include "GuiComponents.hpp"

namespace Omnia
{
    EventListener::EventLock::EventLock(void) 
    {
        setID(EventLock::s_nextID++);
    }

    bool EventListener::EventLock::lock(void)
    {
        if (Flags::isset(FLG_EVENTS_LOCKED) || EventLock::s_lockedID != AUTO__EVENTS_UNLOCKED)
            return false;
        Flags::set(FLG_EVENTS_LOCKED);
        EventLock::s_lockedID = getID();
        return true;
    }

    bool EventListener::EventLock::unlock(void)
    {
        if (Flags::isunset(FLG_EVENTS_LOCKED))
            return false;
        if (EventLock::s_lockedID != getID())
            return false;
        Flags::unset(FLG_EVENTS_LOCKED);
        EventLock::s_lockedID = -1;
        return true;
    }

    bool EventListener::EventLock::isLocked(void)
    {
        return (EventLock::s_lockedID == getID() && Flags::isset(FLG_EVENTS_LOCKED));
    }

    bool GuiEventListener::isMouseInside(void)
    {
        if (m_evtParent == nullptr)
            return false;
        I16Point mp = getMousePosition();
        I16Rect bounds = (I16Rect)m_evtParent->getBounds();
        if (bounds.contains(mp))
            return true;
        return false;
    }

    //Keyboard events
    void GuiEventListener::keyPressed(sf::Event& evt, GameData& gdata) { onKeyPressed(evt, gdata); }
    void GuiEventListener::keyReleased(sf::Event& evt, GameData& gdata) { onKeyReleased(evt, gdata); }

    //Mouse events
    void GuiEventListener::mousePressed(sf::Event& evt, GameData& gdata)
    {
        if (!isMouseInside() || m_gfm == nullptr) return;
        if (Flags::isset(FLG_EVENTS_LOCKED) && !isLockedEvent())
        {
            GuiComponent* foc = m_gfm->currentFocused;
            if (!foc->isMouseInside())
            {
                if (isFocusOnClick())
                {
                    m_gfm->requestFocus(*m_evtParent);
                    lockEvents();
                }
            }
            return;
        }
        if (isFocusOnClick() && !isFocused() && m_gfm != nullptr)
        {
            m_gfm->requestFocus(*m_evtParent);
            lockEvents();
        }
        onMousePressed(evt, gdata);
    }
    void GuiEventListener::mouseReleased(sf::Event& evt, GameData& gdata)
    {
        if (!isMouseInside() || !isFocused()) return;
        onMouseReleased(evt, gdata);
    }
    void GuiEventListener::mouseMoved(sf::Event& evt, GameData& gdata)
    {
        if (!isMouseInside() || !isFocused()) return;
        onMouseMoved(evt, gdata);
    }
    void GuiEventListener::mouseDragged(sf::Event& evt, GameData& gdata)
    {
        if (!isMouseInside() || !isFocused()) return;
        onMouseDragged(evt, gdata);
    }
    void GuiEventListener::mouseScrolled(sf::Event& evt, GameData& gdata)
    {
        if (!isMouseInside() || !isFocused()) return;
        onMouseScrolled(evt, gdata);
    }
    void GuiEventListener::textEntered(sf::Event& evt, GameData& gdata)
    {
        if (!isFocused()) return;
        onTextEntered(evt, gdata);
    }
    void GuiEventListener::mouseEntered(sf::Event& evt, GameData& gdata) { onMouseEntered(evt, gdata); }
    void GuiEventListener::mouseLeft(sf::Event& evt, GameData& gdata) { onMouseLeft(evt, gdata); }
}