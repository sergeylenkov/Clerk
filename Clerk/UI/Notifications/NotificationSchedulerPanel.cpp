#include "NotificationSchedulerPanel.h"

NotificationSchedulerPanel::NotificationSchedulerPanel(wxWindow* parent, const wxPoint& position, const wxSize& size) : wxPanel(parent, wxID_ANY, position, size)
{
	SetBackgroundColour(wxColor(255, 255, 255));

	SetSize(size);

	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

	_label = new wxStaticText(this, wxID_ANY, "", wxDefaultPosition, size);

	mainSizer->Add(_label, 0, wxALL, FromDIP(10));

	wxBoxSizer* horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	wxButton* execButton = new wxButton(this, wxID_ANY, _("Execute"));
	horizontalSizer->Add(execButton, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, FromDIP(10));

	wxButton* skipButton = new wxButton(this, wxID_ANY, _("Skip"));
	horizontalSizer->Add(skipButton, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, FromDIP(10));

	wxButton* dismissButton = new wxButton(this, wxID_ANY, _("Dismiss"));
	horizontalSizer->Add(dismissButton, 0, wxALIGN_CENTER_VERTICAL);

	mainSizer->Add(horizontalSizer, 0, wxALIGN_RIGHT | wxALL, FromDIP(10));

	SetSizer(mainSizer);
	Layout();

	execButton->Bind(wxEVT_BUTTON, &NotificationSchedulerPanel::OnExecClick, this);
	skipButton->Bind(wxEVT_BUTTON, &NotificationSchedulerPanel::OnSkipClick, this);
	dismissButton->Bind(wxEVT_BUTTON, &NotificationSchedulerPanel::OnDismissClick, this);

	Bind(wxEVT_PAINT, &NotificationSchedulerPanel::OnPaint, this);
}

void NotificationSchedulerPanel::SetScheduler(std::shared_ptr<SchedulerPresentationModel> scheduler) {
	_scheduler = scheduler;

	_importance = AlertImportance::Low;	

	wxTimeSpan span = scheduler->nextDate - wxDateTime::Today();

	if (span.GetDays() > 1 && span.GetDays() <= 7) {
		_importance = AlertImportance::Medium;
	} else if (span.GetDays() > 7) {
		_importance = AlertImportance::High;
	}

	Update();
}

void NotificationSchedulerPanel::Update() {
	wxString label = wxString::Format("%s: %s %s", _("Scheduled transaction"), _scheduler->name, Format::Amount(_scheduler->toAmount));

	_label->SetLabel(label);
	_label->Wrap(GetSize().GetWidth());
}

void NotificationSchedulerPanel::OnPaint(wxPaintEvent& event)
{
	wxPaintDC dc(this);
	wxSize size = GetSize();

	wxColor color = Colors::ColorFoImportance(_importance);
	
	dc.SetPen(wxPen(color, 1));
	dc.SetBrush(wxBrush(color));

	dc.DrawRectangle(0, 0, 3, size.GetHeight());
}

void NotificationSchedulerPanel::OnDismissClick(wxCommandEvent& event) {
	if (OnDismiss) {
		OnDismiss(_scheduler);
	}
}

void NotificationSchedulerPanel::OnSkipClick(wxCommandEvent& event) {
	if (OnSkip) {
		OnSkip(_scheduler);
	}
}

void NotificationSchedulerPanel::OnExecClick(wxCommandEvent& event) {
	if (OnExec) {
		OnExec(_scheduler);
	}
}