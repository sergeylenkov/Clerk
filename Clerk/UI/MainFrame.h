#pragma once

#include <wx/wx.h>
#include <wx/splitter.h>
#include <wx/stdpaths.h>
#include <wx/fileconf.h>
#include <wx/dataview.h>
#include <memory>
#include <thread>
#include "../Data/DataContext.h"
#include "../Defines.h"
#include "../Data/Settings.h"
#include "../Data/ExchangeRates/CBRRatesLoader.h"
#include "TreeMenu/Menu.h"
#include "Transactions/TransactionDialog.h"
#include "AccountDialog.h"
#include "Budgets/BudgetDialog.h"
#include "Schedulers/SchedulerDialog.h"
#include "Schedulers/SchedulersConfirmDialog.h"
#include "Goals/GoalDialog.h"
#include "Alerts/AlertDialog.h"
#include "Alerts/AlertsConfirmDialog.h"
#include "TabsPanel.h"
#include "Controls/AddTransactionButton.h"
#include "Controls/Statusbar.h"
#include "Controls/MainMenu.h"
#include "../Utils/Icons.h"
#include "../Commands/CommandsInvoker.h"
#include "../Commands/CommandsReceiver.h"
#include "../Commands/QuitCommand.h"
#include "../Commands/PreferencesCommand.h"
#include "Controllers/DialogsController.h"

using namespace Clerk::Data;
using namespace Clerk::Utils;
using namespace Clerk::UI;
using namespace Clerk::Commands;

class MainFrame : public wxFrame
{
public:
	MainFrame(DataContext& context, Icons& icons);
	~MainFrame();

private:
	DataContext& _context;
	Icons& _icons;
	wxPanel* _toolbar;
	MainMenu* _mainMenu;
	Statusbar* _statusbar;
	TreeMenu* _treeMenu;	
	TabsPanel* _tabsPanel;
	AddTransactionButton* _addTransactionButton;
	CommandsInvoker* _commandsInvoker;
	CommandsReceiver* _commandsReceiver;
	DialogsController* _dialogsController;

	void UpdateStatus();
	std::vector<std::shared_ptr<TransactionViewModel>> OnTreeMenuContextMenu(const AccountViewModel& account);
};