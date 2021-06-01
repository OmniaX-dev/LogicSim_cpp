#include "LSComponents.hpp"
#include "LSColors.hpp"
#include "LSBoard.hpp"
#include "DataFiles.hpp"
#include "Logic.hpp"
#include "Runtime.hpp"
#include "Gui.hpp"

namespace ls
{
	LSChip::LSChip(void)
	{
		m_name = "Unnamed_Chip";
		m_componentImage = "GenericChip";
		m_pinCount = 0;
		m_pinVcc = nullptr;
		m_pinGnd = nullptr;
		m_nPinsBottom = 0;
		m_nPinsLeft = 0;
		m_nPinsRight = 0;
		m_nPinsTop = 0;
		m_component = nullptr;
		setValid(false);
	}

	LSChip::LSChip(const LSChip& copy)
	{
		m_name = copy.m_name;
		m_pinCount = copy.m_pinCount;
		m_pinVcc = copy.m_pinVcc;
		m_pinGnd = copy.m_pinGnd;
		m_pins = copy.m_pins;
		m_component = copy.m_component;
		m_componentImage = copy.m_componentImage;
		m_nPinsTop = copy.m_nPinsTop;
		m_nPinsBottom = copy.m_nPinsBottom;
		m_nPinsLeft = copy.m_nPinsLeft;
		m_nPinsRight = copy.m_nPinsRight;
		setID(copy.getID());
	}

	const LSPin& LSChip::getPin(String label)
	{
		for (auto& p : m_pins)
		{
			if (p.label == label) return p;
		}
		return INVALID_REF(const LSPin);
	}


//--------------------------------------------------------------------------------------------------------


	LSWire::LSWire(void) : m_first(nullptr), m_second(nullptr)
	{
		invalidate();
		setup();
	}

	LSWire::LSWire(LSPin& first, LSPin& second) : m_first(&first), m_second(&second)
	{
		setup();
		setValid(true);
	}
	
	LSWire::LSWire(LSPin& first) : m_first(&first), m_second(nullptr)
	{
		setup(LSColors::DefaultWire, true);
		setValid(true);
	}

	void LSWire::setup(Color color, bool connecting, uint8 width, FPoint mousePos)
	{
		m_color = color;
		m_connecting = connecting;
		m_wireWidth = width;
		m_mousePos = mousePos;
		m_board = nullptr;
	}

	void LSWire::connect(LSPin& second)
	{
		if (!m_connecting || !m_board->m_connectingMode) return;

		m_board->m_connectingMode = false;
		m_connecting = false;
		m_second = &second;
	}

	void LSWire::disconnect(void)
	{
		if (isInvalid()) return;
		//m_first->signal = m_second->signal = false;
		setValid(false);
		uint16 i = 0;
		for (auto& w : ls_mainBoard.m_wires)
		{
			if (w.equals(*this))
				break;
			i++;
		}
		STDVEC_REMOVE(ls_mainBoard.m_wires, i);
		ls_mainBoard.refreshAllComponents();
	}

