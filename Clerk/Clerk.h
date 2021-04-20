#include "wx/wx.h" 
#include "UI/MainFrame.h"
#include "Data/DataConnection.h"
#include "Data/DataContext.h"
#include "Utils/Icons.h"

using namespace Clerk::Data;
using namespace Clerk::Utils;

class ClerkApp : public wxApp {
	virtual bool OnInit();
	virtual int OnExit();

private:
	DataConnection* _connection{};
	DataContext* _context{};
	Icons* _icons{};
};