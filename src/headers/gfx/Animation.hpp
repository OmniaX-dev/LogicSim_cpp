#ifndef __ANIMATION__HPP__
#define __ANIMATION__HPP__

#include <SFML/Graphics.hpp>
#include "Drawable.hpp"
#include "DataTypes.hpp"
#include "Image.hpp"

namespace Omnia
{
	class VertexArray;
	class GameData;
	class Animation : public Drawable, public Object
	{
	public:
		Animation(void);
		Animation(AnimationData ad);
		void create(AnimationData ad);

		virtual void update(GameData& gdata);
		virtual void render(GameData& gdata, VertexArray& layer, Color tintColor = Color::White);

		void resetAnimation(void);

		inline void setFrameCount(int n){m_animData.frameCount = n;}
		inline void setColumnNumber(int n){m_animData.columns = n;}
		inline void playBackwards(bool b){m_animData.playBackwards = b;}
		inline void setDelay(int d){m_animData.delay = d;}
		inline void setColumnOffset(int o){m_animData.columnOffset = o;}
		inline void setRowOffset(int o){m_animData.rowOffset = o;}
		inline void setStill(bool s){m_animData.isStill = s;}
		inline void setGeneralOffsetX(int gox) { m_animData.generalOffsetX = gox; }
		inline void setGeneralOffsetY(int goy) { m_animData.generalOffsetY = goy; }
		inline void setTintColor(Color tint) { m_texInfo.tintColor = tint; }
		inline void setName(String name) { m_name = name; }
		inline void setFrameSize(IPoint fs) { m_animData.frameWidth = fs.x; m_animData.frameHeight = fs.y; }

		inline int getFrameCount(void){return m_animData.frameCount;}
		inline int getColumnNumber(void){return m_animData.columns;}
		inline bool isPlayBackwardsEnabled(void){return m_animData.playBackwards;}
		inline int getDelay(void){return m_animData.delay;}
		inline int getColumnOffset(void){return m_animData.columnOffset;}
		inline int getRowOffset(void){return m_animData.rowOffset;}
		inline bool isStill(void){return m_animData.isStill;}
		inline int getGeneralOffsetX(void) { return m_animData.generalOffsetX; }
		inline int getGeneralOffsetY(void) { return m_animData.generalOffsetY; }
		inline Color getTintColor(void) { return m_texInfo.tintColor; }
		inline String getName(void) { return m_name; }
		inline AnimationData getAnimationData(void) { return m_animData; }
		inline AnimationData& getAnimDataRef(void) { return m_animData; }
		inline IPoint getFrameSize(void) { return IPoint(m_animData.frameWidth, m_animData.frameHeight); }

	private:
		int m_current_time;
		int m_current_frame;
		bool m_back;
		AnimationData m_animData;
		TexInfo m_texInfo;
		String m_name;

	private:
		inline static int32 s_nextID = 0;
	};
}
#endif
