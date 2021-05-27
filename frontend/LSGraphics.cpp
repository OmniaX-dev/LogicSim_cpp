#include "LSGraphics.hpp"
#include "Renderer.hpp"

namespace ls
{
	LSComplexDrawable::LSComplexDrawable(void)
	{
		m_geometryLayer = m_renderer.addLayer();
		m_gfxLayer = m_renderer.addLayer();
		m_textLayer = m_renderer.addLayer();
		m_autoClear = true;
		m_useGeometryLayer = true;
		m_useGfxLayer = true;
		m_useTextLayer = true;
		m_renderEnabled = true;
		m_sharedGraphics = false;
		m_shared = nullptr;
		m_visible = false;
		setValid(true);
	}
	
	void LSComplexDrawable::share(LSComplexDrawable& sh, bool textl, bool gfxl, bool geol)
	{
		if (sh.isInvalid()) return;
		m_shared = &sh;
		m_sharedGraphics = true;
		m_shareTextLayer = textl;
		m_shareGfxLayer = gfxl;
		m_shareGeometryLayer = geol;
	}

	void LSComplexDrawable::clearRenderer(void)
	{
		if (m_useTextLayer && !isTextLayerShared()) m_textLayer.get().clear();
		if (m_useGfxLayer && !isGfxayerShared()) m_gfxLayer.get().clear();
		if (m_useGeometryLayer && !isGeometryLayerShared()) m_geometryLayer.get().clear();
	}

	void LSComplexDrawable::render(LSRuntimeData& rtdata)
	{
		if (isInvalid() || !isRenderEnabled() || !isVisible()) return;
		if (getAutoClear()) clearRenderer();
		onRender(rtdata);
		if (m_useGeometryLayer && !isGeometryLayerShared()) ls_rtdata.renderer.drawVertexArray(m_geometryLayer.get());
		if (m_useGfxLayer && !isGfxayerShared()) ls_rtdata.renderer.drawVertexArray(m_gfxLayer.get());
		if (m_useTextLayer && !isTextLayerShared()) ls_rtdata.renderer.drawVertexArray(m_textLayer.get());
	}




	Animator::Animator(void)
	{
		m_currentAnimTIme = 0;
		m_currentTime = 0;
		setAnimationLength(60);
		setFrameDelay(1);
		setLooping(false);
		m_animating = false;
	}

	void Animator::startAnimation(void)
	{
		m_animating = true;
		onAnimationStart();
	}

	void Animator::stopAnimaation(void)
	{
		m_animating = false;
		onAnimationEnd();
	}
	
	void Animator::resetAnimation(void)
	{
		m_currentAnimTIme = 0;
		m_currentTime = 0;
		onAnimationReset();
	}
	
	void Animator::restartAnimation(void)
	{
		stopAnimaation();
		resetAnimation();
		startAnimation();
		onAnimationRestart();
	}

	bool Animator::renderAnimation(LSRuntimeData& rtdata)
	{
		if (!isAnimating()) return false;
		updateAnimation(rtdata);
		onAnimationRender(rtdata);
		return true;
	}
	
	bool Animator::updateAnimation(LSRuntimeData& rtdata)
	{
		if (!isAnimating()) return false;
		if (m_currentTime++ < m_frameDelay) return false;
		m_currentTime = 0;
		onAnimationUpdate(rtdata);
		if (m_currentAnimTIme++ >= m_animDelay)
		{
			if (isLooping())
			{
				resetAnimation();
				return true;
			}
			stopAnimaation();
			resetAnimation();
		}
		return true;
	}
	
	
}