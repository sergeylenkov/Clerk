#include "TabsController.h"

using namespace Clerk::UI;

TabsController::TabsController(DataContext& context, Icons& icons) : _context(context), _icons(icons) {
	_panel = nullptr;
}

void TabsController::SetTabsPanel(TabsPanel* panel) {
	_panel = panel;
}

void TabsController::OpenNewTab(TabType type) {
	if (_panel) {
		//TODO
	}
}
