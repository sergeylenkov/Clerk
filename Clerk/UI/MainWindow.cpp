#include "MainWindow.h"

MainWindow::MainWindow(DataContext& context, Icons& icons): wxFrame((wxFrame *)NULL, -1, "Clerk"), _context(context), _icons(icons)
{
	this->SetIcon(wxICON(APP_ICON));
	
	if (!Settings::GetInstance().GetWindowIsMaximized())
	{
		this->SetSize(wxSize(Settings::GetInstance().GetWindowWidth(), Settings::GetInstance().GetWindowHeight()));		
	}
	else {
		this->Maximize();
	}

	int activeDisplay = Settings::GetInstance().GetActiveDisplay();

	if (activeDisplay != 0 && activeDisplay < wxDisplay::GetCount()) {
		this->Move(wxDisplay(activeDisplay).GetClientArea().GetPosition());
	}

	SetupCommands();

	TreeMenuViewModel* treeViewModel = new TreeMenuViewModel(_context.GetAccountsService(), _context.GetReportsService(), _context.GetTransactionsService());
	StatusViewModel* statusViewModel = new StatusViewModel(_context.GetAccountingService(), _context.GetExchangeRatesRepository(), _context.GetCurrenciesService(), Settings::GetInstance().GetSelectedExchangeRates());
	TransactionsMenuViewModel* mainMenuViewModel = new TransactionsMenuViewModel(_context.GetTransactionsService());
	TransactionsMenuViewModel* addButtonViewModel = new TransactionsMenuViewModel(_context.GetTransactionsService());

	_mainMenu = new MainMenu(*_commandsInvoker);
	_mainMenu->SetViewModel(mainMenuViewModel);

	this->SetMenuBar(_mainMenu);

	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

	_toolbar = new wxPanel(this, wxID_ANY, wxDefaultPosition, this->FromDIP(wxDefaultSize), wxBORDER_NONE);
	_toolbar->SetBackgroundColour(wxColour(255, 255, 255));

	wxBoxSizer* horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	_newTransactionButton = new NewTransactionButton(_toolbar, *_commandsInvoker, _icons);
	_newTransactionButton->SetViewModel(addButtonViewModel);

	horizontalSizer->Add(_newTransactionButton, 0, wxALL, 5);

	_toolbar->SetSizer(horizontalSizer);
	_toolbar->Layout();

	mainSizer->Add(_toolbar, 0, wxEXPAND, 0);
	
	wxSplitterWindow* splitter = new wxSplitterWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_NOBORDER);
	splitter->SetSashGravity(0.5);
	splitter->SetMinimumPaneSize(300);

	mainSizer->Add(splitter, 1, wxEXPAND, 5);

	wxPanel* splitterLeftPanel = new wxPanel(splitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE);
	splitterLeftPanel->SetBackgroundColour(wxColour(245, 245, 245, 1));
	
	wxBoxSizer* boxSizer = new wxBoxSizer(wxVERTICAL);	

	_treeMenu = new TreeMenu(splitterLeftPanel, _icons, *_commandsInvoker);
	_treeMenu->SetViewModel(treeViewModel);

	boxSizer->Add(_treeMenu, 1, wxEXPAND | wxALL, 0);
	splitterLeftPanel->SetSizer(boxSizer);		

	wxPanel* splitterRightPanel = new wxPanel(splitter);	

	wxBoxSizer* rightPanelSizer = new wxBoxSizer(wxVERTICAL);
	splitterRightPanel->SetSizer(rightPanelSizer);

	_tabsPanel = new TabsPanel(splitterRightPanel, _context, *_commandsInvoker, _icons);

	rightPanelSizer->Add(_tabsPanel, 1, wxEXPAND | wxALL, 0);
	rightPanelSizer->Layout();

	splitter->SplitVertically(splitterLeftPanel, splitterRightPanel, 1);	

	_statusbar = new Statusbar(this, wxID_ANY, wxDefaultPosition, this->FromDIP(wxSize(-1, 20)));	
	_statusbar->SetViewModel(statusViewModel);

	mainSizer->Add(_statusbar, 0, wxEXPAND, 0);

	this->SetSizer(mainSizer);
	this->Layout();

	this->Centre(wxBOTH);	

	_treeMenu->RestoreState();

	_dialogsController->SetMainWindow(this);
	_tabsController->SetTabsPanel(_tabsPanel);

	_tabsController->RestoreLastTabs();

	/*std::thread([this]()
	{
		std::this_thread::sleep_for(std::chrono::seconds(3));		
		this->GetEventHandler()->CallAfter(&MainFrame::CheckSchedulers);
	}).detach();

	std::thread([this]()
	{
		std::this_thread::sleep_for(std::chrono::seconds(5));
		this->GetEventHandler()->CallAfter(&MainFrame::CheckAlerts);
	}).detach();

	if (Settings::GetInstance().IsLoadExchangeRates()) {
		_statusbar->SetExchangeRates("Updating...");		

		std::thread([this]()
		{
			//TODO
			//UpdateExchangeRates();			
			//DataHelper::GetInstance().ReloadExchangeRate();

			std::this_thread::sleep_for(std::chrono::seconds(3));

			this->GetEventHandler()->CallAfter(&MainFrame::UpdateStatus);
		}).detach();
	}*/
}

