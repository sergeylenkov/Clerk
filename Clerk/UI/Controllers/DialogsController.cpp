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