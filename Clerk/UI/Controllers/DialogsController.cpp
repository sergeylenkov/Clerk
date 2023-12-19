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
	
	TransactionViewModel* viewModel = new TransactionViewModel(_context.GetAccountsService(), _context.GetTransactionsService(),  _context.GetCurrenciesService(), _context.GetTagsService());
	viewModel->SetAccountId(id);
	
	transactionDialog->SetViewModel(viewModel);

	transactionDialog->Show(true);
	transactionDialog->CenterOnParent();
}

void DialogsController::ShowCopyTransactionDialog(int id) {
	wxSize size = GetTransactionDialogSize();

	TransactionDialog* transactionDialog = new TransactionDialog(_parent, _("Transaction"), 0, 0, size.GetWidth(), size.GetHeight(), _icons, _context);

	TransactionViewModel* viewModel = new TransactionViewModel(_context.GetAccountsService(), _context.GetTransactionsService(), _context.GetCurrenciesService(), _context.GetTagsService());
	viewModel->SetCopyTransactionId(id);

	transactionDialog->SetViewModel(viewModel);

	transactionDialog->Show(true);
	transactionDialog->CenterOnParent();
}

void DialogsController::ShowSplitTransactionDialog(int id) {
	wxSize size = GetTransactionDialogSize();

	TransactionDialog* transactionDialog = new TransactionDialog(_parent, _("Transaction"), 0, 0, size.GetWidth(), size.GetHeight(), _icons, _context);

	TransactionViewModel* viewModel = new TransactionViewModel(_context.GetAccountsService(), _context.GetTransactionsService(), _context.GetCurrenciesService(), _context.GetTagsService());
	viewModel->SetSplitTransactionId(id);

	transactionDialog->SetViewModel(viewModel);

	transactionDialog->Show(true);
	transactionDialog->CenterOnParent();
}

void DialogsController::ShowEditTransactionDialog(int id) {
	wxSize size = GetTransactionDialogSize();

	TransactionDialog* transactionDialog = new TransactionDialog(_parent, _("Transaction"), 0, 0, size.GetWidth(), size.GetHeight(), _icons, _context);

	TransactionViewModel* viewModel = new TransactionViewModel(_context.GetAccountsService(), _context.GetTransactionsService(), _context.GetCurrenciesService(), _context.GetTagsService());
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

void DialogsController::ShowEditAlertDialog(int id) {
	wxSize size = GetAlertDialogSize();

	AlertDialog* alertDialog = new AlertDialog(_parent, _("Alert"), 0, 0, size.GetWidth(), size.GetHeight(), _icons);
	
	AlertViewModel* viewModel = new AlertViewModel(_context.GetAlertsService(), _context.GetAccountsService());
	viewModel->SetAlertId(id);

	alertDialog->SetViewModel(viewModel);

	alertDialog->Show(true);
	alertDialog->CenterOnParent();
}

wxSize DialogsController::GetAboutDialogSize() {
	wxSize size = wxSize();

	size.SetWidth(_parent->FromDIP(250));
	size.SetHeight(_parent->FromDIP(260));

	return size;
}

wxSize DialogsController::GetTransactionDialogSize() {
	wxSize size = wxSize();

	size.SetWidth(_parent->FromDIP(450));
	size.SetHeight(_parent->FromDIP(350));

	return size;
}

wxSize DialogsController::GetPreferencesDialogSize() {
	wxSize size = wxSize();

	size.SetWidth(_parent->FromDIP(400));
	size.SetHeight(_parent->FromDIP(300));

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