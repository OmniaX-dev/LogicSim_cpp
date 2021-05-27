#include "Gui.hpp"
#include "LSColors.hpp"
#include "Runtime.hpp"

namespace ls
{
	namespace gui
	{
		using namespace Omnia;

		void Tooltip2::create(String text)
		{
			m_text = text;
			m_padding = FPoint(10, 2);
			m_textSize = 20;
			m_updateSize = true;
			m_borderWidth = 1;
			m_borderColor = LSColors::TooltipBorder;
			m_textColor = LSColors::TooltipText;
			m_backgroundColor = LSColors::TooltipBackground;
			m_anim = eTooltipAnim::Show;
			useGfxLayer(false);
			setAnimationLength(30);
			setVisible(false);
			setValid(true);
		}

		void Tooltip2::onRender(LSRuntimeData& rtdata)
		{
			if (isInvalid() || m_text.trim() == "") return;
			geometryLayer().addOutlinedQuad(getBounds(), m_backgroundColor, m_borderColor, m_borderWidth);
			textLayer().renderString(m_text, FPoint(getx() + m_padding.x, gety() + m_padding.y), m_textColor, m_textSize);
		}
		
		void Tooltip2::update(LSRuntimeData& rtdata)
		{
			if (isInvalid() || m_text.trim() == "" || textLayer().isInvalid()) return;
			if (m_updateSize)
			{
				IPoint lb = textLayer().get().getBitmapFont().getStringBounds(m_text, m_textSize);
				setSize(lb.x + (m_padding.x * 2), lb.y + (m_padding.y * 2));
				m_updateSize = false;
			}
			updateAnimation(rtdata);
		}
		
		void Tooltip2::onAnimationUpdate(LSRuntimeData& rtdata)
		{
			if (m_anim == eTooltipAnim::Show)
			{
				m_textColor.setAlphaPercent(m_textColor.getAlphaPercent() + (1.0f / (float)(getAnimationLength())));
				m_borderColor.setAlphaPercent(m_borderColor.getAlphaPercent() + (1.0f / (float)(getAnimationLength())));
				m_backgroundColor.setAlphaPercent(m_backgroundColor.getAlphaPercent() + (1.0f / (float)(getAnimationLength())));
			}
			else if (m_anim == eTooltipAnim::Hide)
			{
				m_textColor.setAlphaPercent(m_textColor.getAlphaPercent() - (1.0f / (float)(getAnimationLength())));
				m_borderColor.setAlphaPercent(m_borderColor.getAlphaPercent() - (1.0f / (float)(getAnimationLength())));
				m_backgroundColor.setAlphaPercent(m_backgroundColor.getAlphaPercent() - (1.0f / (float)(getAnimationLength())));
			}
		}

		void Tooltip2::onAnimationEnd(void)
		{
			if (m_anim == eTooltipAnim::Hide)
				setVisible(false);
		}

		void Tooltip2::show(String text, FPoint position)
		{
			if (text.trim() != "") m_text = text;
			if (position.x != 262144 && position.y != 262144) setPosition(position);
			if (m_text.trim() == "") return;
			m_anim = eTooltipAnim::Show;
			setVisible(true);
			restartAnimation();
		}


		void Tooltip::setup(String text, FPoint position)
		{
			setPosition(position);
			this->text = text;
			padding = FPoint(10, 2);
			borderWidth = 1;
			textSize = 20;
			setValid(true);
		}

		void Tooltip::update(LSRuntimeData& rtdata, Layer& textLayer)
		{
			if (isInvalid()) return;
			IPoint lb = textLayer.get().getBitmapFont().getStringBounds(text, textSize);
			setSize(lb.x + (padding.x * 2), lb.y + (padding.y * 2));
		}

		void Tooltip::render(LSRuntimeData& rtdata, Layer& textLayer, Layer& geoLayer)
		{
			if (isInvalid()) return;
			float bugOffset = 4;
			geoLayer.get().addQuad(FRect(getx() - borderWidth, gety() - borderWidth, getw() + (borderWidth * 2), geth() + (borderWidth * 2)), LSColors::TooltipBorder);
			geoLayer.get().addQuad(getBounds(), LSColors::TooltipBackground);
			textLayer.renderString(text, FPoint(getx() + padding.x + bugOffset, gety() + padding.y), LSColors::TooltipText, textSize);
		}