	void LSWire::renderWire(LSRuntimeData& rtdata, Layer& layer)
	{
		Color col = (m_first->signal ? LSColors::DefaultWire_high : LSColors::DefaultWire);
		FPoint p1, p2, p3, p4;
		if (m_first->edge == eLSCPinEdge::Top)
		{
			p1 = FPoint(m_first->getx(), m_first->gety());
			p2 = FPoint(m_first->getx(), m_first->gety() - GRID_SIZE_PX);
		}
		else if (m_first->edge == eLSCPinEdge::Bottom)
		{
			p1 = FPoint(m_first->getx(), m_first->gety() + m_first->geth());
			p2 = FPoint(m_first->getx(), m_first->gety() + m_first->geth() + GRID_SIZE_PX);
		}
		layer.get().addQuad(newSegment(m_board->convertToGrid(p1, s_bugOffset), m_board->convertToGrid(p2, s_bugOffset), m_wireWidth), col);
		
		if (m_connecting)
		{
			drawSquareLine(rtdata, layer, p2, m_mousePos, col, false);
			return;
		}

		if (m_second->edge == eLSCPinEdge::Top)
		{
			p3 = FPoint(m_second->getx(), m_second->gety());
			p4 = FPoint(m_second->getx(), m_second->gety() - GRID_SIZE_PX);
		}
		else if (m_second->edge == eLSCPinEdge::Bottom)
		{
			p3 = FPoint(m_second->getx(), m_second->gety() + m_second->geth());
			p4 = FPoint(m_second->getx(), m_second->gety() + m_second->geth() + GRID_SIZE_PX);
		}
		layer.get().addQuad(newSegment(m_board->convertToGrid(p3, s_bugOffset), m_board->convertToGrid(p4, s_bugOffset), m_wireWidth), col);
		drawSquareLine(rtdata, layer, p2, p4, col, false);
	}
	
	void LSWire::render(LSRuntimeData& rtdata, Layer& layer)
	{
		if (isInvalid()) return;
		renderWire(rtdata, layer);
	}

	void LSWire::drawSquareLine(LSRuntimeData& rtdata, Layer& layer, FPoint _p1, FPoint p3, Color color, bool addToList)
	{
		FPoint orig =  _p1;
		FPoint p = m_board->convertToGrid(p3, s_bugOffset);
		FPoint diff((int)(abs(p.x - orig.x)), (int)(abs(p.y - orig.y)));
		FPoint p1 = m_board->convertToGrid(_p1, s_bugOffset);
		FPoint p2;
		if (diff.y > diff.x) p2 = m_board->convertToGrid(FPoint(orig.x, p.y), s_bugOffset);
		else p2 = m_board->convertToGrid(FPoint(p.x, orig.y), s_bugOffset);
		layer.get().addQuad(newSegment(p1, p2, m_wireWidth), color); 
		layer.get().addQuad(newSegment(p2, p, m_wireWidth), color); 
	}

	void LSWire::update(LSRuntimeData& rtdata)
	{
		if (m_board == nullptr) invalidate();
		if (isInvalid()) return;
		if (m_connecting) return;
		if (m_first == nullptr || m_second == nullptr) return;
		if (m_first->isInvalid() || m_second->isInvalid()) return;

		if ((m_first->func->run() && m_first->parent->m_component->vccSignal()) || (m_second->func->run() && m_second->parent->m_component->vccSignal()))
		{
			m_first->signal = true;
			m_second->signal = true;
		}
		else
		{
			m_first->signal = false;
			m_second->signal = false;
		}
	}
	
	void LSWire::adjustPosition(void)
	{
		if (isInvalid()) return;
	}
	
	void LSWire::addSegment(FPoint endPos)
	{
		if (isInvalid()) return;
	}

	FRect LSWire::makeSegment(FPoint start, FPoint end, eLSOrientation ori, float w)
	{
		FRect line;
		if (ori == eLSOrientation::Horizontal)
		{
			if (start.y != end.y) return line;
			if (start.x > end.x)
			{
				FPoint tmp = start;
				start = end;
				end = tmp;
			}
			line.x = start.x;
			line.y = start.y - (w / 2);
			line.w = end.x - start.x;
			line.h = w;
		}
		else if (ori == eLSOrientation::Vertical)
		{
			if (start.x != end.x) return line;
			if (start.y > end.y)
			{
				FPoint tmp = start;
				start = end;
				end = tmp;
			}
			line.x = start.x - (w / 2);
			line.y = start.y;
			line.w = w;
			line.h = end.y - start.y;
		}
		return line;
	}

