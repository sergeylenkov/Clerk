#pragma once

#include <wx/wx.h>
#include "../Tabs/Enums.h"
#include "../../Data/DataContext.h"
#include "../../Utils/Icons.h"

using namespace Clerk::Data;
using namespace Clerk::UI;
using namespace Clerk::Utils;

class DataPanel : public wxPanel
{
public:
	DataPanel(wxWindow* parent, DataContext& context, Icons& icons);
	virtual void Update();

	TabType type;
	int id;

protected:
	DataContext& _context;
	Icons& _icons;
};