#include "MainWindow.h"

MainWindow::MainWindow(DataContext& context, Icons& icons): wxFrame((wxFrame *)NULL, -1, APP_NAME),
	_context(context),
	_icons(icons)
{
	SetIcon(wxICON(APP_ICON));
	
	if (!Settings::GetInstance().GetWindowIsMaximized())
	{
		SetSize(wxSize(Settings::GetInstance().GetWindowWidth(), Settings::GetInstance().GetWindowHeight()));		
	}
	else {
		Maximize();
	}

	int activeDisplay = Settings::GetInstance().GetActiveDisplay();

	if (activeDisplay != 0 && activeDisplay < wxDisplay::GetCount()) {
		Move(wxDisplay(activeDisplay).GetClientArea().GetPosition());
	}

	SetupCommands();

	TreeMenuViewModel* treeViewModel = new TreeMenuViewModel(_context.GetAccountsService(), _context.GetReportsService(), _context.GetTransactionsService());
	_statusViewModel = new StatusbarViewModel(_context.GetAccountingService(), _context.GetCurrenciesService(), Settings::GetInstance().GetSelectedExchangeRates());
	MainMenuViewModel* mainMenuViewModel = new MainMenuViewModel(_context.GetTransactionsService());
	MainMenuViewModel* addButtonViewModel = new MainMenuViewModel(_context.GetTransactionsService());
	NotificationsViewModel* notificationsViewModel = new NotificationsViewModel(_context.GetAlertsService(), _context.GetTransactionsService(), _context.GetSchedulersService());

	_mainMenu = new MainMenu(*mainMenuViewModel, *_commandsInvoker, _icons);

	SetMenuBar(_mainMenu);

	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

	_toolbar = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE);
	_toolbar->SetBackgroundColour(wxColour(255, 255, 255));

	wxBoxSizer* horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	_newTransactionButton = new NewTransactionButton(
		*addButtonViewModel,
		*_commandsInvoker,
		_icons,		
		_toolbar,
		wxDefaultPosition,
		FromDIP(wxSize(-1, 34))		
	);
	
	horizontalSizer->Add(_newTransactionButton, 0, wxALIGN_CENTER_VERTICAL | wxALL, FromDIP(2));
	horizontalSizer->Add(0, 1, wxEXPAND);

	_notificationButton = new NotificationsButton(*notificationsViewModel, _icons, _toolbar, wxDefaultPosition, FromDIP(wxSize(34, 34)));
	horizontalSizer->Add(_notificationButton, 0, wxALIGN_CENTER_VERTICAL | wxALL, FromDIP(2));

	_toolbar->SetSizer(horizontalSizer);
	_toolbar->Layout();

	mainSizer->Add(_toolbar, 0, wxEXPAND);
	
	_splitter = new wxSplitterWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_NOBORDER);
	_splitter->SetSashGravity(0);
	_splitter->SetMinimumPaneSize(300);

	mainSizer->Add(_splitter, 1, wxEXPAND, FromDIP(5));

	wxPanel* splitterLeftPanel = new wxPanel(_splitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE);
	splitterLeftPanel->SetBackgroundColour(wxColour(245, 245, 245, 1));
	
	wxBoxSizer* boxSizer = new wxBoxSizer(wxVERTICAL);	

	_treeMenu = new TreeMenu(*treeViewModel, *_commandsInvoker, _icons, splitterLeftPanel);

	boxSizer->Add(_treeMenu, 1, wxEXPAND);
	splitterLeftPanel->SetSizer(boxSizer);		

	wxPanel* splitterRightPanel = new wxPanel(_splitter);

	wxBoxSizer* rightPanelSizer = new wxBoxSizer(wxVERTICAL);
	splitterRightPanel->SetSizer(rightPanelSizer);

	_tabsPanel = new TabsPanel(splitterRightPanel, _context, *_commandsInvoker, _icons);

	rightPanelSizer->Add(_tabsPanel, 1, wxEXPAND);
	rightPanelSizer->Layout();

	_splitter->SplitVertically(splitterLeftPanel, splitterRightPanel, 1);	

	_statusbar = new Statusbar(*_statusViewModel, _icons, this, wxDefaultPosition, FromDIP(wxSize(-1, 20)));

	mainSizer->Add(_statusbar, 0, wxEXPAND);

	_splitter->SetSashPosition(Settings::GetInstance().GetTreeMenuWidth());

	SetSizer(mainSizer);
	Layout();

	Centre(wxBOTH);	

	_dialogsController->SetMainWindow(this);
	_tabsController->SetTabsPanel(_tabsPanel);

	_tabsController->RestoreLastTabs();

	if (Settings::GetInstance().IsLoadExchangeRates()) {
		_statusViewModel->SetIsExchangeRatesLoading(true);

		std::thread([this]()
		{
			_context.GetCurrenciesService().UpdatedExchangeRates();

			std::this_thread::sleep_for(std::chrono::seconds(3));

			GetEventHandler()->CallAfter(&MainWindow::UpdateStatus);
		}).detach();
	}
}