	FRect LSWire::newSegment(FPoint start, FPoint end, float w, eLSOrientation* ori)
	{
		if (start.x == end.x)
		{
			if (ori != nullptr) *ori = eLSOrientation::Vertical;
			return makeSegment(start, end, eLSOrientation::Vertical, w);
		}
		else if (start.y == end.y)
		{
			if (ori != nullptr) *ori = eLSOrientation::Horizontal;
			return makeSegment(start, end, eLSOrientation::Horizontal, w);
		}
		return FRect();
	}

	std::vector<FPoint> LSWire::makeLink(FPoint start, FPoint end)
	{
		std::vector<FPoint> points(3);

		return points;
	}


//--------------------------------------------------------------------------------------------------------


	LSComponentBase::LSComponentBase(Layer& boxLayer, LSBoard& board)
	{
		m_boxLayer = &boxLayer;
		m_board = &board;
		m_selected = false;
		resetMouseDir();
		setValid(false);
	}

	void LSComponentBase::resetMouseDir(bool value)
	{
		m_dirChanged[LSCHD_Left] = value;
		m_dirChanged[LSCHD_Right] = value;
		m_dirChanged[LSCHD_Up] = value;
		m_dirChanged[LSCHD_Down] = value;
	}

	void LSComponentBase::adjustPosition(LSEvent& evt, LSRuntimeData& rtdata)
	{
		FPoint p(evt.mouseMove.x - m_clickDiff.x, evt.mouseMove.y - m_clickDiff.y);
		if ((int)abs(p.x - getx()) < GRID_SIZE_PX)
			p.x = getx();
		if ((int)abs(p.y - gety()) < GRID_SIZE_PX)
			p.y = gety();
		//resetMouseDir();
		p.x = ((int)p.x / GRID_SIZE_PX) * GRID_SIZE_PX;
		p.y = ((int)p.y / GRID_SIZE_PX) * GRID_SIZE_PX;
		setPosition(p);
	}

	void LSComponentBase::mousePressed(LSEvent& evt, LSRuntimeData& data)
	{
		m_pressPos = IPoint(evt.mouseButton.x, evt.mouseButton.y);
		m_lastMousePos = m_pressPos;
		m_clickDiff = IPoint(evt.mouseButton.x - getx(), evt.mouseButton.y - gety());
		m_clickInside = contains(FPoint(m_pressPos.x, m_pressPos.y));
		if (m_clickInside)
		{
			uint16 i = 0, j = 0;
			sh_ptr(LSChip) _this = nullptr;
			for (auto& c : m_board->m_components)
			{
				if (c->getComponent().equals(*this))
				{
					c->getComponent().m_selected = true;
					ls_mainBoard.m_selected = c;
					j = i;
				}
				else
					c->getComponent().m_selected = false;
				i++;
			}
			if (m_board->m_selected == nullptr) return;
			STDVEC_REMOVE(m_board->m_components, j);
			m_board->m_components.insert(m_board->m_components.begin(), m_board->m_selected);
			onMousePressed(evt, data);
		}
	}

	void LSComponentBase::mouseReleased(LSEvent& evt, LSRuntimeData& data)
	{
		if (getMousePressedCode() != SF_MBTN(Left) || !m_clickInside) return;
		m_clickInside = false;
		resetMouseDir();
		onMouseReleased(evt, data);
	}

	void LSComponentBase::updateMouseDirection(LSEvent& evt)
	{
		uint8 moveSensitivity = 0;
		if (evt.mouseMove.x < m_lastMousePos.x - moveSensitivity)
		{
			m_dirChanged[LSCHD_Left] = true;
			m_dirChanged[LSCHD_Right] = false;
		}
		else if (evt.mouseMove.x > m_lastMousePos.x + moveSensitivity)
		{
			m_dirChanged[LSCHD_Left] = false;
			m_dirChanged[LSCHD_Right] = true;
		}

		if (evt.mouseMove.y < m_lastMousePos.y - moveSensitivity)
		{
			m_dirChanged[LSCHD_Up] = true;
			m_dirChanged[LSCHD_Down] = false;
		}
		else if (evt.mouseMove.y > m_lastMousePos.y + moveSensitivity)
		{
			m_dirChanged[LSCHD_Up] = false;
			m_dirChanged[LSCHD_Down] = true;
		}

		m_lastMousePos.x = evt.mouseMove.x;
		m_lastMousePos.y = evt.mouseMove.y;
	}

