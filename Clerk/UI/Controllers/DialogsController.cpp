#include "DialogsController.h"

using namespace Clerk::UI;

DialogsController::DialogsController(wxFrame* frame, DataContext& context, Icons& icons): _context(context), _icons(icons) {
	_parent = frame;
}

void DialogsController::ShowAboutDialog()
{
	AboutDialog* aboutDialog = new AboutDialog(_parent, wxT("About"), 0, 0, 250, 340);

	aboutDialog->Show(true);
	aboutDialog->CenterOnParent();
}

void DialogsController::ShowPreferencesDialog()
{
	PreferencesDialog* preferencesDialog = new PreferencesDialog(_parent, wxT("Preferences"), 0, 0, 400, 300);

	preferencesDialog->Show(true);
	preferencesDialog->CenterOnParent();
}

void DialogsController::ShowNewTransactionDialog(int id) {
	TransactionDialog* transactionDialog = new TransactionDialog(_parent, wxT("Transaction"), 0, 0, 450, 350, _icons);

	TransactionEditViewModel* viewModel = new TransactionEditViewModel(_context.GetAccountsService(), _context.GetTransactionsService(),  _context.GetExchangeRatesRepository(), _context.GetTagsService());
	viewModel->SetAccountId(id);

	transactionDialog->SetViewModel(viewModel);

	transactionDialog->Show(true);
	transactionDialog->CenterOnParent();
}

void DialogsController::ShowCopyTransactionDialog(int id) {
	TransactionDialog* transactionDialog = new TransactionDialog(_parent, wxT("Transaction"), 0, 0, 450, 350, _icons);

	TransactionEditViewModel* viewModel = new TransactionEditViewModel(_context.GetAccountsService(), _context.GetTransactionsService(), _context.GetExchangeRatesRepository(), _context.GetTagsService());
	viewModel->SetCopyTransactionId(id);

	transactionDialog->SetViewModel(viewModel);

	transactionDialog->Show(true);
	transactionDialog->CenterOnParent();
}

void DialogsController::ShowEditAccountDialog(int id) {
	AccountDialog* accountDialog = new AccountDialog(_parent, wxT("Account"), 0, 0, 340, 400, _icons);

	AccountEditViewModel* viewModel = new AccountEditViewModel(_context.GetAccountsService(), _context.GetCurrenciesRepository());
	viewModel->SetAccountId(id);

	accountDialog->SetViewModel(viewModel);

	accountDialog->Show(true);
	accountDialog->CenterOnParent();
}