#include "Clerk.h"
#include "Data/DataHelper.h"
#include "Data/Model.h"

IMPLEMENT_APP(ClerkApp)

bool ClerkApp::OnInit()
{
	wxLocale locale(wxLANGUAGE_DEFAULT);

	wxFileName path(wxStandardPaths::Get().GetUserDataDir(), "Database.sqlite");

	DataHelper::GetInstance().Open(path.GetFullPath().char_str());
	DataHelper::GetInstance().Init();

	Model::SetConnection(DataHelper::GetInstance().Connection());

	Settings::GetInstance().Open("Config.json");

	wxInitAllImageHandlers();

	MainFrame *frame = new MainFrame("Clerk", wxPoint(0, 0), wxSize(1000, 800));
	frame->Show(TRUE);
	frame->Center();

	SetTopWindow(frame);

	return TRUE;
}