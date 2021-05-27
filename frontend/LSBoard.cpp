#include "LSBoard.hpp"
#include "LSComponents.hpp"
#include "LSColors.hpp"
#include "DataFiles.hpp"
#include "Logic.hpp"
#include "Runtime.hpp"

namespace ls
{
	LSBoard::~LSBoard(void)
	{
	}

	FPoint LSBoard::convertToGrid(FPoint p, FPoint offset)
	{
		int32 x = p.x, y = p.y;
		x /= GRID_SIZE_PX;
		x *= GRID_SIZE_PX;
		y /= GRID_SIZE_PX;
		y *= GRID_SIZE_PX;
		return FPoint(x + offset.x, y + offset.y);
	}

	void LSBoard::setup(String libraryFile)
	{
		m_library = LSCLibrary(libraryFile, m_rtdata, *m_rtdata.application);
		if (m_library.isInvalid()) return;
		
		m_gridLayer = m_lrenderer.addLayer(0, false, true);
		m_geometryLayer = m_lrenderer.addLayer(1, true, true);
		m_gfxLayer = m_lrenderer.addLayer(m_library.getCSF().getTexture(), 2, true, true);
		m_textLayer = m_lrenderer.addTextLayer(m_rtdata.application->fonts().font("MonoBold"), 3, true, true);

		m_gridDrawn = false;
		m_pinSpacing = GRID_SIZE_PX;

		m_connectingMode = false;
		m_connectingWire = nullptr;
		m_clickDrag = false;
		m_selected = nullptr;

		setValid(true);
	}

	LSChip& LSBoard::addComponent(String componentName, FPoint position)
	{
		sh_ptr(LSChip) chip = m_library.newChip(componentName);
		if (chip == nullptr || chip->isInvalid()) return INVALID_REF(LSChip);
		if (chip->getID() == (int32)eLSCID::BasicChip)
		{
			chip->m_component = new_sh(LSComponent)(m_library.getCSF().animation(chip->m_componentImage).getAnimationData(), m_geometryLayer, *this);
		}
		else if (chip->getID() == (int32)eLSCID::_7SDisplayCC)
		{
			chip->m_component = new_sh(LS7SegmentDisplayCC)(m_geometryLayer, *this);
			chip->m_component->m_minHeight = GRID_SIZE_PX * 7;
			chip->m_component->m_minWidth = GRID_SIZE_PX * 6;
		}
		chip->m_component->m_chip = chip;
		if (position.x == -1 && position.y == -1)
			chip->m_component->setPosition(convertToGrid(FPoint(RANDOM(10, 800), RANDOM(10, 600))));
		else
			chip->m_component->setPosition(position);
		chip->m_component->setup(m_rtdata);
		chip->m_component->adjustPinPosition();
		chip->m_component->init();
		chip->m_componentID = LSChip::nextComponentID();
		for (auto& p : chip->m_pins)
		{
			p.signal = p.func->run();
			p.parent = &*chip;
		}
		m_components.push_back(chip);
		return *m_components[m_components.size() - 1];
	}

	void LSBoard::onEvent(LSEvent& evt, LSRuntimeData& rtdata)
	{
		if (isInvalid()) return;
		for (auto& c : m_components)
			c->getComponent().handleEvents(evt, rtdata);
	}

	void LSBoard::update(LSRuntimeData& rtdata)
	{
		if (isInvalid()) return;
		for (auto& c : m_components)
			c->getComponent().update(rtdata);
		for (auto& w : m_wires)
			w.update(rtdata);
	}

	void LSBoard::drawGrid(void)
	{
		for (uint x = 0; x <= m_rtdata.window.getSize().x; x += GRID_SIZE_PX)
		{
			for (uint y = 0; y <= m_rtdata.window.getSize().x; y += GRID_SIZE_PX)
			{
				if (x % (GRID_SIZE_PX * GRID_SIZE_PX) == 0 || y % (GRID_SIZE_PX * GRID_SIZE_PX) == 0)
					m_gridLayer.get().addQuad(FRect(x - 1, y - 1, 4, 4), LSColors::GridHighlight);
				else
					m_gridLayer.get().addQuad(FRect(x, y, 2, 2), LSColors::Grid);
			}
		}
	}

	void LSBoard::render(LSRuntimeData& rtdata)
	{
		if (isInvalid()) return;
		m_lrenderer.clear();
		if (!m_gridDrawn)
		{
			m_gridLayer.get().clear();
			drawGrid();
			m_gridDrawn = true;
		}
		for (std::vector<sh_ptr(LSChip)>::reverse_iterator i = m_components.rbegin(); i != m_components.rend(); ++i)
			i->get()->getComponent().render(rtdata, m_gfxLayer);
		for (auto& w : m_wires)
			w.render(rtdata, m_geometryLayer);
		m_lrenderer.render(rtdata);
	}

	void LSBoard::mousePressed(LSEvent& evt, LSRuntimeData& rtdata)
	{
		if (isInvalid()) return;
		if (m_connectingMode) return;
		if (getMousePressedCode() != SF_MBTN(Middle)) return;
		m_lastMousePos = getMousePosition();
	}

	void LSBoard::mouseMoved(LSEvent& evt, LSRuntimeData& rtdata)
	{
		if (isInvalid()) return;
		if (!m_connectingMode) return;
		for (auto& w : m_wires)
		{
			if (w.m_connecting)
				w.setMousePosition(getMousePosition());
		}
	}

	void LSBoard::mouseDragged(LSEvent& evt, LSRuntimeData& rtdata)
	{
		if (isInvalid()) return;
		if (m_connectingMode) return;
		if (getMousePressedCode() != SF_MBTN(Middle)) return;
		FPoint mpos = convertToGrid(getMousePosition());
		FPoint lmpos = convertToGrid(m_lastMousePos);
		m_dragDiff = FPoint(mpos.x - lmpos.x, mpos.y - lmpos.y);
		m_lastMousePos = getMousePosition();
		for (auto& c : m_components)
		{
			c->m_component->addPos(convertToGrid(m_dragDiff));
		}
	}

	void LSBoard::refreshAllComponents(void)
	{
		for (auto& c : m_components)
			c->getComponent().refreshPins();
	}

	LSChip& LSBoard::getChipByID(uint64 oid)
	{
		for (auto& c : m_components)
		{
			if (c->getComponentID() == oid)
				return *c;
		}
		return INVALID_REF(LSChip);
	}
}