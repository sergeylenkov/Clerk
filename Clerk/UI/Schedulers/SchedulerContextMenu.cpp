#include "SchedulerContextMenu.h"

SchedulerContextMenu::SchedulerContextMenu(CommandsInvoker& commandsInvoker, std::shared_ptr<SchedulerPresentationModel> scheduler):
	_commandsInvoker(commandsInvoker),
	_scheduler(scheduler)
{
	if (scheduler) {
		wxMenuItem* item = Append(static_cast<int>(SchedulerContextMenuTypes::Add), _("Add..."));
		item->SetBitmap(wxBitmap("ICON_ADD", wxBITMAP_TYPE_PNG_RESOURCE));

		item = Append(static_cast<int>(SchedulerContextMenuTypes::Edit), _("Edit..."));
		item->SetBitmap(wxBitmap("ICON_EDIT", wxBITMAP_TYPE_PNG_RESOURCE));

		AppendSeparator();

		if (scheduler->isActive) {
			item = Append(static_cast<int>(SchedulerContextMenuTypes::Pause), _("Pause"));
		}
		else {
			item = Append(static_cast<int>(SchedulerContextMenuTypes::Run), _("Run"));
		}

		AppendSeparator();

		item = Append(static_cast<int>(SchedulerContextMenuTypes::Delete), _("Delete"));
		item->SetBitmap(wxBitmap("ICON_DELETE", wxBITMAP_TYPE_PNG_RESOURCE));
	}
	else {
		wxMenuItem* item = Append(static_cast<int>(SchedulerContextMenuTypes::Add), _("Add..."));
		item->SetBitmap(wxBitmap("ICON_ADD", wxBITMAP_TYPE_PNG_RESOURCE));
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