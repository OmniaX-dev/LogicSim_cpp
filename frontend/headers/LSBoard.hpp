#ifndef __LS_BOARD_HPP__
#define __LS_BOARD_HPP__

#include "LSDefines.hpp"
#include "DataFiles.hpp"
#include "Gui.hpp"

namespace ls
{
	class LSChip;
	class LSWire;
	class LSBoard : public EventListener, public Object
	{
		public:
			inline LSBoard(void) { setValid(false); }
			~LSBoard(void);
			void setup(String libraryFile);
			LSChip& addComponent(String componentName, FPoint position = FPoint(-1, -1));
			virtual void onEvent(LSEvent& evt, LSRuntimeData& rtdata);
			void update(LSRuntimeData& rtdata);
			void render(LSRuntimeData& rtdata);
			virtual void mouseMoved(LSEvent& evt, LSRuntimeData& rtdata);
			virtual void mouseDragged(LSEvent& evt, LSRuntimeData& rtdata);
			virtual void mousePressed(LSEvent& evt, LSRuntimeData& rtdata);
			FPoint convertToGrid(FPoint p, FPoint offset = FPoint(0, 0));
			void refreshAllComponents(void);
			LSChip& getChipByID(uint64 oid);

		private:
			void drawGrid(void);
			
		public:
			LSCLibrary m_library;
			inline static LSRuntimeData& m_rtdata = LSRuntimeData::instance();
			std::vector<sh_ptr(LSChip)> m_components;
			std::vector<LSWire> m_wires;
			sh_ptr(LSChip) m_selected;

			LSWire* m_connectingWire;

			Layer m_gfxLayer;
			Layer m_geometryLayer;
			Layer m_textLayer;
			Layer m_gridLayer;
			bool m_gridDrawn;
			LayerRenderer m_lrenderer;
			bool m_connectingMode;
			FPoint m_lastMousePos;
			bool m_clickDrag;
			FPoint m_dragDiff;

			uint8 m_pinSpacing;

			friend class LSComponent;
			friend class LSComponentSheetFile;
	};
}

#endif