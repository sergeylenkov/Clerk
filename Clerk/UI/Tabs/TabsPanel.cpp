#include "TabsPanel.h"

TabsPanel::TabsPanel(wxWindow *parent, DataContext& context, CommandsInvoker& commandsInvoker, Icons& icons) : wxPanel(parent), _context(context), _commandsInvoker(commandsInvoker), _icons(icons) {
	wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

	_notebook = new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNB_TOP);
	_notebook->SetImageList(_icons.GetImageList());

	mainSizer->Add(_notebook, 1, wxEXPAND | wxALL, 0);	

	this->SetSizer(mainSizer);
	this->Layout();

	_notebook->Bind(wxEVT_NOTEBOOK_PAGE_CHANGED, &TabsPanel::OnTabChanged, this);
	_notebook->Bind(wxEVT_RIGHT_DOWN, &TabsPanel::OnTabClick, this);
}

TabsPanel::~TabsPanel()
{
	Settings::GetInstance().ClearTabs();

	for (auto& panel : _tabsPanels)
	{
		Settings::GetInstance().AddTab(static_cast<int>(panel->type), panel->id);
	}

	Settings::GetInstance().SetSelectedTab(_notebook->GetSelection());

	delete _notebook;
}

void TabsPanel::AddPanel(DataPanel* dataPanel, wxString title, int iconIndex) {
	wxPanel* tabPanel = new wxPanel(_notebook);
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	tabPanel->SetSizer(sizer);
	dataPanel->Reparent(tabPanel);
	sizer->Add(dataPanel, 1, wxEXPAND | wxALL, 0);

	_notebook->AddPage(tabPanel, title, true, iconIndex);
	
	sizer->Layout();

	_tabsPanels.push_back(dataPanel);
}

void TabsPanel::SelectLastTab() {
	_notebook->SetSelection(_notebook->GetPageCount() - 1);
}

void TabsPanel::RemoveTab(int index) {
	DataPanel* currentPanel = _tabsPanels[index];

	_notebook->RemovePage(index);

	_tabsPanels.erase(_tabsPanels.begin() + index);

	delete currentPanel;
}

void TabsPanel::OnTabChanged(wxBookCtrlEvent &event) {
	//
}

void TabsPanel::OnTabClick(wxMouseEvent &event) {
	unsigned int tabId = _notebook->HitTest(event.GetPosition());
	wxPoint point = event.GetPosition();

	bool isCloseEnable = _notebook->GetPageCount() > 1;
	bool isLeftEnable = isCloseEnable && tabId > 0;
	bool isRightEnable = isCloseEnable && tabId < _notebook->GetPageCount() - 1;

	TabsContextMenu* menu = new TabsContextMenu(tabId, isLeftEnable, isRightEnable, isCloseEnable);

	menu->OnClose = [&](int tabId) {
		RemoveTab(tabId);
	};

	_notebook->PopupMenu(menu, point);

	delete menu;

	event.Skip();
}