		void ContextMenuItem::setup(String text, LSGuiCallback callback)
		{
			this->text = text;
			this->callback = callback;
			textSize = 18;
			borderWidth = 1;
			this->parent = nullptr;
			hover = false;
			data = &(Object::Invalid);
			setAnimationLength(10);
			animStep = getw() / getAnimationLength();
			animCurrentShade = 0.0f;
			bool_ptr = nullptr;
			enabled = true;
			checkbox = false;
			setValid(true);
		}

		void ContextMenuItem::render(LSRuntimeData& rtdata, Layer& textLayer, Layer& geoLayer)
		{
			if (isInvalid()) return;
			geoLayer.addOutlinedQuad(getBounds(), LSColors::ContextMenuItemBackground.getShaded(animCurrentShade), (hover ? LSColors::ContextMenuItemHover.getShaded(animCurrentShade) : LSColors::ContextMenuItemBorder.getShaded(animCurrentShade)), borderWidth);
			if (checkbox && bool_ptr != nullptr)
			{
				geoLayer.addQuad(FRect(getx() + 6, gety() + 6, geth() - 12, geth() - 12), LSColors::ContextMenuItemCheckBorder.getShaded(animCurrentShade), false, 1);
				if (*bool_ptr)
					geoLayer.addQuad(FRect(getx() + 8, gety() + 8, geth() - 15, geth() - 15), LSColors::ContextMenuItemCheckFill.getShaded(animCurrentShade), true);
			}
			IPoint tb = textLayer.get().getBitmapFont().getStringBounds(text, textSize);
			if (enabled)
			{
				if (!isAnimating())
					textLayer.renderString(text, FPoint(getx() + geth() + 3, gety() + (geth() / 2) - (tb.y / 2)), (hover ? LSColors::ContextMenuItemHover.getShaded(animCurrentShade) : LSColors::ContextMenuItemText.getShaded(animCurrentShade)), textSize);
			}
			else
			{
				if (!isAnimating())
					textLayer.renderString(text, FPoint(getx() + geth() + 3, gety() + (geth() / 2) - (tb.y / 2)), LSColors::ContextMenuItemDisabled.getShaded(animCurrentShade), textSize);
			}
		}

		void ContextMenuItem::update(LSRuntimeData& rtdata)
		{
			if (isInvalid()) return;
			updateAnimation(rtdata);
		}

		void ContextMenuItem::onAnimationUpdate(LSRuntimeData& rtdata)
		{
			if (isInvalid()) return;
			addw(animStep);
			animCurrentShade = CAP(animCurrentShade + (1.0f / getAnimationLength()), 1.0f);
		}



		ContextMenuItem& ContextMenuTemplate::addMenuItem(String text, LSGuiCallback callback, int32 id, Object& data, bool checkbox, bool* bool_ptr, bool enabled)
		{
			menuItems.push_back(ContextMenuItem(id));
			ContextMenuItem& cmi = menuItems[menuItems.size() - 1];
			cmi.setup(text, callback);
			cmi.checkbox = checkbox;
			cmi.bool_ptr = bool_ptr;
			cmi.enabled = enabled;
			cmi.data = &data;
			cmi.setx(getx());
			cmi.sety(gety() + ((menuItemHeight + cmi.borderWidth) * (menuItems.size() - 1)));
			cmi.setSize(getw(), menuItemHeight);
			return cmi;
		}



		ContextMenu::ContextMenu(void)
		{
			menuTemplate = nullptr;
			animNext = 0;
			setw(180);
			useGfxLayer(false);
			setVisible(false);
			setAnimationLength(30);
		}

		void ContextMenu::setMenuTemplate(ContextMenuTemplate& cmt)
		{
			menuTemplate = &cmt;
			float h = 0, w = 0;
			for (auto& mi : menuTemplate->menuItems)
			{
				h += mi.geth();
				w = mi.getw();
				mi.parent = this;
			}
			seth(h);
		}