	void LSComponentBase::mouseMoved(LSEvent& evt, LSRuntimeData& data)
	{
		//THIS METHOD GETS CALLED FROM mouseDragged()
		onMouseMoved(evt, data);
	}

	void LSComponentBase::mouseDragged(LSEvent& evt, LSRuntimeData& data)
	{
		if (getMousePressedCode() != SF_MBTN(Left) || !m_clickInside || m_board->m_selected == nullptr || !equals(m_board->m_selected->getComponent())) return;
		updateMouseDirection(evt);
		adjustPosition(evt, data);
		onMouseDragged(evt, data);
		mouseMoved(evt, data);
	}


//--------------------------------------------------------------------------------------------------------


	LSPinComponent::LSPinComponent(Layer& boxLayer, LSBoard& board) : LSComponentBase(boxLayer, board)
	{
		m_chip = nullptr;
		m_pinLength = GRID_SIZE_PX;
		m_pinWidth = 2;
		m_selPinBounds = FRect(-100, -100, 0, 0);
		m_borderWidth = 1;
		m_useCustomBg = false;
		m_customBgCol = LSColors::ComponentBackground;

		m_pinMenuTemplate.addMenuItem("Clear connections", &component_menu_callback, CMI_Pin_ClearConnections);
		m_pinMenuTemplate.addMenuItem("Signal", &component_menu_callback, CMI_Pin_Signal, Object::Invalid, true);
		m_pinMenuTemplate.addMenuItem("Find", &component_menu_callback, 0, Object::Invalid, false, nullptr, false);
		m_pinMenuTemplate.addMenuItem("Test Component", &component_menu_callback, CMI_Component_Test);

		m_minWidth = GRID_SIZE_PX * 8;
		m_minHeight = GRID_SIZE_PX * 3;
	}

	void LSPinComponent::setup(LSRuntimeData& gdata)
	{
		setSize(0, 0);

		if (m_chip->m_nPinsBottom > m_chip->m_nPinsTop)
			setw((m_chip->m_nPinsBottom + 1) * m_board->m_pinSpacing);
		else
			setw((m_chip->m_nPinsTop + 1) * m_board->m_pinSpacing);
		
		if (m_chip->m_nPinsLeft > m_chip->m_nPinsRight)
			seth((m_chip->m_nPinsLeft + 1) * m_board->m_pinSpacing);
		else
			seth((m_chip->m_nPinsRight + 1) * m_board->m_pinSpacing);

		if (getw() < m_minWidth) setw(m_minWidth);
		if (geth() < m_minHeight) seth(m_minHeight);

		resetMouseDir();
	}

	void LSPinComponent::adjustPinPosition(void)
	{
		uint16 it = m_chip->m_nPinsTop + 1, ib = 0, il = 0, ir = 0;
		for (auto& p : m_chip->m_pins)
		{
			if (p.edge == eLSCPinEdge::Top)
			{
				it--;
				p.sety(gety() - m_pinLength);
				p.setx(getx() + (it * m_board->m_pinSpacing));
				p.setw(m_pinWidth);
				p.seth(m_pinLength);
			}
			else if (p.edge == eLSCPinEdge::Bottom)
			{
				ib++;
				p.sety(gety() + geth());
				p.setx(getx() + (ib * m_board->m_pinSpacing));
				p.setw(m_pinWidth);
				p.seth(m_pinLength);
			}
			else if (p.edge == eLSCPinEdge::Left)
			{
				il++;
				p.sety(gety() + (il * m_board->m_pinSpacing));
				p.setx(getx() - m_pinLength);
				p.setw(m_pinLength);
				p.seth(m_pinWidth);
			}
			else if (p.edge == eLSCPinEdge::Right)
			{
				ir++;
				p.sety(gety() + (ir * m_board->m_pinSpacing));
				p.setx(getx() + getw());
				p.setw(m_pinLength);
				p.seth(m_pinWidth);
			}
		}
	}

