#ifndef __LS_COMPONENT_HPP__
#define __LS_COMPONENT_HPP__

#include "LSDefines.hpp"
#include "LSColors.hpp"
#include "Gui.hpp"

namespace ls
{
	#define LSCHD_Left 0
	#define LSCHD_Right 1
	#define LSCHD_Up 2
	#define LSCHD_Down 3
	#define LSCHD_Count 4

	class LSChip;
	class LSLogicFunction;
	class LSPinComponent;

	class LSPin : public Object, public Drawable
	{
		public:
			String label;
			eLSCPinEdge edge;
			eLSCPinType type;
			eLSCSIgnalType signalType;
			uint8 edgePosition;
			String function;
			LSLogicFunction* func;
			LSChip* parent;

			bool signal;

			friend class LSChip;
			friend class LSComponentSheetFile;
			friend class LSPinComponent;
			friend class LSComponent;
			friend class LSCLibrary;
			friend class LSWire;
	};

	class LSBoard;
	class LSWire : public Object
	{
		public:
			LSWire(void);
			LSWire(LSPin& first, LSPin& second);
			LSWire(LSPin& first);
			void setup(Color color = LSColors::DefaultWire, bool connecting = false, uint8 width = 2, FPoint mousePos = FPoint(0, 0));
			void render(LSRuntimeData& rtdata, Layer& layer);
			void update(LSRuntimeData& rtdata);
			//void renderWire(LSRuntimeData& rtdata, Layer& layer);
			void renderWire(LSRuntimeData& rtdata, Layer& layer);
			void adjustPosition(void);
			void drawSquareLine(LSRuntimeData& rtdata, Layer& layer, FPoint p1, FPoint p2, Color color, bool addToList = false);
			//FRect getCoords(void);
			void connect(LSPin& second);
			void disconnect(void);
			inline void setMousePosition(IPoint mpos) { m_mousePos = FPoint(mpos); }
			void addSegment(FPoint endPos);
			static FRect makeSegment(FPoint start, FPoint end, eLSOrientation ori = eLSOrientation::Horizontal, float w = 1);
			static std::vector<FPoint> makeLink(FPoint start, FPoint end);
			static FRect newSegment(FPoint start, FPoint end, float w = 1, eLSOrientation* ori = nullptr);

		private:
			Color m_color;
			LSPin* m_first;
			LSPin* m_second;
			FPoint m_mousePos;
			bool m_connecting;
			uint8 m_wireWidth;
			LSBoard* m_board;

			inline static const FPoint s_bugOffset = FPoint(1, 0);

			friend class LSChip;
			friend class LSPin;
			friend class LSComponentSheetFile;
			friend class LSPinComponent;
			friend class LSComponent;
			friend class LSBoard;
			friend class LSCLibrary;
	};

	class LSChip : public Object
	{
		public:
			LSChip(void);
			LSChip(const LSChip& copy);
			inline void setName(String name) { m_name = name; }
			inline String getName(void) { return m_name; }
			inline void setCOmponentImage(String imgName) { m_componentImage = imgName; }
			inline String getComponentImage(void) { return m_componentImage; }
			inline uint16 getPinCount(void) { return m_pinCount; }
			inline const LSPin& vccPin(void) { return (m_pinVcc != nullptr ? *m_pinVcc : INVALID_REF(LSPin)); }
			inline const LSPin& gndPin(void) { return (m_pinGnd != nullptr ? *m_pinGnd : INVALID_REF(LSPin)); }
			inline LSPinComponent& getComponent(void) { return (m_component != nullptr ? *m_component : INVALID_REF(LSPinComponent)); }
			inline const std::vector<LSPin>& getPinList(void) { return m_pins; }
			inline uint32 getComponentID(void) { return m_componentID; }
			const LSPin& getPin(String label);
			inline static uint32 nextComponentID(void) { return LSChip::s_nextComponentID++; }

		private:
			String m_name;
			uint16 m_pinCount;
			LSPin* m_pinVcc;
			LSPin* m_pinGnd;
			std::vector<LSPin> m_pins;
			sh_ptr(LSPinComponent) m_component;
			String m_componentImage;
			uint16 m_nPinsTop;
			uint16 m_nPinsBottom;
			uint16 m_nPinsLeft;
			uint16 m_nPinsRight;

			uint32 m_componentID;

			inline static uint32 s_nextComponentID = 0;

			friend class LSComponentSheetFile;
			friend class LSBoard;
			friend class LSComponent;
			friend class LSPinComponent;
			friend class LS7SegmentDisplayCC;
			friend class LSLogicFunction;
			friend class LSCLibrary;
			friend class LSWire;
			friend class LSPowerLine;
	};

