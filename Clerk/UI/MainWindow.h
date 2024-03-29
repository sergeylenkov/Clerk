#pragma once

#include <wx/wx.h>
#include <wx/splitter.h>
#include <wx/stdpaths.h>
#include <wx/fileconf.h>
#include <wx/dataview.h>
#include <wx/display.h>
#include <memory>
#include "../Defines.h"
#include "../Data/DataContext.h"
#include "../Utils/Settings/Settings.h"
#include "../Utils/Icons.h"
#include "../Commands/CommandsInvoker.h"
#include "../Commands/CommandsReceiver.h"
#include "Controllers/DialogsController.h"
#include "Tabs/TabsPanel.h"
#include "TreeMenu/TreeMenu.h"
#include "NewTransactionButton/NewTransactionButton.h"
#include "Statusbar/Statusbar.h"
#include "MainMenu/MainMenu.h"
#include "Notifications/NotificationsButton.h"

using namespace Clerk::Data;
using namespace Clerk::Utils;
using namespace Clerk::UI;
using namespace Clerk::Commands;

class MainWindow : public wxFrame
{
public:
	MainWindow(DataContext& context, Icons& icons);
	~MainWindow();

private:
	DataContext& _context;
	Icons& _icons;
	wxPanel* _toolbar;
	MainMenu* _mainMenu;
	Statusbar* _statusbar;
	TreeMenu* _treeMenu;	
	TabsPanel* _tabsPanel;
	wxSplitterWindow* _splitter;
	NewTransactionButton* _newTransactionButton;
	NotificationsButton* _notificationButton;
	CommandsInvoker* _commandsInvoker;
	CommandsReceiver* _commandsReceiver;
	DialogsController* _dialogsController;
	StatusbarViewModel* _statusViewModel;

	void SetupCommands();
	void UpdateStatus();
};