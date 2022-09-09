#pragma once

#include <wx/wx.h>
#include "../UI/Enums.h"
#include "../Data/DataContext.h"
#include "../Commands/CommandsInvoker.h"

using namespace Clerk::Data;
using namespace Clerk::UI;
using namespace Clerk::Commands;

class DataPanel : public wxPanel
{
public:
	DataPanel(wxWindow* parent, DataContext& context, CommandsInvoker& commandsInvoker);
	virtual void Update();

	TabType type;
	int id;

protected:
	DataContext& _context;
	CommandsInvoker& _commandsInvoker;
};