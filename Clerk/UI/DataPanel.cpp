#include "DataPanel.h"

DataPanel::DataPanel(wxWindow *parent, DataContext& context) : wxPanel(parent), _context(context) {
	id = 0;
	type = TabType::Dashboard;
}

void DataPanel::Update() {
}
