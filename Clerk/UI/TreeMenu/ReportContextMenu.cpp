#include "ReportContextMenu.h"

const int reportOffset = 1000;

ReportContextMenu::ReportContextMenu(DataContext& context, Icons& icons, ReportPresentationModel& report):
	TreeContextMenu(context, icons),
	_report(report)
{
	wxMenuItem* item = Append(static_cast<int>(TreeContextMenuType::NewTab), _("Open in New Tab"));
	item->SetBitmap(_icons.GetIconByType(IconType::Tab));

	Bind(wxEVT_COMMAND_MENU_SELECTED, &ReportContextMenu::OnMenuSelect, this);
}

void ReportContextMenu::OnMenuSelect(wxCommandEvent& event) {
	_context.GetCommandsInvoker().OpenReportTab(_report.id);
}