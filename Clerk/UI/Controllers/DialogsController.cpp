#include "DialogsController.h"

using namespace Clerk::UI;

DialogsController::DialogsController(DataContext& context, Icons& icons): _context(context), _icons(icons) {
	
}

void DialogsController::SetMainWindow(wxFrame* frame) {
	_parent = frame;
}

void DialogsController::ShowAboutDialog() {
	wxSize size = GetAboutDialogSize();
	AboutDialog* aboutDialog = new AboutDialog(_parent, _("About"), 0, 0, size.GetWidth(), size.GetHeight());

	aboutDialog->Show(true);
	aboutDialog->CenterOnParent();
}

void DialogsController::ShowPreferencesDialog() {
	wxSize size = GetPreferencesDialogSize();

	PreferencesDialog* preferencesDialog = new PreferencesDialog(_parent, _("Preferences"), 0, 0, size.GetWidth(), size.GetHeight(), _context);

	preferencesDialog->Show(true);
	preferencesDialog->CenterOnParent();
}

void DialogsController::ShowNewTransactionDialog(int id) {
	wxSize size = GetTransactionDialogSize();

	TransactionDialog* transactionDialog = new TransactionDialog(_parent, _("Transaction"), 0, 0, size.GetWidth(), size.GetHeight(), _icons, _context);
	
	TransactionViewModel* viewModel = new TransactionViewModel(_context.GetTransactionsService(), _context.GetAccountsService(), _context.GetCurrenciesService(), _context.GetTagsService());
	viewModel->SetAccountId(id);
	
	transactionDialog->SetViewModel(viewModel);

	transactionDialog->Show(true);
	transactionDialog->CenterOnParent();
}

void DialogsController::ShowCopyTransactionDialog(int id) {
	wxSize size = GetTransactionDialogSize();

	TransactionDialog* transactionDialog = new TransactionDialog(_parent, _("Transaction"), 0, 0, size.GetWidth(), size.GetHeight(), _icons, _context);

	TransactionViewModel* viewModel = new TransactionViewModel(_context.GetTransactionsService(), _context.GetAccountsService(), _context.GetCurrenciesService(), _context.GetTagsService());
	viewModel->SetCopyTransactionId(id);

	transactionDialog->SetViewModel(viewModel);

	transactionDialog->Show(true);
	transactionDialog->CenterOnParent();
}

void DialogsController::ShowSplitTransactionDialog(int id) {
	wxSize size = GetTransactionDialogSize();

	TransactionDialog* transactionDialog = new TransactionDialog(_parent, _("Transaction"), 0, 0, size.GetWidth(), size.GetHeight(), _icons, _context);

	TransactionViewModel* viewModel = new TransactionViewModel(_context.GetTransactionsService(), _context.GetAccountsService(), _context.GetCurrenciesService(), _context.GetTagsService());
	viewModel->SetSplitTransactionId(id);

	transactionDialog->SetViewModel(viewModel);

	transactionDialog->Show(true);
	transactionDialog->CenterOnParent();
}

void DialogsController::ShowEditTransactionDialog(int id) {
	wxSize size = GetTransactionDialogSize();

	TransactionDialog* transactionDialog = new TransactionDialog(_parent, _("Transaction"), 0, 0, size.GetWidth(), size.GetHeight(), _icons, _context);

	TransactionViewModel* viewModel = new TransactionViewModel(_context.GetTransactionsService(), _context.GetAccountsService(), _context.GetCurrenciesService(), _context.GetTagsService());
	viewModel->SetTransactionId(id);

	transactionDialog->SetViewModel(viewModel);

	transactionDialog->Show(true);
	transactionDialog->CenterOnParent();
}

void DialogsController::ShowNewAccountDialog(AccountType type) {
	wxSize size = GetAccountDialogSize();

	AccountDialog* accountDialog = new AccountDialog(_parent, _("Account"), 0, 0, size.GetWidth(), size.GetHeight(), _icons);

	AccountViewModel* viewModel = new AccountViewModel(_context.GetAccountsService(), _context.GetCurrenciesService());
	viewModel->SetAccountType(type);

	accountDialog->SetViewModel(viewModel);

	accountDialog->Show(true);
	accountDialog->CenterOnParent();

}
void DialogsController::ShowEditAccountDialog(int id) {
	wxSize size = GetAccountDialogSize();

	AccountDialog* accountDialog = new AccountDialog(_parent, _("Account"), 0, 0, size.GetWidth(), size.GetHeight(), _icons);

	AccountViewModel* viewModel = new AccountViewModel(_context.GetAccountsService(), _context.GetCurrenciesService());
	viewModel->SetAccountId(id);

	accountDialog->SetViewModel(viewModel);

	accountDialog->Show(true);
	accountDialog->CenterOnParent();
}

void DialogsController::ShowNewAlertDialog() {
	wxSize size = GetAlertDialogSize();

	AlertDialog* alertDialog = new AlertDialog(_parent, _("Alert"), 0, 0, size.GetWidth(), size.GetHeight(), _icons);

	AlertViewModel* viewModel = new AlertViewModel(_context.GetAlertsService(), _context.GetAccountsService());

	alertDialog->SetViewModel(viewModel);

	alertDialog->Show(true);
	alertDialog->CenterOnParent();
}