MainWindow::~MainWindow() 
{
	delete _treeMenu;
	delete _tabsPanel;
	delete _commandsReceiver;
	delete _commandsInvoker;
	delete _dialogsController;
	delete _tabsController;

	Settings::GetInstance().SetWindowWidth(GetSize().GetWidth());
	Settings::GetInstance().SetWindowHeight(GetSize().GetHeight());
	Settings::GetInstance().SetWindowIsMaximized(IsMaximized());
	Settings::GetInstance().SetActiveDisplay(wxDisplay::GetFromWindow(this));
	Settings::GetInstance().SetTreeMenuWidth(_splitter->GetSashPosition());

	Settings::GetInstance().Save();
}

void MainWindow::SetupCommands() {
	_dialogsController = new DialogsController(_context, _icons);
	_tabsController = new TabsController(_context, _icons);

	_commandsReceiver = new CommandsReceiver(_dialogsController, _tabsController);

	QuitCommand* quitCommand = new QuitCommand(*this);
	OpenPreferencesCommand* preferencesCommand = new OpenPreferencesCommand(*_commandsReceiver);
	AboutCommand* aboutCommand = new AboutCommand(*_commandsReceiver);
	OpenTabCommand* openTabCommand = new OpenTabCommand(*_commandsReceiver);
	OpenAccountTabCommand* openAccountTabCommand = new OpenAccountTabCommand(*_commandsReceiver);
	OpenAccountsTabCommand* openAccountsTabCommand = new OpenAccountsTabCommand(*_commandsReceiver);
	OpenReportTabCommand* openReportTabCommand = new OpenReportTabCommand(*_commandsReceiver);
	NewTransactionCommand* newTransactionCommand = new NewTransactionCommand(*_commandsReceiver);
	CopyTransactionCommand* copyTransactionCommand = new CopyTransactionCommand(*_commandsReceiver);
	SplitTransactionCommand* splitTransactionCommand = new SplitTransactionCommand(*_commandsReceiver);
	EditTransactionCommand* editTransactionCommand = new EditTransactionCommand(*_commandsReceiver);
	DeleteTransactionCommand* deleteTransactionCommand = new DeleteTransactionCommand(_context.GetTransactionsService());
	DuplicateTransactionCommand* duplicateTransactionCommand = new DuplicateTransactionCommand(_context.GetTransactionsService());
	NewAccountCommand* newAccountCommand = new NewAccountCommand(*_commandsReceiver);
	EditAccountCommand* editAccountCommand = new EditAccountCommand(*_commandsReceiver);
	ArchiveAccountCommand* archiveAccountCommand = new ArchiveAccountCommand(_context.GetAccountsService());
	RestoreAccountCommand* restoreAccountCommand = new RestoreAccountCommand(_context.GetAccountsService());
	NewAlertCommand* newAlertCommand = new NewAlertCommand(*_commandsReceiver);
	EditAlertCommand* editAlertCommand = new EditAlertCommand(*_commandsReceiver);
	DeleteAlertCommand* deleteAlertCommand = new DeleteAlertCommand(_context.GetAlertsService());
	NewBudgetCommand* newBudgetCommand = new NewBudgetCommand(*_commandsReceiver);
	EditBudgetCommand* editBudgetCommand = new EditBudgetCommand(*_commandsReceiver);
	DeleteBudgetCommand* deleteBudgetCommand = new DeleteBudgetCommand(_context.GetBudgetsService());
	NewSchedulerCommand* newSchedulerCommand = new NewSchedulerCommand(*_commandsReceiver);
	EditSchedulerCommand* editSchedulerCommand = new EditSchedulerCommand(*_commandsReceiver);
	DeleteSchedulerCommand* deleteSchedulerCommand = new DeleteSchedulerCommand(_context.GetSchedulersService());
	RunSchedulerCommand* runSchedulerCommand = new RunSchedulerCommand(_context.GetSchedulersService());
	PauseSchedulerCommand* pauseSchedulerCommand = new PauseSchedulerCommand(_context.GetSchedulersService());
	NewGoalCommand* newGoalCommand = new NewGoalCommand(*_commandsReceiver);
	EditGoalCommand* editGoalCommand = new EditGoalCommand(*_commandsReceiver);
	DeleteGoalCommand* deleteGoalCommand = new DeleteGoalCommand(_context.GetGoalsService());

	_commandsInvoker = new CommandsInvoker(
		*quitCommand,
		*preferencesCommand,
		*aboutCommand,
		*openTabCommand,
		*openAccountTabCommand,
		*openAccountsTabCommand,
		*openReportTabCommand,		
		*newTransactionCommand, 
		*copyTransactionCommand,
		*splitTransactionCommand, 
		*editTransactionCommand, 
		*deleteTransactionCommand,
		*duplicateTransactionCommand,
		*newAccountCommand, 
		*editAccountCommand,
		*archiveAccountCommand,
		*restoreAccountCommand,
		*newAlertCommand,
		*editAlertCommand,
		*deleteAlertCommand,
		*newBudgetCommand,
		*editBudgetCommand,
		*deleteBudgetCommand,
		*newSchedulerCommand,
		*editSchedulerCommand,
		*deleteSchedulerCommand,
		*runSchedulerCommand,
		*pauseSchedulerCommand,
		*newGoalCommand,
		*editGoalCommand,
		*deleteGoalCommand
	);

	_context.SetCommandsInvoker(_commandsInvoker);
}

void MainWindow::UpdateStatus() {
	_statusViewModel->SetIsExchangeRatesLoading(false);
}