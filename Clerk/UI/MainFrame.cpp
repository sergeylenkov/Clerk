﻿#include "MainFrame.h"

MainFrame::MainFrame(DataContext& context, Icons& icons): wxFrame((wxFrame *)NULL, -1, ""), _context(context), _icons(icons)
{
	this->SetTitle("Clerk");
	this->SetSize(wxSize(Settings::GetInstance().GetWindowWidth(), Settings::GetInstance().GetWindowHeight()));
	this->SetIcon(wxICON(APP_ICON));
	
	_dialogsController = new DialogsController(this);

	_commandsReceiver = new CommandsReceiver(this, _dialogsController);

	QuitCommand* quitCommand = new QuitCommand(_commandsReceiver);
	PreferencesCommand* preferencesCommand = new PreferencesCommand(_commandsReceiver);
	AboutCommand* aboutCommand = new AboutCommand(_commandsReceiver);

	_commandsInvoker = new CommandsInvoker(*quitCommand, *preferencesCommand, *aboutCommand);

	_mainMenu = new MainMenu(*_commandsInvoker);

	this->SetMenuBar(_mainMenu);

	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

	_toolbar = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE);
	_toolbar->SetBackgroundColour(wxColour(255, 255, 255));

	wxBoxSizer* horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	_addTransactionButton = new AddTransactionButton(_toolbar);
	horizontalSizer->Add(_addTransactionButton, 0, wxALL, 5);

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

	_treeMenu = new TreeMenu(splitterLeftPanel, &_icons);
	_treeMenu->OnContextMenu = std::bind(&MainFrame::OnTreeMenuContextMenu, this, std::placeholders::_1);

	boxSizer->Add(_treeMenu, 1, wxEXPAND | wxALL, 0);
	splitterLeftPanel->SetSizer(boxSizer);		

	wxPanel* splitterRightPanel = new wxPanel(splitter);	

	wxBoxSizer* rightPanelSizer = new wxBoxSizer(wxVERTICAL);
	splitterRightPanel->SetSizer(rightPanelSizer);

	_tabsPanel = new TabsPanel(splitterRightPanel, _context);

	rightPanelSizer->Add(_tabsPanel, 1, wxEXPAND | wxALL, 0);
	rightPanelSizer->Layout();

	splitter->SplitVertically(splitterLeftPanel, splitterRightPanel, 1);

	_statusbar = new Statusbar(this, wxID_ANY, wxDefaultPosition, wxSize(-1, 20));	
	mainSizer->Add(_statusbar, 0, wxEXPAND, 0);

	this->SetSizer(mainSizer);
	this->Layout();

	this->Centre(wxBOTH);
	
	_mainMenu->SetTransactions(_context.GetTransactionsService().GetRecents(10));
	_addTransactionButton->SetTransactions(_context.GetTransactionsService().GetRecents(10));

	_treeMenu->SetReceipts(_context.GetAccountsService().GetByType(AccountType::Receipt));
	_treeMenu->SetDeposits(_context.GetAccountsService().GetByType(AccountType::Deposit));
	_treeMenu->SetExpenses(_context.GetAccountsService().GetByType(AccountType::Expens));
	_treeMenu->SetDebts(_context.GetAccountsService().GetByType(AccountType::Debt));
	_treeMenu->SetVirtuals(_context.GetAccountsService().GetByType(AccountType::Virtual));
	_treeMenu->SetArchive(_context.GetAccountsService().GetArchive());
	_treeMenu->SetReports(_context.GetReportsService().GetAll());

	bool isEmpty = _context.GetTransactionsService().GetDeleted().size() == 0;

	_treeMenu->SetIsTrashEmpty(isEmpty);
	_treeMenu->RestoreState();

	_tabsPanel->RestoreTabs();

	UpdateStatus();

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

MainFrame::~MainFrame() 
{
	delete _treeMenu;
	delete _tabsPanel;
	delete _commandsInvoker;
	delete _commandsReceiver;
	delete _dialogsController;

	Settings::GetInstance().SetWindowWidth(this->GetSize().GetWidth());
	Settings::GetInstance().SetWindowHeight(this->GetSize().GetHeight());

	Settings::GetInstance().Save();
}

void MainFrame::UpdateStatus() {
	wxDateTime fromDate = wxDateTime::Now();
	wxDateTime toDate = wxDateTime::Now();

	fromDate.SetDay(1);
	toDate.SetToLastMonthDay();

	int baseCurrencyId = Settings::GetInstance().GetBaseCurrencyId();
	
	wxString rates("");

	auto settingsRates = Settings::GetInstance().GetSelectedExchangeRates();

	for (int id : settingsRates) {
		float rate = _context.GetExchangeRatesRepository().GetExchangeRate(id, baseCurrencyId);
		auto currency = _context.GetCurrenciesRepository().GetById(id);

		rates = rates + wxNumberFormatter::ToString(rate, 2) + wxT(" ") + *currency->shortName + wxT("  ");
	}

	_statusbar->SetPeriod(wxDateTime::Now());
	_statusbar->SetReceipts(_context.GetAccountingService().GetReceipts(fromDate, toDate));
	_statusbar->SetExpenses(_context.GetAccountingService().GetExpenses(fromDate, toDate));
	_statusbar->SetBalance(_context.GetAccountingService().GetBalance());
	_statusbar->SetExchangeRates(rates);

	_statusbar->Layout();
}

std::vector<std::shared_ptr<TransactionViewModel>> MainFrame::OnTreeMenuContextMenu(const AccountViewModel& account) {
	return _context.GetTransactionsService().GetRecents(account, 10);
}