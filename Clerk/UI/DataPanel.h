#pragma once

#include <wx/wx.h>
#include "TreeMenu/Enums.h"
#include "../Data/DataContext.h"

using namespace Clerk::Data;
using namespace Clerk::UI;

class DataPanel : public wxPanel
{
public:
	DataPanel(wxWindow* parent, DataContext& context);
	virtual void Update();

	TreeMenuItemType type;
	int id;

protected:
	DataContext& _context;
};