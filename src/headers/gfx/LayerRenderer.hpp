#ifndef __LAYERRENDERER__HPP__
#define __LAYERRENDERER__HPP__

#include <iostream>
#include "VertexArray.hpp"

namespace Omnia
{
	class GameData;
	class Animation;
	class Sprite;
	class Layer
	{
		public:
			inline Layer(void) { m_va_ptr = nullptr; }
			inline Layer(VertexArray& va) { m_va_ptr = &va; }
			VertexArray& get(void);
			bool isValid(void);
			bool isTextLayer(void);
			inline bool isInvalid(void) { return !isValid(); }
			StringInfo makeString(String str, Color color = Color::White);
			void addQuad(FRect bounds, Color color, bool fill = true, float frameWidth = 1);
			void addOutlinedQuad(FRect bounds, Color fillColor, Color outlineColor, float outlineThickness = 1);

			StringInfo renderString(String str, FPoint position, Color color, uint8 charHeight = AUTO__LAYER_RENDER_STRING_FONT_HEIGHT);
			void renderAnimation(GameData& gdata, Animation& anim, FPoint position, Color tint = Color::White, bool update = false, bool savePosition = true);
			void renderTile(GameData& gdata, Sprite& tile, FPoint position, bool savePosition = true);

		private:
			VertexArray* m_va_ptr;
	};

	class GameData;
	class LayerRenderer
	{
		public:
			inline LayerRenderer(int32 maxLayers = AUTO__MAX_RENDER_LAYERS) { create(maxLayers); }
			void create(int32 maxLayers);
			Layer addLayer(int32 drawIndex = AUTO__LAYER_RENDER_INDEX, bool autoClear = true, bool autoAdjToCam = true);
			Layer addLayer(Image& texture, int32 drawIndex = AUTO__LAYER_RENDER_INDEX, bool autoClear = true, bool autoAdjToCam = true);
			Layer addTextLayer(BitmapFont& font, int32 drawIndex = AUTO__LAYER_RENDER_INDEX, bool autoClear = true, bool autoAdjToCam = true);
			Layer getLayer(int32 id);
			inline int32 getLayerCount(void) { return m_layerCount; }
			inline auto& getDrawOrderList(void) { return m_drawOrder; }

			void render(GameData& gdata);
			void clear(void);

		private:
			std::vector<VertexArray> m_layers;
			std::vector<uint16> m_drawOrder;
			int32 m_layerCount;
			int32 m_maxLayers;
	};
}

#endif
