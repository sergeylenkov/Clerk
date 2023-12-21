#include "GoalContextMenu.h"

GoalContextMenu::GoalContextMenu(CommandsInvoker& commandsInvoker, Icons& icons, std::shared_ptr<GoalPresentationModel> goal):
	_commandsInvoker(commandsInvoker),
	_icons(icons),
	_goal(goal)
{
	if (goal) {
		wxMenuItem* item = Append(static_cast<int>(GoalContextMenuTypes::Add), _("Add..."));
		item->SetBitmap(_icons.GetIconByType(IconType::Plus));

		item = Append(static_cast<int>(GoalContextMenuTypes::Edit), _("Edit..."));
		item->SetBitmap(_icons.GetIconByType(IconType::Pencil));

		AppendSeparator();

		item = Append(static_cast<int>(GoalContextMenuTypes::Delete), _("Delete"));
		item->SetBitmap(_icons.GetIconByType(IconType::Delete));
	}
	else {
		wxMenuItem* item = Append(static_cast<int>(GoalContextMenuTypes::Add), _("Add..."));
		item->SetBitmap(_icons.GetIconByType(IconType::Plus));
	}

	Bind(wxEVT_COMMAND_MENU_SELECTED, &GoalContextMenu::OnMenuSelect, this);
}

void GoalContextMenu::OnMenuSelect(wxCommandEvent& event) {
	GoalContextMenuTypes type = static_cast<GoalContextMenuTypes>(event.GetId());
	int id = -1;

	if (_goal) {
		id = _goal->id;
	}

	switch (type)
	{
	case GoalContextMenuTypes::Add:
		_commandsInvoker.NewGoal();
		break;
	case GoalContextMenuTypes::Edit:
		_commandsInvoker.EditGoal(id);
		break;
	case GoalContextMenuTypes::Delete:
		_commandsInvoker.DeleteGoal(id);
		break;
	default:
		break;
	}
}