		void ContextMenu::clearMenuTemplate(void)
		{
			menuTemplate = nullptr;
			for (auto& mi : menuTemplate->menuItems)
				mi.parent = nullptr;
		}

		void ContextMenu::setMenuPosition(FPoint pos)
		{
			if (!hasMenuTemplate()) return;
			setPosition(pos);
			uint16 i = 0;
			for (auto& mi : menu().menuItems)
			{
				mi.setSize(getw(), menu().menuItemHeight);
				mi.setPosition(globx(), globy((menu().menuItemHeight + mi.borderWidth) * i++));
			}
		}

		void ContextMenu::mousePressed(LSEvent& evt, LSRuntimeData& rtdata)
		{
			if (!isVisible() || !hasMenuTemplate()) return;
			if (!contains(getMousePosition()))
				hide();
		}

		void ContextMenu::mouseReleased(LSEvent& evt, LSRuntimeData& rtdata)
		{
			if (!isVisible() || !hasMenuTemplate()) return;
			for (auto& mi : menu().menuItems)
			{
				if (mi.contains(getMousePosition()) && mi.enabled)
				{
					if (mi.checkbox && mi.bool_ptr != nullptr) *mi.bool_ptr = !(*mi.bool_ptr);
					else if (mi.callback != nullptr) mi.callback(rtdata, mi, *mi.data);
					mi.hover = false;
					hide();
					return;
				}
			}
		}

		void ContextMenu::mouseMoved(LSEvent& evt, LSRuntimeData& rtdata)
		{
			if (!isVisible() || !hasMenuTemplate()) return;
			for (auto& mi : menu().menuItems)
			{
				mi.hover = false;
			}
			for (auto& mi : menu().menuItems)
			{
				if (mi.contains(getMousePosition()) && mi.enabled)
				{
					mi.hover = true;
					return;
				}
			}
		}

		void ContextMenu::onAnimationUpdate(LSRuntimeData& rtdata)
		{
			if (!hasMenuTemplate()) return;
			menu().menuItems[animNext].animStep = menu().menuItems[animNext].getw() / menu().menuItems[animNext].getAnimationLength();
			menu().menuItems[animNext].setw(0);
			menu().menuItems[animNext].animCurrentShade = 0.0f;
			menu().menuItems[animNext].restartAnimation();
			if (++animNext == menu().menuItems.size()) stopAnimaation();
		}
		
		void ContextMenu::update(LSRuntimeData& rtdata)
		{
			if (!isVisible() || !hasMenuTemplate()) return;
			updateAnimation(rtdata);
			for (uint8 i = 0; i < animNext; i++)
			{
				menu().menuItems[i].update(rtdata);
			}
		}
		
		void ContextMenu::onRender(LSRuntimeData& rtdata)
		{
			if (!isVisible() || !hasMenuTemplate()) return;
			for (uint8 i = 0; i < animNext; i++)
			{
				menu().menuItems[i].render(rtdata, textLayer(), geometryLayer());
			}
		}

		void ContextMenu::show(void)
		{
			setVisible(true);
			animNext = 0;
			restartAnimation();
		}

		void ContextMenu::hide(void)
		{
			setVisible(false);
		}

		void ContextMenu::display(FPoint position, ContextMenuTemplate& cmt)
		{
			hide();
			reset();
			setMenuPosition(position);
			setMenuTemplate(cmt);
			show();
		}




		void Button::create(LSComplexDrawable& base, bool hasText)
		{
			if (base.isValid()) share(base);
			m_defaultSize = FPoint(32, 32);
			m_iconSize = FPoint(20, 20);
			m_icon.invalidate();
			showText(hasText);
			m_text = "Button";
			setSize(m_defaultSize);
			m_updateWidth = true;
			m_padding = 7;
			m_textSize = 18;
			m_minWidth = 12;
			m_borderWidth = 1;
			m_hover = false;
			m_mouseDown = false;
			m_evtData = nullptr;
			m_callback = nullptr;
			m_autoDrawTooltip = true;
			m_tooltip.share(*this, true, false, true);

			m_textColor = LSColors::ButtonText;
			m_backgroundColor = LSColors::ButtonBackground;
			m_borderColor = LSColors::ButtonBorder;

			setAnimationLength(30);
			setVisible(true);
			setValid(true);
		}

