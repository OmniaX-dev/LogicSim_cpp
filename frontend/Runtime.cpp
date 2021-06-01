#include "Runtime.hpp"
#include "LSColors.hpp"
#include "LSComponents.hpp"
#include "DataFiles.hpp"
#include "LSDebugConsole.hpp"

namespace ls
{
	void RuntimeState::onInit(GameData& gdata)
	{
		ls_cmd.initialize();
		ls_mainBoard.setup("res/dat/library.dat");

		ls_mainBoard.addComponent("74LS04");
		ls_mainBoard.addComponent("74LS04");
		ls_mainBoard.addComponent("7SDisplayCC");
		ls_mainBoard.addComponent("PowerLine");

		edit.addMenuItem("Copy", nullptr);
		edit.addMenuItem("Paste", nullptr);
		edit.addMenuItem("Crop", nullptr);
		edit.addMenuItem("Revert", nullptr);
		
		file.addMenuItem("Save", nullptr);
		file.addMenuItem("Save as", nullptr);
		file.addMenuItem("Print", nullptr);
		file.addMenuItem("Clear", nullptr);
		file.addMenuItem("Close", nullptr);
		
		help.addMenuItem("Version", nullptr);
		help.addMenuItem("Online page", nullptr);
		help.addMenuItem("About", nullptr);
		
		component.addMenuItem("New", nullptr);
		component.addMenuItem("Delete", nullptr);
		component.addMenuItem("Test", nullptr);
		component.addMenuItem("Reload", nullptr);

		ls_menuBar.addMenu("File", file);
		ls_menuBar.addMenu("Edit", edit);
		ls_menuBar.addMenu("Component", component);
		ls_menuBar.addMenu("Help", help);

		ls_toolBar.addButton(&test_btn_callback, "NewProject", false, "", 0, Object::Invalid, true, "DebugConsole");
		ls_toolBar.addButton(nullptr, "CreateComponent", false, "", 0, Object::Invalid, true, "DebugConsole");
		ls_toolBar.addButton(nullptr, "LastSession", false, "", 0, Object::Invalid, true, "DebugConsole");
		ls_toolBar.addButton(nullptr, "OpenProject", false, "", 0, Object::Invalid, true, "DebugConsole");
		ls_toolBar.addButton(nullptr, "SaveAs", false, "", 0, Object::Invalid, true, "DebugConsole");

		gui::Button& term = ls_statusBar.addButton(&sb_terminal_btn_callback, "Terminal", false, "", 0, Object::Invalid, true, "DebugConsole");
		gui::Button& sav = ls_statusBar.addButton(nullptr, "SaveProject", false, "", 0, Object::Invalid, true, "SaveProject");
		gui::Button& sets = ls_statusBar.addButton(nullptr, "Settings", false, "", 0, Object::Invalid, true, "Settings");

		ls_statusBar.attachPtr(ls_runtime.getBuildNumberPtr(), "Build", Color::Green, 18);

		gdata.application->setClearColor(LSColors::Background);
	}

	void RuntimeState::test_btn_callback(LSRuntimeData& rtdata, Object& sender, Object& data)
	{
		std::cout << "ciao\n";
	}

	void RuntimeState::sb_terminal_btn_callback(LSRuntimeData& rtdata, Object& sender, Object& data)
	{
		if (ls_cmd.isAnimating()) return;
		if (ls_cmd.is_Visible()) ls_cmd.hide();
		else ls_cmd.show();
	}

	void RuntimeState::windowResized(sf::Event& evt, GameData& gdata)
	{
		ls_rtdata.window.setView(sf::View(sf::FloatRect(0, 0, evt.size.width, evt.size.height)));
		ls_statusBar.setBounds(ls_statusBar.getx(), ls_rtdata.getScreenSize().y - ls_statusBar.m_height - ls_statusBar.m_borderWidth, ls_rtdata.getScreenSize().x - ls_statusBar.m_borderWidth, ls_statusBar.geth());
		ls_toolBar.setBounds(ls_toolBar.getx(), ls_menuBar.gety() + ls_menuBar.geth(), ls_rtdata.getScreenSize().x - ls_toolBar.m_borderWidth, ls_toolBar.geth());
		ls_menuBar.setBounds(ls_menuBar.getx(), ls_menuBar.gety(), ls_rtdata.getScreenSize().x - ls_menuBar.m_borderWidth, ls_menuBar.geth());
		ls_sideBar.setBounds(ls_rtdata.getScreenSize().x - ls_sideBar.m_width, ls_menuBar.geth(), ls_sideBar.m_width, ls_rtdata.getScreenSize().y - ls_menuBar.geth() - ls_statusBar.geth());
		ls_cmd.refreshSize();
		ls_mainBoard.m_gridDrawn = false;
	}

