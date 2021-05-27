#include "LayerRenderer.hpp"
#include "GameData.hpp"
#include "BitmapFont.hpp"
#include "Animation.hpp"
#include "Sprite.hpp"

namespace Omnia
{
	void LayerRenderer::create(int32 maxLayers)
	{
		m_maxLayers = maxLayers;
		m_layers.reserve(maxLayers);
		m_layerCount = 0;
		for (uint32 i = 0; i < maxLayers; i++)
			m_drawOrder.push_back(0);
	}

	Layer LayerRenderer::addLayer(int32 drawIndex, bool autoClear, bool autoAdjToCam)
	{
		if (m_layerCount >= m_maxLayers)
		{
			GameData::instance().errorQueue.push(Error::MAX_LAYERS_REACHED, ERROR_DATA());
			return Layer(INVALID_REF(VertexArray));
		}
		VertexArray va;
		va.setAutoClear(autoClear);
		va.setAdjustedToCamera(autoAdjToCam);
		if (drawIndex == AUTO__LAYER_RENDER_INDEX)
		{
			va.m_drawIndex = drawIndex;
			m_layers.push_back(va);
			m_drawOrder[m_layerCount] = m_layerCount;
			m_layerCount++;
			return Layer(m_layers[m_layerCount - 1]);
		}
		else if (drawIndex < m_maxLayers && drawIndex >= 0)
		{
			va.m_drawIndex = drawIndex;
			m_layers.push_back(va);
			m_drawOrder[va.m_drawIndex] = m_layerCount++;
			return Layer(m_layers[m_layerCount - 1]);
		}
		GameData::instance().errorQueue.push(Error::LAYER_DRAW_INDEX_OUT_OF_BOUNDS, ERROR_DATA());
		return Layer(INVALID_REF(VertexArray));
	}

	Layer LayerRenderer::addLayer(Image& texture, int32 drawIndex, bool autoClear, bool autoAdjToCam)
	{
		Layer l = addLayer(drawIndex, autoClear, autoAdjToCam);
		if (l.isValid())
			l.get().setTexture(texture);
		return l;
	}

	Layer LayerRenderer::addTextLayer(BitmapFont& font, int32 drawIndex, bool autoClear, bool autoAdjToCam)
	{
		Layer l = addLayer(font.getTexture(), drawIndex, autoClear, autoAdjToCam);
		if (l.isValid())
			l.get().setBitmapFont(font);
		return l;
	}

	Layer LayerRenderer::getLayer(int32 id)
	{
		for (auto& layer : m_layers)
		{
			if (layer.getID() == id)
				return Layer(layer);
		}
		GameData::instance().errorQueue.push(Error::LAYER_NOT_FOUND, ERROR_DATA());
		return Layer(INVALID_REF(VertexArray));
	}

	void LayerRenderer::clear(void)
	{
		VertexArray* va = nullptr;
		for (int32 i = 0; i < m_layerCount; i++)
		{
			va = &m_layers[m_drawOrder[i]];
			if (va->isAutoClearEnabled())
				va->clear();
		}
	}

	void LayerRenderer::render(GameData& gdata)
	{
		for (int32 i = 0; i < m_layerCount; i++)
			gdata.renderer.drawVertexArray(m_layers[m_drawOrder[i]]);
	}

	VertexArray& Layer::get(void)
	{
		if (m_va_ptr != nullptr)
			return *m_va_ptr;
		GameData::instance().errorQueue.push(Error::NULL_VERTEX_ARRAY_POINTER, ERROR_DATA());
		return INVALID_REF(VertexArray);
	}

	bool Layer::isValid(void)
	{
		if (m_va_ptr != nullptr)
			return get().isValid();
		return false;
	}

	bool Layer::isTextLayer(void)
	{
		if (m_va_ptr != nullptr)
			return get().isTextLayer();
		GameData::instance().errorQueue.push(Error::NULL_VERTEX_ARRAY_POINTER, ERROR_DATA());
		return false;
	}

	StringInfo Layer::makeString(String str, Color color)
	{
		if (isInvalid() || !isTextLayer())
		{
			GameData::instance().errorQueue.push(Error::INVALID_OR_NON_TEXT_LAYER, ERROR_DATA());
			return INVALID_OBJ(StringInfo);
		}
		StringInfo strInfo;
		strInfo.create(str, get().getBitmapFont(), color);
		return strInfo;
	}

	StringInfo Layer::renderString(String str, FPoint position, Color color, uint8 charHeight)
	{
		StringInfo strInfo = makeString(str, color);
		if (strInfo.isInvalid()) return strInfo;
		if (strInfo.isValid())
		{
			std::vector<TexInfo> data = strInfo.getData();
			UI8Point chSize = get().getBitmapFont().getCharacterSize();
			I8Point pad = get().getBitmapFont().getPadding();
			UI8Point cs = get().getBitmapFont().getCharacterSize();
			if (charHeight != AUTO__LAYER_RENDER_STRING_FONT_HEIGHT)
				chSize = UI8Point(PROPORTION(cs.x, charHeight, cs.y), charHeight);
			for (uint32 i = 0; i < strInfo.length(); i++)
			{
				I16Point padding = I16Point(pad.x * i, pad.y);
				get().addQuad(FRect(position.x + (chSize.x * i) + padding.x - (chSize.x), position.y + padding.y, chSize.x, chSize.y), data[i]); //TODO: Hardcoded - (chSize.x) due to bug F0001
			}
		}
		return strInfo;
	}

	void Layer::renderAnimation(GameData& gdata, Animation& anim, FPoint position, Color tint, bool update, bool savePosition)
	{
		FPoint np = anim.getPosition();
		anim.setPosition(position);
		if (update)
			anim.update(gdata);
		anim.render(gdata, get(), tint);
		if (!savePosition)
			anim.setPosition(np);
	}

	void Layer::renderTile(GameData& gdata, Sprite& tile, FPoint position, bool savePosition)
	{
		FPoint np = tile.getPosition();
		tile.setPosition(position);
		tile.render(gdata, get());
		if (!savePosition)
			tile.setPosition(np);
	}

	
	void Layer::addQuad(FRect bounds, Color color, bool fill, float frameWidth)
	{
		if (!isValid()) return;
		if (fill)
			get().addQuad(bounds, color);
		else
		{
			get().addQuad(FRect(bounds.x, bounds.y, bounds.w, frameWidth), color);
			get().addQuad(FRect(bounds.x + bounds.w, bounds.y, frameWidth, bounds.h), color);
			get().addQuad(FRect(bounds.x, bounds.y + bounds.h, bounds.w, frameWidth), color);
			get().addQuad(FRect(bounds.x, bounds.y, frameWidth, bounds.h), color);
		}
	}

	void Layer::addOutlinedQuad(FRect bounds, Color fillColor, Color outlineColor, float outlineThickness)
	{
		if (!isValid()) return;
		addQuad(bounds, outlineColor, false, outlineThickness);
		addQuad(FRect(bounds.x + outlineThickness, bounds.y + outlineThickness, bounds.w - outlineThickness, bounds.h - outlineThickness), fillColor, true);
	}
}