		void Button::setIcon(Animation icon) 
		{
			m_icon = icon;
			if(m_icon.isInvalid()) return;
			m_icon.setSize(getIconSize());
			m_updateWidth = true;
		}

		void Button::setIconSize(FPoint is)
		{
			m_iconSize = is;
			if (m_icon.isInvalid()) return;
			m_icon.setSize(getIconSize());
			m_updateWidth = true;
		}

		void Button::onRender(LSRuntimeData& rtdata)
		{
			if (isInvalid()) return;
			geometryLayer().addOutlinedQuad(getBounds(), (m_hover ? LSColors::ButtonBackgroundHover : m_backgroundColor), (m_hover ? LSColors::ButtonHover : (isBorderDrawn() ? LSColors::ButtonBorder : Color::Transparent)), m_borderWidth);
			float x = m_padding;
			if (m_icon.isValid())
			{
				if (hasText())
				{
					gfxLayer().renderAnimation(rtdata, m_icon, FPoint(globx(x), globy(2 + ((geth() / 2) - (m_icon.geth() / 2)))), getCurrentColor(), true, false);
					x += m_icon.getw() + m_padding;
					textLayer().renderString(m_text, FPoint(globx(x), globy(2 + ((geth() / 2) - (textLayer().get().getBitmapFont().getStringBounds(m_text, m_textSize).y / 2)))), LSColors::ButtonText, m_textSize);
				}
				else
					gfxLayer().renderAnimation(rtdata, m_icon, FPoint(globx(((getw() / 2) - (m_icon.getw() / 2))), globy(((geth() / 2) - (m_icon.geth() / 2)))), getCurrentColor(), true, false);
			}
			else if (hasText())
				textLayer().renderString(m_text, FPoint(globx(x), globy(2 + ((geth() / 2) - (textLayer().get().getBitmapFont().getStringBounds(m_text, m_textSize).y / 2)))), LSColors::ButtonText, m_textSize);

			if (m_autoDrawTooltip)
				m_tooltip.render(rtdata);
		}
		
		void Button::update(LSRuntimeData& rtdata)
		{
			if (isInvalid()) return;
			updateAnimation(rtdata);
			if (m_updateWidth)
			{
				float w = m_padding;
				if (m_icon.isValid() && !hasText())
				{
					setSize(m_iconSize.x + (m_padding * 2) - 1, m_iconSize.y + (m_padding * 2) - 1);
					w = getw();
				}
				else if (m_icon.isValid() && hasText())
				{
					w += m_padding + m_icon.getw() + textLayer().get().getBitmapFont().getStringBounds(m_text, m_textSize).x + (m_padding / 2);
					seth(m_iconSize.y + (m_padding * 2) - 1);
				}
				else if (hasText())
					w += textLayer().get().getBitmapFont().getStringBounds(m_text, m_textSize).x + (m_padding / 2);

				if (w < m_minWidth) w = m_minWidth;
				setw(w);
				m_updateWidth = false;
			}
			m_tooltip.update(rtdata);
		}
		
		void Button::onAnimationUpdate(LSRuntimeData& rtdata)
		{
			if (isInvalid()) return;
		}
		
		void Button::mousePressed(LSEvent& evt, LSRuntimeData& rtdata)
		{
			if (isInvalid()) return;
			if (getBounds().contains(getMousePosition(), true))
				m_mouseDown = true;
		}
		
		void Button::mouseReleased(LSEvent& evt, LSRuntimeData& rtdata)
		{
			if (isInvalid()) return;
			m_mouseDown = false;
			if (!getBounds().contains(getMousePosition(), true))
				m_hover = false;
			else if (m_callback != nullptr)
				m_callback(rtdata, *this, (m_evtData != nullptr ? *m_evtData : Object::Invalid));
		}
		
		void Button::mouseMoved(LSEvent& evt, LSRuntimeData& rtdata)
		{
			if (isInvalid()) return;
			if (getBounds().contains(getMousePosition(), true))
			{
				if (!m_hover)
					m_tooltip.show("", getMousePosition());
				m_hover = true;
			}
			else
			{
				if (m_hover)
					m_tooltip.hide();
				m_hover = false;
			}
		}
		




