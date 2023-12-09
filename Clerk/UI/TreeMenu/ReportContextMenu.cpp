#include "ReportContextMenu.h"

const int reportOffset = 1000;

ReportContextMenu::ReportContextMenu(CommandsInvoker& commandsInvoker, ReportPresentationModel& report):
	TreeContextMenu(commandsInvoker),
	_report(report)
{
	wxMenuItem* item = Append(static_cast<int>(TreeContextMenuType::NewTab), _("Open in New Tab"));
	item->SetBitmap(wxBitmap("ICON_NEW_TAB", wxBITMAP_TYPE_PNG_RESOURCE));	

	Bind(wxEVT_COMMAND_MENU_SELECTED, &ReportContextMenu::OnMenuSelect, this);
}

void ReportContextMenu::OnMenuSelect(wxCommandEvent& event) {
	_commandsInvoker.OpenReportTab(_report.id);
}