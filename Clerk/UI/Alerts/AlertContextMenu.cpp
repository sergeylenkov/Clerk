#include "AlertContextMenu.h"

AlertContextMenu::AlertContextMenu(CommandsInvoker& commandsInvoker, Icons& icons, std::shared_ptr<AlertPresentationModel> alert):
	_commandsInvoker(commandsInvoker),
	_icons(icons),
	_alert(alert)
{
	if (alert) {
		wxMenuItem* item = Append(static_cast<int>(AlertContextMenuTypes::Add), _("Add..."));
		item->SetBitmap(_icons.GetIconByType(IconType::Plus));

		item = Append(static_cast<int>(AlertContextMenuTypes::Edit), _("Edit..."));
		item->SetBitmap(_icons.GetIconByType(IconType::Pencil));

		AppendSeparator();

		item = Append(static_cast<int>(AlertContextMenuTypes::Delete), _("Delete"));
		item->SetBitmap(_icons.GetIconByType(IconType::Delete));
	}
	else {
		wxMenuItem* item = Append(static_cast<int>(AlertContextMenuTypes::Add), _("Add..."));
		item->SetBitmap(_icons.GetIconByType(IconType::Plus));
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