		void StatusBar::create(void)
		{
			m_borderWidth = 1;
			m_height = 20;
			m_separatorWidth = 3;
			m_iconSize = FPoint(geth() - 8, geth() - 8);
			setBounds(0, ls_rtdata.getScreenSize().y - m_height - m_borderWidth, ls_rtdata.getScreenSize().x - m_borderWidth, m_height);
			m_nextBtnX = globx(getw() - m_borderWidth - (m_iconSize.x + 4));
			setVisible(true);
			setValid(true);
		}

		void StatusBar::onRender(LSRuntimeData& rtdata)
		{
			if (isInvalid()) return;
			geometryLayer().addOutlinedQuad(getBounds(), LSColors::StatusBarBackground, LSColors::StatusBarBorder, m_borderWidth);

			if (m_pointers.size() == 0) return;
			float xOffset = 10, strSpace = 12;
			uint16 i = 0;
			for (auto& ptr : m_pointers)
			{
				i++;
				String str = ptr.text;
				if (str.trim().length() > 0)
					str = str.add(" ");
				str = str.add(ptr.ptr->toString());
				textLayer().renderString(str, FPoint(clientX(xOffset),  clientY(1)), ptr.color, ptr.charHeight);
				xOffset += textLayer().get().getBitmapFont().getStringBounds(str, ptr.charHeight).x + (strSpace / 2);
				if (i < m_pointers.size()) geometryLayer().addQuad(FRect(clientX(xOffset), clientY(4), m_separatorWidth, m_height - 8), LSColors::StatusBarSeparator);
				xOffset += strSpace + m_separatorWidth + (strSpace / 3);
			}

			for (auto& b : m_iconBar.m_buttons)
			{
				//geometryLayer().addQuad(FRect(b.getx(), b.gety() + 5, m_borderWidth, geth() - m_borderWidth - 10), LSColors::ToolBarBorder);
				b.render(rtdata);
			}

			for (auto& b : m_iconBar.m_buttons)
				b.m_tooltip.render(rtdata);
		}

		void StatusBar::update(LSRuntimeData& rtdata)
		{
			if (isInvalid()) return;
			m_iconBar.update(rtdata);
		}

		uint16 StatusBar::attachPtr(Object& variable, String text, Color color, uint16 chHeight)
		{
			if (isInvalid()) return -1;
			PtrHolder ptr;
			ptr.ptr = &variable;
			ptr.text = text;
			ptr.color = color;
			ptr.charHeight = chHeight;
			m_pointers.push_back(ptr);
			return (uint16)(m_pointers.size() - 1);
		}

		bool StatusBar::removePtr(uint16 index)
		{
			if (isInvalid()) return false;
			if (index >= m_pointers.size())
				return false;
			m_pointers.erase(m_pointers.begin() + index);
			return true;
		}

		void StatusBar::windowResized(LSEvent& evt, LSRuntimeData& rtdata)
		{
			if (isInvalid()) return;
			m_nextBtnX = globx(getw() - m_borderWidth - (m_iconSize.x + 4));
			for (auto& b : m_iconBar.m_buttons)
			{
				b.setPosition(m_nextBtnX, globy((geth() / 2) - (b.geth() / 2) + m_borderWidth));
				m_nextBtnX -= b.getw() + 4;
			}
		}

		void StatusBar::onEvent(LSEvent& evt, LSRuntimeData& rtdata)
		{
			if (isInvalid()) return;
			m_iconBar.handleEvents(evt, rtdata);
		}

		Button& StatusBar::addButton(LSGuiCallback callback, String iconName, bool drawLabel, String label, int32 id, Object& data, bool enabled, String tooltip)
		{
			if (m_iconBar.isInvalid()) return INVALID_REF(Button);
			Button& btn = m_iconBar.addButton(callback, iconName, drawLabel, label, id, data, enabled);
			btn.share(*this);
			btn.m_tooltip.share(*this);
			btn.m_padding = 2;
			btn.drawBorder(true);
			btn.m_borderColor = LSColors::StatusBarButtonBorder;
			btn.setIconSize(m_iconSize);
			btn.seth(geth());
			btn.update(ls_rtdata);
			btn.setPosition(m_nextBtnX, globy((geth() / 2) - (btn.geth() / 2) + m_borderWidth));
			btn.setTooltipText(tooltip);
			m_nextBtnX -= btn.getw() + 4;
			return btn;
		}