MainWindow::~MainWindow() 
{
	delete _treeMenu;
	delete _tabsPanel;
	delete _commandsReceiver;
	delete _commandsInvoker;
	delete _dialogsController;
	delete _tabsController;

	Settings::GetInstance().SetWindowWidth(this->GetSize().GetWidth());
	Settings::GetInstance().SetWindowHeight(this->GetSize().GetHeight());
	Settings::GetInstance().SetWindowIsMaximized(this->IsMaximized());
	Settings::GetInstance().SetActiveDisplay(wxDisplay::GetFromWindow(this));

	Settings::GetInstance().Save();
}

void MainWindow::SetupCommands() {
	_dialogsController = new DialogsController(_context, _icons);
	_tabsController = new TabsController(_context, _icons);

	_commandsReceiver = new CommandsReceiver(this, _dialogsController, _tabsController);

	QuitCommand* quitCommand = new QuitCommand(_commandsReceiver);
	PreferencesCommand* preferencesCommand = new PreferencesCommand(_commandsReceiver);
	AboutCommand* aboutCommand = new AboutCommand(_commandsReceiver);
	NewTransactionCommand* newTransactionCommand = new NewTransactionCommand(_commandsReceiver);
	CopyTransactionCommand* copyTransactionCommand = new CopyTransactionCommand(_commandsReceiver);
	SplitTransactionCommand* splitTransactionCommand = new SplitTransactionCommand(_commandsReceiver);
	EditTransactionCommand* editTransactionCommand = new EditTransactionCommand(_commandsReceiver);
	DeleteTransactionCommand* deleteTransactionCommand = new DeleteTransactionCommand(_commandsReceiver);
	NewAccountCommand* newAccountCommand = new NewAccountCommand(_commandsReceiver);
	EditAccountCommand* editAccountCommand = new EditAccountCommand(_commandsReceiver);
	NewTabCommand* newTabCommand = new NewTabCommand(_commandsReceiver);
	NewAccountTabCommand* newAccountTabCommand = new NewAccountTabCommand(_commandsReceiver);
	NewAccountsTabCommand* newAccountsTabCommand = new NewAccountsTabCommand(_commandsReceiver);

	_commandsInvoker = new CommandsInvoker(*quitCommand, *preferencesCommand, *aboutCommand,
		                                   *newTransactionCommand, *copyTransactionCommand,
										   *splitTransactionCommand, *editTransactionCommand, *deleteTransactionCommand,
		                                   *newAccountCommand, *editAccountCommand, *newTabCommand, *newAccountTabCommand, *newAccountsTabCommand);

	_context.SetCommandsInvoker(_commandsInvoker);
}