	void LSPinComponent::mouseMoved(LSEvent& evt, LSRuntimeData& rtdata)
	{
		//THIS METHOD GETS CALLED FROM mouseDragged()
		if (ls_pinMenu.isVisible()) return;
		FPoint mp = getMousePosition();
		Drawable pinBounds;
		for (auto& p : m_chip->m_pins)
		{
			pinBounds = Drawable(p.getx(), p.gety(), p.getw(), p.geth());
			pinBounds.subx(GRID_SIZE_PX / 2);
			pinBounds.suby(GRID_SIZE_PX / 2);
			pinBounds.addh(GRID_SIZE_PX);
			pinBounds.setw(GRID_SIZE_PX);

			if (pinBounds.contains(mp))
			{
				m_selPinBounds = pinBounds.getBounds();
				m_pinHover = &p;
				return;
			}
		}
		m_pinHover = nullptr;
		m_selPinBounds = FRect(-100, -100, 0, 0);
		LSComponentBase::mouseMoved(evt, rtdata);
	}

	void LSPinComponent::mouseDragged(LSEvent& evt, LSRuntimeData& rtdata)
	{
		LSComponentBase::mouseDragged(evt, rtdata);
		adjustPinPosition();
	}
	
	void LSPinComponent::mousePressed(LSEvent& evt, LSRuntimeData& rtdata)
	{
		LSComponentBase::mousePressed(evt, rtdata);
		if (m_pinHover != nullptr) pinMousePressed(evt, rtdata, *m_pinHover);
		else m_selPinBounds = FRect(-100, -100, 0, 0);
	}

	void LSPinComponent::pinMousePressed(LSEvent& evt, LSRuntimeData& rtdata, LSPin& sender)
	{
		if (getMousePressedCode() == SF_MBTN(Right))
		{
			loadPinContextMenu();
			return;
		}
		if (m_board->m_connectingMode)
		{
			//if (sender.type != eLSCPinType::Input) return;
			m_board->m_connectingWire->connect(sender);
			m_board->m_connectingWire = nullptr;
			return;
		}
		//if (sender.type != eLSCPinType::Output) return;
		if (sender.edge == eLSCPinEdge::Top || sender.edge == eLSCPinEdge::Bottom)
		{
			m_board->m_connectingMode = true;
			LSWire wire(sender);
			wire.m_board = m_board;
			wire.m_wireWidth = sender.getw();
			m_board->m_wires.push_back(wire);
			m_board->m_connectingWire = &(m_board->m_wires[m_board->m_wires.size() - 1]);
		}
	}

	void LSPinComponent::update(LSRuntimeData& rtdata)
	{
		for (auto& p : m_chip->m_pins)
		{
			if (p.type == eLSCPinType::Output)
			{
				p.func->run();
			}
		}
	}