		void MenuBar::create(void)
		{
			useGfxLayer(false);
			m_borderWidth = 1;
			m_height = 20;
			m_textSize = 18;
			m_nextMenuX = 0;
			m_hoverMenu = nullptr;
			m_menuVisible = false;
			setBounds(0, 0, ls_rtdata.getScreenSize().x - m_borderWidth, m_height);
			setVisible(true);
			setValid(true);
		}

		void MenuBar::addMenu(String name, ContextMenuTemplate& cmt)
		{
			Menu menu;
			menu.text = name;
			menu.menu = &cmt;
			uint32 sw = textLayer().get().getBitmapFont().getStringBounds(menu.text, m_textSize).x + 20;
			menu.setBounds(m_nextMenuX, gety(), sw, geth());
			menu.hover = false;
			m_nextMenuX += sw + m_borderWidth;
			m_menuList.push_back(menu);
		}

		void MenuBar::onRender(LSRuntimeData& rtdata)
		{
			if (isInvalid()) return;
			geometryLayer().addOutlinedQuad(getBounds(), LSColors::MenuBarBackground, LSColors::MenuBarBorder, m_borderWidth);
			for (auto& m : m_menuList)
			{
				geometryLayer().addQuad(m.getBounds(), (m.hover ? LSColors::MenuBarHover : LSColors::MenuBarBorder), false, m_borderWidth);
				float sw = (float)(textLayer().get().getBitmapFont().getStringBounds(m.text, m_textSize).x);
				textLayer().renderString(m.text, FPoint(m.getx() + ((m.getw() / 2) - (sw / 2)) + 6, globy(1)), (m.hover ? LSColors::MenuBarHover : LSColors::MenuBarText), m_textSize);
			}
		}

		void MenuBar::update(LSRuntimeData& rtdata)
		{
			if (isInvalid()) return;
		}

		void MenuBar::onEvent(LSEvent& evt, LSRuntimeData& rtdata)
		{
			if (isInvalid()) return;
		}

		void MenuBar::mouseMoved(LSEvent& evt, LSRuntimeData& rtdata)
		{
			if (isInvalid()) return;
			Menu* tmp = m_hoverMenu;
			for (auto& m : m_menuList)
				m.hover = false;
			m_hoverMenu = nullptr;
			bool inMenu = false;
			for (auto& m : m_menuList)
			{
				if (m.getBounds().contains(getMousePosition(), true))
				{
					inMenu = true;
					m.hover = true;
					m_hoverMenu = &m;
					if (m_menuVisible && tmp != nullptr && m.menu != tmp->menu)
					{
						if (m.menu == nullptr) break;
						ls_pinMenu.hide();
						ls_pinMenu.reset();
						ls_pinMenu.setMenuTemplate(*m.menu);
						ls_pinMenu.setMenuPosition(FPoint(m.getx() + 2, globy(geth() + m_borderWidth)));
						ls_pinMenu.show();
						m_menuVisible = true;
					}
					break;
				}
			}
			if (!inMenu && tmp != nullptr)
			{
				if (m_menuVisible)
				{
					m_hoverMenu = tmp;
					m_hoverMenu->hover = true;
				}
			}
		}

		void MenuBar::mousePressed(LSEvent& evt, LSRuntimeData& rtdata)
		{
			if (isInvalid()) return;
			if (!getBounds().contains(getMousePosition(), true))
			{
				for (auto& m : m_menuList)
					m.hover = false;
				m_hoverMenu = nullptr;
				m_menuVisible = false;
				return;
			}
			for (auto& m : m_menuList)
			{
				if (m.getBounds().contains(getMousePosition(), true))
				{
					if (m.menu == nullptr) break;
					ls_pinMenu.hide();
					ls_pinMenu.reset();
					bool v = m_menuVisible;
					m_menuVisible = false;
					if (v) break;
					ls_pinMenu.setMenuTemplate(*m.menu);
					ls_pinMenu.setMenuPosition(FPoint(m.getx() + 2, globy(geth() + m_borderWidth)));
					ls_pinMenu.show();
					m_menuVisible = true;
					break;
				}
			}
		}
	
		


