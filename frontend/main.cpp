#include "Runtime.hpp"

int main(int argc, char** argv)
{
	ls::LogicSimApp app;
	ls_menuBar.create();
	ls_toolBar.create("res/dat/icons.dat");
	ls_statusBar.create();
	ls_statusBar.m_iconBar.create("res/dat/icons.dat");
	ls_statusBar.setTexture(ls_statusBar.m_iconBar.getTexture());
	app.init();
	app.drawSplashScreen();
	app.createWindow();
	ls_guiBase.setAutoClear(false);
	ls_guiBase.enableRender(false);
	ls_statusBar.setBounds(ls_statusBar.getx(), ls_rtdata.getScreenSize().y - ls_statusBar.m_height - ls_statusBar.m_borderWidth, ls_rtdata.getScreenSize().x - ls_statusBar.m_borderWidth, ls_statusBar.geth());
	ls_toolBar.setBounds(ls_toolBar.getx(), ls_menuBar.gety() + ls_menuBar.geth(), ls_rtdata.getScreenSize().x - ls_toolBar.m_borderWidth, ls_toolBar.geth());
	ls_menuBar.setBounds(ls_menuBar.getx(), ls_menuBar.gety(), ls_rtdata.getScreenSize().x - ls_menuBar.m_borderWidth, ls_menuBar.geth());
	app.setIcon("res/img/icon.png");
	app.startGameLoop();
	return 0;
}