void DialogsController::ShowEditAlertDialog(int id) {
	wxSize size = GetAlertDialogSize();

	AlertDialog* alertDialog = new AlertDialog(_parent, _("Alert"), 0, 0, size.GetWidth(), size.GetHeight(), _icons);
	
	AlertViewModel* viewModel = new AlertViewModel(_context.GetAlertsService(), _context.GetAccountsService());
	viewModel->SetAlertId(id);

	alertDialog->SetViewModel(viewModel);

	alertDialog->Show(true);
	alertDialog->CenterOnParent();
}

void DialogsController::ShowNewBudgetDialog() {
	wxSize size = GetBudgetDialogSize();

	BudgetDialog* budgetDialog = new BudgetDialog(_parent, _("Budget"), 0, 0, size.GetWidth(), size.GetHeight(), _icons);

	BudgetViewModel* viewModel = new BudgetViewModel(_context.GetBudgetsService(), _context.GetAccountsService());	

	budgetDialog->SetViewModel(viewModel);

	budgetDialog->Show(true);
	budgetDialog->CenterOnParent();
}

void DialogsController::ShowEditBudgetDialog(int id) {
	wxSize size = GetBudgetDialogSize();

	BudgetDialog* budgetDialog = new BudgetDialog(_parent, _("Budget"), 0, 0, size.GetWidth(), size.GetHeight(), _icons);

	BudgetViewModel* viewModel = new BudgetViewModel(_context.GetBudgetsService(), _context.GetAccountsService());
	viewModel->SetBudgetId(id);

	budgetDialog->SetViewModel(viewModel);

	budgetDialog->Show(true);
	budgetDialog->CenterOnParent();
}

void DialogsController::ShowNewGoalDialog() {
	wxSize size = GetGoalDialogSize();

	GoalDialog* goalDialog = new GoalDialog(_parent, _("Goal"), 0, 0, size.GetWidth(), size.GetHeight(), _icons);

	GoalViewModel* viewModel = new GoalViewModel(_context.GetGoalsService(), _context.GetAccountsService());

	goalDialog->SetViewModel(viewModel);

	goalDialog->Show(true);
	goalDialog->CenterOnParent();
}

void DialogsController::ShowEditGoalDialog(int id) {
	wxSize size = GetGoalDialogSize();

	GoalDialog* goalDialog = new GoalDialog(_parent, _("Goal"), 0, 0, size.GetWidth(), size.GetHeight(), _icons);

	GoalViewModel* viewModel = new GoalViewModel(_context.GetGoalsService(), _context.GetAccountsService());
	viewModel->SetGoalId(id);

	goalDialog->SetViewModel(viewModel);

	goalDialog->Show(true);
	goalDialog->CenterOnParent();
}

void DialogsController::ShowNewSchedulerDialog() {
	wxSize size = GetSchedulerDialogSize();

	SchedulerDialog* schedulerDialog = new SchedulerDialog(_parent, _("Scheduler"), 0, 0, size.GetWidth(), size.GetHeight(), _icons, _context);

	SchedulerViewModel* viewModel = new SchedulerViewModel(_context.GetSchedulersService(), _context.GetAccountsService(), _context.GetCurrenciesService(), _context.GetTagsService());

	schedulerDialog->SetViewModel(viewModel);

	schedulerDialog->Show(true);
	schedulerDialog->CenterOnParent();
}

void DialogsController::ShowEditSchedulerDialog(int id) {
	wxSize size = GetSchedulerDialogSize();

	SchedulerDialog* schedulerDialog = new SchedulerDialog(_parent, _("Scheduler"), 0, 0, size.GetWidth(), size.GetHeight(), _icons, _context);

	SchedulerViewModel* viewModel = new SchedulerViewModel(_context.GetSchedulersService(), _context.GetAccountsService(), _context.GetCurrenciesService(), _context.GetTagsService());
	viewModel->SetSchedulerId(id);

	schedulerDialog->SetViewModel(viewModel);

	schedulerDialog->Show(true);
	schedulerDialog->CenterOnParent();
}

wxSize DialogsController::GetAboutDialogSize() {
	wxSize size = wxSize();

	size.SetWidth(_parent->FromDIP(250));
	size.SetHeight(_parent->FromDIP(260));

	return size;
}

wxSize DialogsController::GetTransactionDialogSize() {
	wxSize size = wxSize();

	size.SetWidth(_parent->FromDIP(460));
	size.SetHeight(_parent->FromDIP(350));

	return size;
}

wxSize DialogsController::GetPreferencesDialogSize() {
	wxSize size = wxSize();

	size.SetWidth(_parent->FromDIP(400));
	size.SetHeight(_parent->FromDIP(380));

	return size;
}

wxSize DialogsController::GetAccountDialogSize() {
	wxSize size = wxSize();

	size.SetWidth(_parent->FromDIP(380));
	size.SetHeight(_parent->FromDIP(400));

	return size;
}

wxSize DialogsController::GetAlertDialogSize() {
	wxSize size = wxSize();

	size.SetWidth(_parent->FromDIP(400));
	size.SetHeight(_parent->FromDIP(500));

	return size;
}

wxSize DialogsController::GetBudgetDialogSize() {
	wxSize size = wxSize();

	size.SetWidth(_parent->FromDIP(400));
	size.SetHeight(_parent->FromDIP(400));

	return size;
}

wxSize DialogsController::GetGoalDialogSize() {
	wxSize size = wxSize();

	size.SetWidth(_parent->FromDIP(400));
	size.SetHeight(_parent->FromDIP(350));

	return size;
}

wxSize DialogsController::GetSchedulerDialogSize() {
	wxSize size = wxSize();

	size.SetWidth(_parent->FromDIP(450));
	size.SetHeight(_parent->FromDIP(420));

	return size;
}