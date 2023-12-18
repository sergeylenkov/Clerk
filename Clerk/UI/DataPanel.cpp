#include "DataPanel.h"

DataPanel::DataPanel(wxWindow *parent, DataContext& context, Icons& icons):
	wxPanel(parent),
	_context(context),
	_icons(icons)
{
	id = 0;
	type = TabType::Dashboard;
}

void DataPanel::Update() {
}
