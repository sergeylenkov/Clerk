#include "SchedulerFrame.h"

SchedulerFrame::SchedulerFrame(wxFrame *parent, const wxChar *title, int x, int y, int width, int height) : wxFrame(parent, -1, title, wxPoint(x, y), wxSize(width, height), wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX)) {
	SetBackgroundColour(wxColor(*wxWHITE));

	wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *buttonsSizer = new wxBoxSizer(wxHORIZONTAL);

	okButton = new wxButton(this, wxID_ANY, wxT("OK"), wxDefaultPosition, wxDefaultSize, 0);
	buttonsSizer->Add(okButton, 0, wxALIGN_CENTER | wxRIGHT, 10);

	cancelButton = new wxButton(this, wxID_ANY, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0);
	buttonsSizer->Add(cancelButton, 0, wxALL, 5);

	mainSizer->Add(buttonsSizer, 0, wxALIGN_RIGHT | wxALL, 5);

	this->SetSizer(mainSizer);
	this->Layout();

	this->Centre(wxBOTH);

	okButton->Bind(wxEVT_BUTTON, &SchedulerFrame::OnOK, this);
	cancelButton->Bind(wxEVT_BUTTON, &SchedulerFrame::OnCancel, this);
}

SchedulerFrame::~SchedulerFrame() {
	delete okButton;
	delete cancelButton;
}

void SchedulerFrame::SetScheduler(std::shared_ptr<Scheduler> scheduler) {
	this->scheduler = scheduler;
}

void SchedulerFrame::OnOK(wxCommandEvent &event) {
	Close();

	if (OnClose) {
		OnClose();
	}
}

void SchedulerFrame::OnCancel(wxCommandEvent &event) {
	Close();
}