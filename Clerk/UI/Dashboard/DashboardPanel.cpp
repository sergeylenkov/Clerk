#include "DashboardPanel.h"

DashboardPanel::DashboardPanel(wxWindow *parent, DataContext& context) : DataPanel(parent, context) {
	wxBoxSizer *mainSizer= new wxBoxSizer(wxVERTICAL);

	this->SetBackgroundColour(*wxWHITE);

	_scrolledWindow = new wxScrolledWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxVSCROLL);
	_scrolledWindow->SetScrollRate(5, 5);

	wxBoxSizer *horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	_leftPanel = new wxPanel(_scrolledWindow, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);

	wxBoxSizer *leftSizer = new wxBoxSizer(wxVERTICAL);

	_viewModel = new DashboardViewModel(_context.GetAccountingService(), _context.GetTransactionsService(), _context.GetAccountsService(),
		_context.GetBudgetsService(), _context.GetSchedulersService(), _context.GetGoalsService(), _context.GetCurrenciesService());

	_viewModel->OnUpdate([=]() {
		Update();
	});

	_balancePanel = new DashboardBalancePanel(_leftPanel);
	_balancePanel->SetViewModel(_viewModel);

	leftSizer->Add(_balancePanel, 0, wxEXPAND | wxALL, 5);

	_accountsPanel = new DashboardAccountsPanel(_leftPanel);
	_accountsPanel->SetViewModel(_viewModel);

	leftSizer->Add(_accountsPanel, 0, wxEXPAND | wxALL, 5);

	_receiptsPanel = new DashboardReceiptsPanel(_leftPanel);
	_receiptsPanel->SetViewModel(_viewModel);

	leftSizer->Add(_receiptsPanel, 0, wxEXPAND | wxALL, 5);

	_expensesPanel = new DashboardExpensesPanel(_leftPanel);
	_expensesPanel->SetViewModel(_viewModel);

	leftSizer->Add(_expensesPanel, 0, wxEXPAND | wxALL, 5);

	_leftPanel->SetSizer(leftSizer);
	_leftPanel->Layout();

	leftSizer->Fit(_leftPanel);
	horizontalSizer->Add(_leftPanel, 4, wxEXPAND | wxALL, 10);

	horizontalSizer->Add(0, 0, 1, wxEXPAND, 0);

	_rightPanel = new wxPanel(_scrolledWindow, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
	wxBoxSizer *rightSizer = new wxBoxSizer(wxVERTICAL);

	_schedulersPanel = new DashboardSchedulersPanel(_rightPanel);
	_schedulersPanel->SetViewModel(_viewModel);

	rightSizer->Add(_schedulersPanel, 0, wxEXPAND | wxALL, 5);

	_budgetsPanel = new DashboardBudgetsPanel(_rightPanel);
	_budgetsPanel->SetViewModel(_viewModel);

	rightSizer->Add(_budgetsPanel, 0, wxEXPAND | wxALL, 5);

	_goalsPanel = new DashboardGoalsPanel(_rightPanel);
	_goalsPanel->SetViewModel(_viewModel);

	rightSizer->Add(_goalsPanel, 0, wxEXPAND | wxALL, 5);

	_debtsPanel = new DashboardDebtsPanel(_rightPanel);
	_debtsPanel->SetViewModel(_viewModel);

	rightSizer->Add(_debtsPanel, 0, wxEXPAND | wxALL, 5);

	_rightPanel->SetSizer(rightSizer);
	_rightPanel->Layout();
	rightSizer->Fit(_rightPanel);

	horizontalSizer->Add(_rightPanel, 6, wxEXPAND | wxALL, 10);

	_scrolledWindow->SetSizer(horizontalSizer);
	_scrolledWindow->Layout();

	horizontalSizer->Fit(_scrolledWindow);
	mainSizer->Add(_scrolledWindow, 1, wxEXPAND | wxALL, 0);

	this->SetSizer(mainSizer);
	this->Layout();

	this->SetDoubleBuffered(true);
}

DashboardPanel::~DashboardPanel() {
	delete _viewModel;
}

void DashboardPanel::Update() {	
	this->Layout();
}