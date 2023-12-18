#include "SchedulerContextMenu.h"

SchedulerContextMenu::SchedulerContextMenu(CommandsInvoker& commandsInvoker, Icons& icons, std::shared_ptr<SchedulerPresentationModel> scheduler):
	_commandsInvoker(commandsInvoker),
	_icons(icons),
	_scheduler(scheduler)
{
	if (scheduler) {
		wxMenuItem* item = Append(static_cast<int>(SchedulerContextMenuTypes::Add), _("Add..."));
		item->SetBitmap(_icons.GetIconByType(IconType::Plus));

		item = Append(static_cast<int>(SchedulerContextMenuTypes::Edit), _("Edit..."));
		item->SetBitmap(_icons.GetIconByType(IconType::Pencil));

		AppendSeparator();

		if (scheduler->isActive) {
			item = Append(static_cast<int>(SchedulerContextMenuTypes::Pause), _("Pause"));
			item->SetBitmap(_icons.GetIconByType(IconType::Pause));
		}
		else {
			item = Append(static_cast<int>(SchedulerContextMenuTypes::Run), _("Run"));
			item->SetBitmap(_icons.GetIconByType(IconType::Play));
		}

		AppendSeparator();

		item = Append(static_cast<int>(SchedulerContextMenuTypes::Delete), _("Delete"));
		item->SetBitmap(_icons.GetIconByType(IconType::Delete));
	}
	else {
		wxMenuItem* item = Append(static_cast<int>(SchedulerContextMenuTypes::Add), _("Add..."));
		item->SetBitmap(_icons.GetIconByType(IconType::Plus));
	}

	Bind(wxEVT_COMMAND_MENU_SELECTED, &SchedulerContextMenu::OnMenuSelect, this);
}

void SchedulerContextMenu::OnMenuSelect(wxCommandEvent& event) {
	SchedulerContextMenuTypes type = static_cast<SchedulerContextMenuTypes>(event.GetId());
	int id = -1;

	if (_scheduler) {
		id = _scheduler->id;
	}

	switch (type)
	{
	case SchedulerContextMenuTypes::Add:
		_commandsInvoker.NewScheduler();
		break;
	case SchedulerContextMenuTypes::Edit:
		_commandsInvoker.EditScheduler(id);
		break;
	case SchedulerContextMenuTypes::Delete:
		_commandsInvoker.DeleteScheduler(id);
		break;
	default:
		break;
	}
}