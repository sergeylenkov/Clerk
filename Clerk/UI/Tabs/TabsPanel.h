#pragma once

#include <wx/wx.h>
#include <wx/notebook.h>
#include <memory>
#include "../../Defines.h"
#include "../../Data/Settings.h"
#include "../../Data/DataContext.h"
#include "../TreeMenu/Enums.h"
#include "../../Commands/CommandsInvoker.h"
#include "./TabsContextMenu.h"
#include "../../Utils/Icons.h"
#include "../TagsPanel.h"

using namespace Clerk::Data;
using namespace Clerk::Commands;

class TabsPanel : public wxPanel
{
public:
	TabsPanel(wxWindow *parent, DataContext& context, CommandsInvoker& commandsInvoker, Icons& icons);
	~TabsPanel();
		
	void AddPanel(DataPanel* panel, wxString title, int iconIndex);
	void SelectTab(int index);
	void SelectLastTab();

private:
	DataContext& _context;
	CommandsInvoker& _commandsInvoker;
	Icons& _icons;
	wxNotebook *_notebook;	
	std::vector<DataPanel *> _tabsPanels;

	void OnTabChanged(wxBookCtrlEvent &event);	
	void OnTabClick(wxMouseEvent &event);
	void RemoveTab(int index);
};

