#include "DialogsController.h"

using namespace Clerk::UI;

DialogsController::DialogsController(wxFrame* frame) {
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

void DialogsController::ShowTransactionDialog() {
	TransactionDialog* transactionDialog = new TransactionDialog(_parent, wxT("Transaction"), 0, 0, 450, 350);

	//transactionDialog->OnClose = std::bind(&MainFrame::OnTransactionDialogClose, this);

	transactionDialog->Show(true);
	transactionDialog->CenterOnParent();
}