	void LSPinComponent::render(LSRuntimeData& rtdata, Layer& layer)
	{
		if (m_boxLayer == nullptr) return;
		Color bcol = LSColors::ComponentBorder, gcol = LSColors::ComponentImage;
		if (m_selected)
		{
			bcol = LSColors::ComponentBorder_sel;
			gcol = LSColors::ComponentBorder_sel;
		}
		m_boxLayer->get().addQuad(getBounds(), (m_useCustomBg ? m_customBgCol : LSColors::ComponentBackground));
		m_boxLayer->get().addQuad(FRect(getx(), gety(), m_borderWidth, geth()), bcol);
		m_boxLayer->get().addQuad(FRect(getx(), gety(), getw(), m_borderWidth), bcol);
		m_boxLayer->get().addQuad(FRect(getx(), gety() + geth() - m_borderWidth, getw(), m_borderWidth), bcol);
		m_boxLayer->get().addQuad(FRect(getx() + getw() - m_borderWidth, gety(), m_borderWidth, geth()), bcol);

		if (m_chip != nullptr)
		{
			Color color;
			for (auto& p : m_chip->m_pins)
			{
				if (p.type == eLSCPinType::Power) color = LSColors::LSCPowerPin;
				else if (p.type == eLSCPinType::Ground) color = LSColors::LSCGroundPin;
				else if (p.type == eLSCPinType::Input) color = LSColors::LSCInputPin;
				else if (p.type == eLSCPinType::Output) color = LSColors::LSCOutputPin;
				else color = Color::White;
				m_boxLayer->get().addQuad(p.getBounds(), color);
			}
		}

		if (m_selPinBounds.x >= 0 && !m_clickInside)
		{
			m_boxLayer->get().addQuad(m_selPinBounds, LSColors::LSCPin_sel);
			if (m_pinHover != nullptr)
			{
				float margin = 10;
				gui::Tooltip tt(m_pinHover->label, FPoint());
				tt.update(rtdata, m_board->m_textLayer);
				tt.setx((m_selPinBounds.x + (m_selPinBounds.w / 2)) - (tt.getw() / 2));
				if (m_pinHover->edge == eLSCPinEdge::Top)
					tt.sety(m_selPinBounds.y - tt.geth() - margin);
				else if (m_pinHover->edge == eLSCPinEdge::Bottom)
					tt.sety(m_selPinBounds.y + m_selPinBounds.h + margin);
				tt.render(rtdata, m_board->m_textLayer, m_board->m_geometryLayer);
			}
		}

		onRender(rtdata, layer);
	}

	void LSPinComponent::refreshPins(void)
	{
		for (auto& p : m_chip->m_pins)
		{
			bool connected = false;
			for (auto& w : m_board->m_wires)
			{
				if (w.m_first->equals(p) || w.m_second->equals(p))
				{
					connected = true;
					break;
				}
			}
			if (!connected)
				p.signal = false;
		}
	}

	void LSPinComponent::loadPinContextMenu(void)
	{
		if (m_pinHover == nullptr) return;
		ls_pinMenu.reset();
		ls_pinMenu.setMenuTemplate(m_pinMenuTemplate);
		for (auto& mi : m_pinMenuTemplate.menuItems)
		{
			if (mi.getID() == CMI_Pin_ClearConnections || mi.getID() == CMI_Component_Test)
				mi.data = m_pinHover;
			else if (mi.getID() == CMI_Pin_Signal)
				mi.bool_ptr = &(m_pinHover->signal);
		}
		ls_pinMenu.setMenuPosition(getMousePosition());
		ls_pinMenu.show();
		//m_selPinBounds = FRect(-100, -100, 0, 0);
		m_pinHover = nullptr;
	}

	void LSPinComponent::component_menu_callback(LSRuntimeData& rtdata, Object& sender, Object& data)
	{
		if (data.isInvalid()) return;
		gui::ContextMenuItem& cmi = (gui::ContextMenuItem&)sender;
		if (cmi.getID() == CMI_Pin_ClearConnections)
		{
			LSPin& pin = (LSPin&)data;
			for (auto& w : ls_mainBoard.m_wires)
			{
				if (w.isInvalid()) continue;
				if (w.m_first->equals(pin) || w.m_second->equals(pin))
				{
					w.disconnect();
					rtdata.debugConsole.system("Removed wire.");
				}
			}
		}
		else if (cmi.getID() == CMI_Component_Test)
		{
			LSPin& pin = (LSPin&)data;
			ls_cmd.show();
			ls_cmd.command(StringBuilder("/test_component -sig ").add((int)pin.parent->getComponentID()).get());
		}
	}


//--------------------------------------------------------------------------------------------------------


