#include "TabsContextMenu.h"

using namespace Clerk::UI;

TabsContextMenu::TabsContextMenu(Icons& icons, int tabId, bool isLeftEnabled, bool isRightEnabled, bool isCloseEnabled):
	_icons(icons),
	_tabId(tabId),
	_isLeftEnabled(isLeftEnabled),
	_isRightEnabled(isRightEnabled),
	_isCloseEnabled(isCloseEnabled)
{
	wxMenuItem* item = Append(static_cast<int>(TabsContextMenuType::Left), _("Move to Left"));
	item->SetBitmap(_icons.GetIconByType(IconType::ArrowLeft));

	if (!_isLeftEnabled) {
		item->Enable(false);
		item->SetTextColour(*wxLIGHT_GREY);
	}

	item = Append(static_cast<int>(TabsContextMenuType::Right), _("Move to Right"));	
	item->SetBitmap(_icons.GetIconByType(IconType::ArrowRight));

	if (!_isRightEnabled) {
		item->Enable(false);
		item->SetTextColour(*wxLIGHT_GREY);
	}

	AppendSeparator();

	item = Append(static_cast<int>(TabsContextMenuType::Close), _("Close"));
	item->SetBitmap(_icons.GetIconByType(IconType::TabRemove));

	if (!_isCloseEnabled) {
		item->Enable(false);
		item->SetTextColour(*wxLIGHT_GREY);
	}

	Bind(wxEVT_COMMAND_MENU_SELECTED, &TabsContextMenu::OnMenuSelect, this);
}

void TabsContextMenu::OnMenuSelect(wxCommandEvent& event) {
	TabsContextMenuType type = static_cast<TabsContextMenuType>(event.GetId());

	if (type == TabsContextMenuType::Left) {
		if (OnMoveLeft) {
			OnMoveLeft(_tabId);
		}
	}
	else if (type == TabsContextMenuType::Right) {
		if (OnMoveRight) {
			OnMoveRight(_tabId);
		}
	}
	else {
		if (OnClose) {
			OnClose(_tabId);
		}
	}
}