	void RuntimeState::mouseDragged(LSEvent& evt, LSRuntimeData& rtdata)
	{
		ls_mainBoard.mouseDragged(evt, rtdata);
	}

	void RuntimeState::onAnyEvent(LSEvent& evt, LSRuntimeData& rtdata)
	{
		ls_pinMenu.handleEvents(evt, rtdata);
		ls_menuBar.handleEvents(evt, rtdata);
		ls_toolBar.handleEvents(evt, rtdata);
		ls_cmd.handleEvents(evt, rtdata);
		ls_statusBar.handleEvents(evt, rtdata);
		ls_mainBoard.handleEvents(evt, rtdata);
		ls_sideBar.handleEvents(evt, rtdata);
	}

	void RuntimeState::onRender(LSRuntimeData& rtdata)
	{
		ls_guiBase.clearRenderer();
		ls_mainBoard.render(rtdata);
		ls_toolBar.render(rtdata);
		ls_menuBar.render(rtdata);
		ls_sideBar.render(rtdata);
		ls_statusBar.render(rtdata);
		ls_cmd.render(rtdata);
		ls_pinMenu.render(rtdata);
		ls_guiBase.render(rtdata);
	}

	void RuntimeState::onUpdate(GameData& rtdata)
	{
		ls_mainBoard.update(rtdata);
		ls_cmd.update(rtdata);
		ls_statusBar.update(rtdata);
		ls_menuBar.update(rtdata);
		ls_pinMenu.update(rtdata);
		ls_toolBar.update(rtdata);
	}

	void RuntimeState::onActivate(GameData& gdata) { RuntimeState::m_ready = true; }

	void RuntimeState::keyPressed(sf::Event& evt, GameData& gdata)
	{
		if (evt.key.code == SF_KEY(F11))
		{
			if (ls_cmd.isAnimating()) return;
			if (ls_cmd.is_Visible()) ls_cmd.hide();
			else ls_cmd.show();
		}
	}
	
	void LogicSimApp::onInit(void)
	{
		DataFile::addGlobal("LSC_LEFT_EDGE", (int32)eLSCPinEdge::Left);
		DataFile::addGlobal("LSC_RIGHT_EDGE", (int32)eLSCPinEdge::Right);
		DataFile::addGlobal("LSC_TOP_EDGE", (int32)eLSCPinEdge::Top);
		DataFile::addGlobal("LSC_BOTTOM_EDGE", (int32)eLSCPinEdge::Bottom);
		
		DataFile::addGlobal("PIN_TYPE_POWER", (int32)eLSCPinType::Power);
		DataFile::addGlobal("PIN_TYPE_GROUND", (int32)eLSCPinType::Ground);
		DataFile::addGlobal("PIN_TYPE_INPUT", (int32)eLSCPinType::Input);
		DataFile::addGlobal("PIN_TYPE_OUTPUT", (int32)eLSCPinType::Output);

		DataFile::addGlobal("SIG_ACTIVE_LOW", (int32)eLSCSIgnalType::ActiveLow);
		DataFile::addGlobal("SIG_ACTIVE_HIGH", (int32)eLSCSIgnalType::ActiveHigh);

		DataFile::addGlobal("LSC_7S_DISPLAY", (int32)eLSCID::_7SDisplayCC);
		DataFile::addGlobal("LSC_BASIC_CHIP", (int32)eLSCID::BasicChip);
		DataFile::addGlobal("LSC_POWER_LINE", (int32)eLSCID::PowerLine);
		

		gdata().initONSP();
		openLoadScript("res/load.dat");
		if (Flags::isset(FLG_DEBUG))
		{
			Flags::set(FLG_SHOW_DEBUG_VIEW);
			Flags::set(FLG_SHOW_COLLISION_RECTS);
		}
		rtsID = m_gameData.gameStateManager.addGameState(rts);
	}

	String Utils::zeroPad(String base, uint16 newLen, char c)
	{
		if (newLen <= base.length()) return base;
		for (uint32 i = base.length(); i < newLen; i++)
			base = base.add(c);
		return base;
	}
}
