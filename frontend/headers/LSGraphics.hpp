#ifndef __LS_GRAPHICS_HPP__
#define __LS_GRAPHICS_HPP__

#include "LSDefines.hpp"
#include "LayerRenderer.hpp"

namespace ls
{
	class LSComplexDrawable : public Object
	{
		public:
			LSComplexDrawable(void);
			void render(LSRuntimeData& rtdata);
			inline virtual void onRender(LSRuntimeData& rtdata) {  }
			inline Layer& textLayer(void) { if (isTextLayerShared()) return m_shared->textLayer(); return m_textLayer; }
			inline Layer& gfxLayer(void) { if (isGfxayerShared()) return m_shared->gfxLayer(); return m_gfxLayer; }
			inline Layer& geometryLayer(void) { if (isGeometryLayerShared()) return m_shared->geometryLayer(); return m_geometryLayer; }
			void share(LSComplexDrawable& sh, bool textl = true, bool gfxl = true, bool geol = true);
			inline void stopSharing(void) { m_sharedGraphics = false; m_shared = nullptr; }

			inline bool isShared(void) { return m_sharedGraphics && m_shared != nullptr && m_shared->isValid(); }
			inline bool isTextLayerShared(void) { return isShared() && m_shareTextLayer; }
			inline bool isGfxayerShared(void) { return isShared() && m_shareGfxLayer; }
			inline bool isGeometryLayerShared(void) { return isShared() && m_shareGeometryLayer; }
			inline bool getAutoClear(void) { return m_autoClear; }
			inline Image& getTexture(void) { return m_gfxLayer.get().getTexture(); }
			inline void setBitmapFont(BitmapFont& font) { m_textLayer.get().setBitmapFont(font); }
			inline void setTexture(Image& texture) { m_gfxLayer.get().setTexture(texture);  }
			inline void setAutoClear(bool ac) { m_autoClear = ac; }
			inline void setVisible(bool v) { m_visible = v; }
			void clearRenderer(void);
			inline void useTextLayer(bool u) { m_useTextLayer = u; }
			inline void useGfxLayer(bool u) { m_useGfxLayer = u; }
			inline void useGeometryLayer(bool u) { m_useGeometryLayer = u; }
			inline void enableRender(bool er) { m_renderEnabled = er; }
			inline bool isVisible(void) { return m_visible; }
			inline bool isTextLayerUsed(void) { if (isTextLayerShared()) return m_shared->m_useTextLayer; return m_useTextLayer; }
			inline bool isGfxLayerUsed(void) { if (isGfxayerShared()) return m_shared->m_useGfxLayer; return m_useGfxLayer; }
			inline bool isGeometryLayerUsed(void) { if (isGeometryLayerShared()) return m_shared->m_useGeometryLayer; return m_useGeometryLayer; }
			inline bool isRenderEnabled(void) { if (isShared()) return m_shared->m_renderEnabled; return m_renderEnabled; }

		private:
			Layer m_textLayer;
			Layer m_geometryLayer;
			Layer m_gfxLayer;

			bool m_autoClear;
			bool m_useTextLayer;
			bool m_useGfxLayer;
			bool m_useGeometryLayer;
			bool m_renderEnabled;
			bool m_sharedGraphics;
			bool m_shareTextLayer;
			bool m_shareGfxLayer;
			bool m_shareGeometryLayer;

			bool m_visible;

			LSComplexDrawable* m_shared;

			LayerRenderer m_renderer;
	};

	class Animator
	{
		public:
			Animator(void);
			void startAnimation(void);
			void stopAnimaation(void);
			void resetAnimation(void);
			void restartAnimation(void);
			bool renderAnimation(LSRuntimeData& rtdata);
			bool updateAnimation(LSRuntimeData& rtdata);

			inline virtual void onAnimationStart(void) {  }
			inline virtual void onAnimationEnd(void) {  }
			inline virtual void onAnimationReset(void) {  }
			inline virtual void onAnimationRestart(void) {  }
			inline virtual void onAnimationRender(LSRuntimeData& rtdata) {  }
			inline virtual void onAnimationUpdate(LSRuntimeData& rtdata) {  }

			inline bool isAnimating(void) { return m_animating; }
			inline void setAnimationLength(uint16 at) { m_animDelay = at; }
			inline void setFrameDelay(uint16 fd) { m_frameDelay = fd; }
			inline void setLooping(bool l) { m_loop = l; }
			inline uint16 getAnimationLength(void) { return m_animDelay; }
			inline uint16 getFrameDelay(void) { return m_frameDelay; }
			inline bool isLooping(void) { return m_loop; }

		private:
			bool m_animating;
			uint16 m_currentAnimTIme;
			uint16 m_animDelay;
			uint16 m_frameDelay;
			uint16 m_currentTime;
			bool m_loop;
	};
}

#endif