	LSComponent::LSComponent(AnimationData ad, Layer& boxLayer, LSBoard& board) : LSPinComponent(boxLayer, board)
	{
		m_sprite.create(ad);
		m_defaultScale = 0.5f;
		IPoint fixedSize(m_sprite.getSize());
		fixedSize.x *= m_defaultScale;
		fixedSize.y *= m_defaultScale;
		fixedSize.x = (int)(fixedSize.x / GRID_SIZE_PX) * GRID_SIZE_PX;
		fixedSize.y = (int)(fixedSize.y / GRID_SIZE_PX) * GRID_SIZE_PX;
		m_defaultSize = FPoint((float)fixedSize.x, (float)fixedSize.y);
		m_sprite.setSize(m_defaultSize);
		setValid(true);
	}

	void LSComponent::update(LSRuntimeData& rtdata)
	{
		LSPinComponent::update(rtdata);
		m_sprite.setPosition(getx() + (getw() / 2) - (m_sprite.getSize().x / 2), gety() + (geth() / 2) - (m_sprite.getSize().y / 2));
	}

	void LSComponent::onRender(LSRuntimeData& rtdata, Layer& layer)
	{
		IPoint sb = m_board->m_textLayer.get().getBitmapFont().getStringBounds(m_chip->m_name, 20);
		m_board->m_textLayer.renderString(m_chip->m_name, FPoint(getx() + (getw() / 2) - (sb.x / 2) + 4, gety() + (geth() / 2) - (sb.y / 2)), LSColors::ComponentName, 20);
		//if (m_selected) m_board->m_gfxLayer.renderAnimation(rtdata, m_sprite, m_sprite.getPosition(), LSColors::ComponentBorder_sel, true, false);
		//else m_board->m_gfxLayer.renderAnimation(rtdata, m_sprite, m_sprite.getPosition(), LSColors::ComponentImage, true, false);
	}


//--------------------------------------------------------------------------------------------------------


	LS7SegmentDisplayCC::LS7SegmentDisplayCC(Layer& boxLayer, LSBoard& board) : LSPinComponent(boxLayer, board)
	{
		init();
	}

	void LS7SegmentDisplayCC::init(void)
	{
		m_borderWidth = 1;
		for (uint8 i = 0; i < 8; i++)
			m_segmentStatus[i] = false;

		m_segw = 6;
		m_segl = 20;
		m_customSegColor_h = Color(0, 30, 150, 210);
		m_customSegColor_l = Color(0, 0, 20, 210);
		m_useCustomSegColor = false;
		m_segSpacing = 3;
		m_customBgCol = Color(30, 30, 30, 210);
		m_useCustomBg = true;
		LSEvent evt;
		onMouseDragged(evt, LSRuntimeData::instance());
		setValid(true);
	}

