#include "InterestAccrualDialog.h"

InterestAccrualDialog::InterestAccrualDialog(wxFrame* parent, const wxChar* title, int x, int y, int width, int height) :
	wxFrame(parent, -1, title, wxPoint(x, y), wxSize(width, height), wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX)) {
	SetBackgroundColour(wxColor(*wxWHITE));

	SetIcon(wxICON(APP_ICON));

	int indent = FromDIP(5);
	int bottomIndent = FromDIP(15);
	wxSize labelSize = FromDIP(wxSize(80, -1));
	wxSize fieldSize = FromDIP(wxSize(100, -1));

	wxPanel* mainPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
	wxBoxSizer* panelSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText* amountLabel = new wxStaticText(mainPanel, wxID_ANY, _("Amount:"), wxDefaultPosition, labelSize);
	horizontalSizer->Add(amountLabel, 0, wxALIGN_CENTER_VERTICAL);

	_amountField = new AmountField(mainPanel, wxID_ANY, "0.00", wxDefaultPosition, fieldSize);
	horizontalSizer->Add(_amountField);

	panelSizer->Add(horizontalSizer, 0, wxEXPAND | wxBOTTOM, bottomIndent);

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

	_amountField->Bind(wxEVT_KILL_FOCUS, &InterestAccrualDialog::OnAmountKillFocus, this);
	okButton->Bind(wxEVT_BUTTON, &InterestAccrualDialog::OnOK, this);
	cancelButton->Bind(wxEVT_BUTTON, &InterestAccrualDialog::OnCancel, this);

	Bind(wxEVT_CHAR_HOOK, &InterestAccrualDialog::OnKeyDown, this);
}

InterestAccrualDialog::~InterestAccrualDialog() {
	delete _viewModel;
}

void InterestAccrualDialog::SetViewModel(InterestAccrualViewModel* viewModel) {
	_viewModel = viewModel;
	_viewModel->OnUpdate = [&](InterestAccrualViewModelField field) {
		if (field == InterestAccrualViewModelField::Amount) {
			_amountField->SetValue(Format::Amount(_viewModel->GetAmount()));
		}
	};

	Update();

	_amountField->SetFocus();
}

void InterestAccrualDialog::Update() {
	_amountField->SetValue(Format::Amount(_viewModel->GetAmount()));
}

void InterestAccrualDialog::OnAmountKillFocus(wxFocusEvent& event) {
	event.Skip();

	_viewModel->SetAmount(_amountField->GetFloatValue());
}

void InterestAccrualDialog::OnKeyDown(wxKeyEvent& event) {
	if ((int)event.GetKeyCode() == 27) {
		event.StopPropagation();
		Close();
	}
	else {
		event.Skip();
	}
}

void InterestAccrualDialog::OnOK(wxCommandEvent& event) {
	_viewModel->Save();

	Close();
}

void InterestAccrualDialog::OnCancel(wxCommandEvent& event) {
	Close();
}