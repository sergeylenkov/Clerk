#include "Clerk.h"
#include "DataHelper.h"

IMPLEMENT_APP(ClerkApp)

bool ClerkApp::OnInit()
{
	DataHelper::GetInstance().Open("Resources/Database.sqlite");

	wxInitAllImageHandlers();

	MainFrame *frame = new MainFrame("Clerk", wxPoint(0, 0), wxSize(1000, 800));
	frame->Show(TRUE);
	frame->Center();

	SetTopWindow(frame);

	return TRUE;
}