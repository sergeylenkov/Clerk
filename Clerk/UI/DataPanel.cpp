#include "DataPanel.h"

DataPanel::DataPanel(wxWindow *parent, DataContext& context, CommandsInvoker& commandsInvoker) : wxPanel(parent), _context(context), _commandsInvoker(commandsInvoker) {
	id = 0;
	type = TabType::Dashboard;
}

void DataPanel::Update() {
}
