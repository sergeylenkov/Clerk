#include "AlertContextMenu.h"

AlertContextMenu::AlertContextMenu(CommandsInvoker& commandsInvoker, std::shared_ptr<AlertPresentationModel> alert):
	_commandsInvoker(commandsInvoker),
	_alert(alert)
{
	if (alert) {
		wxMenuItem* item = Append(static_cast<int>(AlertContextMenuTypes::Add), _("Add..."));
		item->SetBitmap(wxBitmap("ICON_ADD", wxBITMAP_TYPE_PNG_RESOURCE));

		item = Append(static_cast<int>(AlertContextMenuTypes::Edit), _("Edit..."));
		item->SetBitmap(wxBitmap("ICON_EDIT", wxBITMAP_TYPE_PNG_RESOURCE));

		AppendSeparator();

		item = Append(static_cast<int>(AlertContextMenuTypes::Delete), _("Delete"));
		item->SetBitmap(wxBitmap("ICON_DELETE", wxBITMAP_TYPE_PNG_RESOURCE));
	}
	else {
		wxMenuItem* item = Append(static_cast<int>(AlertContextMenuTypes::Add), _("Add..."));
		item->SetBitmap(wxBitmap("ICON_ADD", wxBITMAP_TYPE_PNG_RESOURCE));
	}

	Bind(wxEVT_COMMAND_MENU_SELECTED, &AlertContextMenu::OnMenuSelect, this);
}

void AlertContextMenu::OnMenuSelect(wxCommandEvent& event) {
	AlertContextMenuTypes type = static_cast<AlertContextMenuTypes>(event.GetId());
	int id = -1;

	if (_alert) {
		id = _alert->id;
	}

	switch (type)
	{
	case AlertContextMenuTypes::Add:
		_commandsInvoker.NewAlert();
		break;
	case AlertContextMenuTypes::Edit:
		_commandsInvoker.EditAlert(id);
		break;
	case AlertContextMenuTypes::Delete:
		_commandsInvoker.DeleteAlert(id);
		break;
	default:
		break;
	}
}