		void ToolBar::create(String iconsFile)
		{
			if (iconsFile.trim() == "")
			{
				invalidate();
				return;
			}
			m_borderWidth = 1;
			m_height = 20;
			m_textSize = 18;
			m_nextBtnX = m_borderWidth;
			m_hoverBtn = nullptr;
			m_iconSize = FPoint(15, 15);
			setBounds(0, ls_menuBar.gety() + ls_menuBar.geth(), ls_rtdata.getScreenSize().x - m_borderWidth, m_height);
			setVisible(true);
			setValid(true);

			iconFile.load(iconsFile, ls_rtdata, ls_runtime);
			setTexture(iconFile.getTexture());
		}

		Button& ToolBar::addButton(LSGuiCallback callback, String iconName, bool drawLabel, String label, int32 id, Object& data, bool enabled, String tooltip)
		{
			Animation icon(iconFile.animation(iconName).getAnimationData());
			Button btn(*this, drawLabel);
			btn.m_tooltip.share(*this);
			btn.m_autoDrawTooltip = false;
			btn.setPosition(m_nextBtnX, globy(m_borderWidth));
			btn.drawBorder(false);
			btn.setIconSize(m_iconSize);
			btn.seth(22);
			btn.setIcon(icon);
			btn.setText(label);
			btn.set_id(id);
			btn.update(ls_rtdata);
			btn.m_evtData = &data;
			btn.m_callback = callback;
			btn.setTooltipText(tooltip);
			m_buttons.push_back(btn);
			m_nextBtnX += btn.getw() + 1;
			if (geth() < btn.geth() + (m_borderWidth * 2)) seth(btn.geth() + (m_borderWidth * 2));
			return m_buttons[m_buttons.size() - 1];
		}

		void ToolBar::onRender(LSRuntimeData& rtdata)
		{
			if (isInvalid()) return;
			geometryLayer().addOutlinedQuad(getBounds(), LSColors::ToolBarBackground, LSColors::ToolBarBorder, m_borderWidth);
			for (auto& b : m_buttons)
			{
				geometryLayer().addQuad(FRect(b.getx() + b.getw(), b.gety() + 5, m_borderWidth, geth() - m_borderWidth - 10), LSColors::ToolBarBorder);
				b.render(rtdata);
			}

			for (auto& b : m_buttons)
				b.m_tooltip.render(rtdata);
		}

		void ToolBar::update(LSRuntimeData& rtdata) 
		{
			if (isInvalid()) return;
			for (auto& b : m_buttons)
				b.update(rtdata);
		}

		void ToolBar::onEvent(LSEvent& evt, LSRuntimeData& rtdata)
		{
			if (isInvalid()) return;
			for (auto& b : m_buttons)
				b.handleEvents(evt, rtdata);
		}

		void ToolBar::mouseMoved(LSEvent& evt, LSRuntimeData& rtdata)
		{
			if (isInvalid()) return;
		}

		void ToolBar::mousePressed(LSEvent& evt, LSRuntimeData& rtdata)
		{
			if (isInvalid()) return;
		}





		void SideBar::create(void)
		{
			m_borderWidth = 1;
			m_width = 240;
			setBounds(ls_rtdata.getScreenSize().x - m_width, 1, m_width, ls_rtdata.getScreenSize().y - ls_statusBar.geth());
			setVisible(true);
			setValid(true);
		}
		

		void SideBar::onRender(LSRuntimeData& rtdata)
		{
			if (isInvalid()) return;
			geometryLayer().addOutlinedQuad(getBounds(), LSColors::SideBarBackground, LSColors::SideBarBorder, 1);
		}
		
		void SideBar::update(LSRuntimeData& rtdata)
		{
			if (isInvalid()) return;
		}
		
		void SideBar::onEvent(LSEvent& evt, LSRuntimeData& rtdata)
		{
			if (isInvalid()) return;
		}
				
	}
}
