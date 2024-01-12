#include "SchedulerDialog.h"

SchedulerDialog::SchedulerDialog(wxFrame *parent, const wxChar *title, int x, int y, int width, int height, Icons& icons, DataContext& context):
	wxFrame(parent, -1, title, wxPoint(x, y), wxSize(width, height), wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX)),
	_icons(icons),
	_context(context)
{
	SetBackgroundColour(wxColor(*wxWHITE));

	SetIcon(wxICON(APP_ICON));

	int indent = FromDIP(5);
	int bottomIndent = FromDIP(15);
	wxSize labelSize = FromDIP(wxSize(40, -1));	
		
	wxPanel* mainPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
	wxBoxSizer *panelSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText* nameLabel = new wxStaticText(mainPanel, wxID_ANY, _("Name:"), wxDefaultPosition, labelSize);
	horizontalSizer->Add(nameLabel, 0, wxALIGN_CENTER_VERTICAL);

	_nameField = new wxTextCtrl(mainPanel, wxID_ANY);
	horizontalSizer->Add(_nameField, 1, wxEXPAND);

	panelSizer->Add(horizontalSizer, 0, wxEXPAND | wxBOTTOM, bottomIndent);
		
	_schedulerPatternPanel = new SchedulerPatternPanel(mainPanel, wxDefaultPosition, wxDefaultSize);
	panelSizer->Add(_schedulerPatternPanel, 0, wxEXPAND | wxBOTTOM, bottomIndent);

	_schedulerTransactionPanel = new SchedulerTransactionPanel(mainPanel, wxDefaultPosition, wxDefaultSize, _icons, _context.GetTagsService());
	panelSizer->Add(_schedulerTransactionPanel, 1, wxEXPAND | wxBOTTOM, bottomIndent);

	horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	wxButton* okButton = new wxButton(mainPanel, wxID_ANY, _("OK"));
	horizontalSizer->Add(okButton, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, indent);

	wxButton* cancelButton = new wxButton(mainPanel, wxID_ANY, _("Cancel"));
	horizontalSizer->Add(cancelButton, 0, wxALIGN_CENTER_VERTICAL);

	panelSizer->Add(horizontalSizer, 0, wxALIGN_RIGHT);

	mainPanel->SetSizer(panelSizer);
	mainPanel->Layout();

	panelSizer->Fit(mainPanel);

	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
	mainSizer->Add(mainPanel, 1, wxEXPAND | wxALL, indent * 2);

	SetSizer(mainSizer);
	Layout();

	Centre(wxBOTH);

	okButton->Bind(wxEVT_BUTTON, &SchedulerDialog::OnOK, this);
	cancelButton->Bind(wxEVT_BUTTON, &SchedulerDialog::OnCancel, this);	

	Bind(wxEVT_CHAR_HOOK, &SchedulerDialog::OnKeyDown, this);
}

SchedulerDialog::~SchedulerDialog() {
	delete _viewModel;
}

void SchedulerDialog::SetViewModel(SchedulerViewModel* viewModel) {
	_viewModel = viewModel;

	_schedulerPatternPanel->SetViewModel(_viewModel);
	_schedulerTransactionPanel->SetViewModel(_viewModel);

	Update();

	_nameField->SetValue(_viewModel->GetName());
	_nameField->SetFocus();
}

void SchedulerDialog::Update() {
	
}

void SchedulerDialog::OnOK(wxCommandEvent &event) {
	_viewModel->Save();

	Close();
}

void SchedulerDialog::OnCancel(wxCommandEvent &event) {
	Close();
}

void SchedulerDialog::OnKeyDown(wxKeyEvent &event) {
	if ((int)event.GetKeyCode() == 27) {
		event.StopPropagation();
		Close();
	}
	else {
		event.Skip();
	}
}