	class LSComponentBase : public Drawable, public Object, public EventListener
	{
		public:
			inline LSComponentBase(void) { invalidate(); }
			LSComponentBase(Layer& boxLayer, LSBoard& board);
			void resetMouseDir(bool value = false);
			void adjustPosition(LSEvent& evt, LSRuntimeData& rtdata);
			inline virtual void update(LSRuntimeData& rtdata) {  }
			inline virtual void render(GameData& gdata, Layer& layer) {  }
			virtual void mousePressed(LSEvent& evt, LSRuntimeData& data);
			virtual void mouseReleased(LSEvent& evt, LSRuntimeData& data);
			virtual void mouseDragged(LSEvent& evt, LSRuntimeData& data);
			virtual void mouseMoved(LSEvent& evt, LSRuntimeData& data);
			void updateMouseDirection(LSEvent& evt);
			virtual void init(void) {  }

			virtual void onMousePressed(LSEvent& evt, LSRuntimeData& rtdata) {  }
			virtual void onMouseReleased(LSEvent& evt, LSRuntimeData& rtdata) {  }
			virtual void onMouseDragged(LSEvent& evt, LSRuntimeData& rtdata) {  }
			virtual void onMouseMoved(LSEvent& evt, LSRuntimeData& rtdata) {  }

		protected:
			Layer* m_boxLayer;
			LSBoard* m_board;
			bool m_dirChanged[LSCHD_Count];
			IPoint m_lastMousePos;
			IPoint m_pressPos;
			IPoint m_clickDiff;
			bool m_clickInside;
			bool m_selected;

			friend class LSBoard;
	};

	class LSChip;
	class LSPin;
	class LSWire;
	class LSPinComponent : public LSComponentBase
	{
		public:
			inline LSPinComponent(void) { invalidate(); }
			LSPinComponent(Layer& boxLayer, LSBoard& board);
			void setup(LSRuntimeData& rtdata);
			void adjustPinPosition(void);
			virtual void mouseMoved(LSEvent& evt, LSRuntimeData& data);
			virtual void mouseDragged(LSEvent& evt, LSRuntimeData& rtdata);
			virtual void mousePressed(LSEvent& evt, LSRuntimeData& data);
			virtual void render(LSRuntimeData& rtdata, Layer& layer);
			virtual void update(LSRuntimeData& rtdata);
			inline virtual void onRender(GameData& gdata, Layer& layer) {  }
			virtual void pinMousePressed(LSEvent& evt, LSRuntimeData& rtdata, LSPin& sender);
			void refreshPins(void);

			inline virtual bool vccSignal(void) { return (m_chip->vccPin().isValid() ? m_chip->vccPin().signal : false); }
			inline virtual bool gndSignal(void) { return (m_chip->gndPin().isValid() ? m_chip->gndPin().signal : true); }

			void loadPinContextMenu(void);
			static void component_menu_callback(LSRuntimeData& rtdata, Object& sender, Object& data);

		protected:
			sh_ptr(LSChip) m_chip;
			uint8 m_pinWidth;
			uint8 m_pinLength;
			FRect m_selPinBounds;
			uint8 m_borderWidth;
			LSPin* m_pinHover;

			uint16 m_minWidth;
			uint16 m_minHeight;

			Color m_customBgCol;
			bool m_useCustomBg;

			gui::ContextMenuTemplate m_pinMenuTemplate;

			friend class LSBoard;

	};

	class LSComponent : public LSPinComponent
	{
		public:
			inline LSComponent(void) : LSPinComponent() { invalidate(); }
			LSComponent(AnimationData ad, Layer& boxLayer, LSBoard& board);
			virtual void update(GameData& gdata);
			virtual void onRender(LSRuntimeData& rtdata, Layer& layer);

		private:
			Animation m_sprite;
			FPoint m_defaultSize;
			float m_defaultScale;

			friend class LSBoard;
	};

	class LS7SegmentDisplayCC : public LSPinComponent
	{
		public:
			inline LS7SegmentDisplayCC(void) : LSPinComponent() { invalidate(); }
			LS7SegmentDisplayCC(Layer& boxLayer, LSBoard& board);
			void init(void);
			virtual void update(GameData& gdata);
			virtual void onRender(GameData& gdata, Layer& layer);
			inline bool vccSignal(void) { return false; }

		private:
			uint8 m_borderWidth;
			bool m_segmentStatus[8];
			std::vector<Drawable> m_segments;
			uint8 m_segw;
			uint8 m_segl;
			bool m_useCustomSegColor;
			Color m_customSegColor_h;
			Color m_customSegColor_l;
			uint8 m_segSpacing;

			friend class LSBoard;
	};

	class LSPowerLine : public LSPinComponent
	{
		public:
			inline LSPowerLine(void) : LSPinComponent() { invalidate(); }
			LSPowerLine(Layer& boxLayer, LSBoard& board);
			void init(void);
			virtual void update(GameData& gdata);
			virtual void onRender(GameData& gdata, Layer& layer);
			inline bool vccSignal(void) { return true; }
			inline bool gndSignal(void) { return false; }

		private:
			uint8 m_borderWidth;

			friend class LSBoard;
	};
}

#endif