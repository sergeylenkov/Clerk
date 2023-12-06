#include "TabsContextMenu.h"

using namespace Clerk::UI;

TabsContextMenu::TabsContextMenu(int tabId, bool isLeftEnabled, bool isRightEnabled, bool isCloseEnabled):
	_tabId(tabId), _isLeftEnabled(isLeftEnabled), _isRightEnabled(isRightEnabled), _isCloseEnabled(isCloseEnabled) {
	wxMenuItem* item = this->Append(static_cast<int>(TabsContextMenuType::Left), _("Move to Left"));
	item->SetBitmap(wxBitmap("ICON_ARROW_LEFT", wxBITMAP_TYPE_PNG_RESOURCE));

	if (!_isLeftEnabled) {
		item->Enable(false);
		item->SetTextColour(*wxLIGHT_GREY);
	}

	item = this->Append(static_cast<int>(TabsContextMenuType::Right), _("Move to Right"));	
	item->SetBitmap(wxBitmap("ICON_ARROW_RIGHT", wxBITMAP_TYPE_PNG_RESOURCE));

	if (!_isRightEnabled) {
		item->Enable(false);
		item->SetTextColour(*wxLIGHT_GREY);
	}

	this->AppendSeparator();

	item = this->Append(static_cast<int>(TabsContextMenuType::Close), _("Close"));
	item->SetBitmap(wxBitmap("ICON_CLOSE_TAB", wxBITMAP_TYPE_PNG_RESOURCE));

	if (!_isCloseEnabled) {
		item->Enable(false);
		item->SetTextColour(*wxLIGHT_GREY);
	}

	this->Bind(wxEVT_COMMAND_MENU_SELECTED, &TabsContextMenu::OnMenuSelect, this);
}

void TabsContextMenu::OnMenuSelect(wxCommandEvent& event) {
	TabsContextMenuType type = static_cast<TabsContextMenuType>(event.GetId());

	if (type == TabsContextMenuType::Left) {
		if (OnLeft) {
			OnLeft(_tabId);
		}
	}
	else if (type == TabsContextMenuType::Right) {
		if (OnRight) {
			OnRight(_tabId);
		}
	}
	else {
		if (OnClose) {
			OnClose(_tabId);
		}
	}
}