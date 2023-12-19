#include "BudgetContextMenu.h"

BudgetContextMenu::BudgetContextMenu(CommandsInvoker& commandsInvoker, Icons& icons, std::shared_ptr<BudgetPresentationModel> budget):
	_commandsInvoker(commandsInvoker),
	_icons(icons),
	_budget(budget)
{
	if (budget) {
		wxMenuItem* item = Append(static_cast<int>(BudgetContextMenuTypes::Add), _("Add..."));
		item->SetBitmap(_icons.GetIconByType(IconType::Plus));

		item = Append(static_cast<int>(BudgetContextMenuTypes::Edit), _("Edit..."));
		item->SetBitmap(_icons.GetIconByType(IconType::Pencil));

		AppendSeparator();

		item = Append(static_cast<int>(BudgetContextMenuTypes::Delete), _("Delete"));
		item->SetBitmap(_icons.GetIconByType(IconType::Delete));
	}
	else {
		wxMenuItem* item = Append(static_cast<int>(BudgetContextMenuTypes::Add), _("Add..."));
		item->SetBitmap(_icons.GetIconByType(IconType::Plus));
	}

	Bind(wxEVT_COMMAND_MENU_SELECTED, &BudgetContextMenu::OnMenuSelect, this);
}

void BudgetContextMenu::OnMenuSelect(wxCommandEvent& event) {
	BudgetContextMenuTypes type = static_cast<BudgetContextMenuTypes>(event.GetId());
	int id = -1;

	if (_budget) {
		id = _budget->id;
	}

	switch (type)
	{
	case BudgetContextMenuTypes::Add:
		_commandsInvoker.NewBudget();
		break;
	case BudgetContextMenuTypes::Edit:
		_commandsInvoker.EditBudget(id);
		break;
	case BudgetContextMenuTypes::Delete:
		_commandsInvoker.DeleteBudget(id);
		break;
	default:
		break;
	}
}