	void LS7SegmentDisplayCC::update(LSRuntimeData& rtdata)
	{
		LSPinComponent::update(rtdata);
		m_segments.clear();
		m_segments.push_back(Drawable(m_segw + m_segSpacing,   //------------------A------------------
									  0,
									  m_segl,
									  m_segw)
							);
		m_segments.push_back(Drawable(m_segw + m_segSpacing + m_segl + m_segSpacing,   //------------------B------------------
									  m_segw + m_segSpacing,
									  m_segw,
									  m_segl)
							);
		m_segments.push_back(Drawable(m_segw + m_segSpacing + m_segl + m_segSpacing,   //------------------C------------------
							 		  m_segw + m_segSpacing + m_segl + m_segSpacing + (m_segw / 2) + m_segSpacing,
							 		  m_segw,
							 		  m_segl)
							);
		m_segments.push_back(Drawable(m_segw + m_segSpacing,   //------------------D------------------
									  m_segw + m_segSpacing + m_segl + m_segSpacing + (m_segw / 2) + m_segSpacing + m_segl + m_segSpacing,
									  m_segl,
									  m_segw)
							);
		m_segments.push_back(Drawable(0,   //------------------E------------------
									  m_segw + m_segSpacing + m_segl + m_segSpacing + (m_segw / 2) + m_segSpacing,
									  m_segw,
									  m_segl)
							);
		m_segments.push_back(Drawable(0,   //------------------F------------------
									  m_segw + m_segSpacing,
									  m_segw,
									  m_segl)
							);
		m_segments.push_back(Drawable(m_segw + m_segSpacing,   //------------------G------------------
									  m_segw + m_segSpacing + m_segl + m_segSpacing - (m_segw / 4),
									  m_segl,
									  m_segw)
							);
		m_segments.push_back(Drawable(m_segw + m_segSpacing + m_segl + m_segSpacing + m_segw + m_segSpacing,   //-----------------DP------------------
									  m_segw + m_segSpacing + m_segl + m_segSpacing + (m_segw / 2) + m_segSpacing + m_segl + m_segSpacing,
									  m_segw,
									  m_segw)
							);

		uint16 width = m_segw + m_segSpacing + m_segl + m_segSpacing + m_segw;
		uint16 height = m_segw + m_segSpacing + m_segl + m_segSpacing + (m_segw / 2) + m_segSpacing + m_segl + m_segSpacing + m_segw;

		uint8 dp_offset = 2;
		for (uint8 i = 0; i < m_segments.size(); i++)
		{
			m_segments[i].addx(getx() + ((getw() / 2) - (width / 2)));
			m_segments[i].addy(gety() + ((geth() / 2) - (height / 2)));
		}

		m_segmentStatus[0] = m_chip->m_pins[DataFile::getGlobal("PIN_POS_A").asInt() - 1].signal;
		m_segmentStatus[1] = m_chip->m_pins[DataFile::getGlobal("PIN_POS_B").asInt() - 1].signal;
		m_segmentStatus[2] = m_chip->m_pins[DataFile::getGlobal("PIN_POS_C").asInt() - 1].signal;
		m_segmentStatus[3] = m_chip->m_pins[DataFile::getGlobal("PIN_POS_D").asInt() - 1].signal;
		m_segmentStatus[4] = m_chip->m_pins[DataFile::getGlobal("PIN_POS_E").asInt() - 1].signal;
		m_segmentStatus[5] = m_chip->m_pins[DataFile::getGlobal("PIN_POS_F").asInt() - 1].signal;
		m_segmentStatus[6] = m_chip->m_pins[DataFile::getGlobal("PIN_POS_G").asInt() - 1].signal;
		m_segmentStatus[7] = m_chip->m_pins[DataFile::getGlobal("PIN_POS_P").asInt() - 1].signal;
	}

	void LS7SegmentDisplayCC::onRender(GameData& gdata, Layer& layer)
	{
		for (uint8 i = 0; i < m_segments.size(); i++)
		{
			if (m_segmentStatus[i])
				m_boxLayer->get().addQuad(m_segments[i].getBounds(), (m_useCustomSegColor ? m_customSegColor_h : LSColors::LS7SDisplaySegment_high));
			else
				m_boxLayer->get().addQuad(m_segments[i].getBounds(), (m_useCustomSegColor ? m_customSegColor_l : LSColors::LS7SDisplaySegment_low));
		}
	}
	
	
	
	LSPowerLine::LSPowerLine(Layer& boxLayer, LSBoard& board) : LSPinComponent(boxLayer, board)
	{
		init();
	}

	void LSPowerLine::init(void)
	{
		m_borderWidth = 1;
		setValid(true);
	}

	void LSPowerLine::update(LSRuntimeData& rtdata)
	{
		LSPinComponent::update(rtdata);
	}

	void LSPowerLine::onRender(GameData& gdata, Layer& layer)
	{
		m_customBgCol = Color(30, 100, 30, 210);
		m_useCustomBg = true;
	}
}