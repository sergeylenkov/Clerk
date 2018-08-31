#include "BudgetFrame.h"

BudgetFrame::BudgetFrame(wxFrame *parent, const wxChar *title, int x, int y, int width, int height) : wxFrame(parent, -1, title, wxPoint(x, y), wxSize(width, height), wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX)) {
	SetBackgroundColour(wxColor(*wxWHITE));

	wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

	wxFlexGridSizer *fieldsSizer = new wxFlexGridSizer(6, 2, 10, 0);
	fieldsSizer->AddGrowableCol(1);
	fieldsSizer->AddGrowableRow(5);
	fieldsSizer->SetFlexibleDirection(wxBOTH);
	fieldsSizer->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

	nameLabel = new wxStaticText(this, wxID_ANY, "Name:", wxDefaultPosition, wxDefaultSize, 0);
	nameLabel->Wrap(-1);
	fieldsSizer->Add(nameLabel, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

	nameField = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, 0);
	fieldsSizer->Add(nameField, 0, wxALL | wxEXPAND | wxLEFT | wxRIGHT | wxALIGN_CENTER_VERTICAL, 5);

	typeLabel = new wxStaticText(this, wxID_ANY, "Type:", wxDefaultPosition, wxDefaultSize, 0);
	typeLabel->Wrap(-1);
	fieldsSizer->Add(typeLabel, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

	typeList = new wxComboBox(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_DROPDOWN | wxCB_READONLY);
	fieldsSizer->Add(typeList, 0, wxALL | wxEXPAND, 5);

	periodLabel = new wxStaticText(this, wxID_ANY, "Period:", wxDefaultPosition, wxDefaultSize, 0);
	periodLabel->Wrap(-1);
	fieldsSizer->Add(periodLabel, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

	periodList = new wxComboBox(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_DROPDOWN | wxCB_READONLY);
	fieldsSizer->Add(periodList, 0, wxALL | wxEXPAND, 5);

	accountLabel = new wxStaticText(this, wxID_ANY, "Account:", wxDefaultPosition, wxDefaultSize, 0);
	fieldsSizer->Add(accountLabel, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

	accountList = new wxBitmapComboBox(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_DROPDOWN | wxCB_READONLY);
	fieldsSizer->Add(accountList, 0, wxALL, 5);

	amountLabel = new wxStaticText(this, wxID_ANY, "Amount:", wxDefaultPosition, wxDefaultSize, 0);
	fieldsSizer->Add(amountLabel, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

	wxFloatingPointValidator<float> validator(2, &amountValue, wxNUM_VAL_DEFAULT);
	validator.SetRange(0.0f, 999999999.0f);

	amountField = new wxTextCtrl(this, wxID_ANY, "0.00", wxDefaultPosition, wxDefaultSize, wxTE_RIGHT, validator);
	fieldsSizer->Add(amountField, 0, wxALL, 5);

	mainSizer->Add(fieldsSizer, 1, wxALL | wxEXPAND, 5);

	wxBoxSizer *buttonsSizer = new wxBoxSizer(wxHORIZONTAL);

	okButton = new wxButton(this, wxID_ANY, wxT("OK"), wxDefaultPosition, wxDefaultSize, 0);
	buttonsSizer->Add(okButton, 0, wxALIGN_CENTER | wxRIGHT, 10);

	cancelButton = new wxButton(this, wxID_ANY, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0);
	buttonsSizer->Add(cancelButton, 0, wxALL, 5);

	mainSizer->Add(buttonsSizer, 0, wxALIGN_RIGHT | wxALL, 5);

	this->SetSizer(mainSizer);
	this->Layout();

	this->Centre(wxBOTH);

	typeList->AppendString("Limit");
	typeList->AppendString("Goal");

	typeList->SetSelection(0);

	periodList->AppendString("Week");
	periodList->AppendString("Month");
	periodList->AppendString("Year");
	periodList->AppendString("Custom");

	periodList->SetSelection(1);

	accountsImageList = new wxImageList(16, 16, false);

	for (int i = 0; i <= 50; i++) {
		wxString path = wxString::Format("Resources\\Accounts Icons\\%d.png", i);
		wxImage image(path);

		if (image.IsOk())
		{
			wxBitmap *bitmap = new wxBitmap(image);
			accountsImageList->Add(*bitmap);

			delete bitmap;
		}
	}

	UpdateAccounts();

	okButton->Bind(wxEVT_BUTTON, &BudgetFrame::OnOK, this);
	cancelButton->Bind(wxEVT_BUTTON, &BudgetFrame::OnCancel, this);
}

BudgetFrame::~BudgetFrame() {
	delete nameLabel;
	delete nameField;
	delete typeLabel;
	delete typeList;
	delete periodLabel;
	delete periodList;
	delete accountLabel;
	delete accountList;
	delete amountLabel;
	delete amountField;
	delete okButton;
	delete cancelButton;
	delete accountsImageList;
}

void BudgetFrame::SetBudget(std::shared_ptr<Budget> budget) {
	this->budget = budget;

	nameField->SetValue(*this->budget->name);
	typeList->SetSelection(static_cast<int>(this->budget->type));
	periodList->SetSelection(static_cast<int>(this->budget->period));
	amountField->SetValue(wxString::Format("%.2f", this->budget->amount));

	if (this->budget->account) {
		for (unsigned int i = 0; i < accounts.size(); i++) {
			if (this->budget->account->id == accounts[i]->id) {
				accountList->Select(i);
				break;
			}
		}
	}
}

void BudgetFrame::UpdateAccounts() {
	accounts = DataHelper::GetInstance().GetAccounts(AccountTypes::Expens);

	for each (auto account in accounts)
	{
		int icon = 0;

		if (account->iconId < accountsImageList->GetImageCount()) {
			icon = account->iconId;
		}

		accountList->Append(*account->name, accountsImageList->GetBitmap(icon));
	}
}

void BudgetFrame::OnOK(wxCommandEvent &event) {
	double val;

	amountField->GetValue().ToDouble(&val);
	amountValue = val;

	budget->name = make_shared<wxString>(nameField->GetValue());
	budget->type = static_cast<BudgetTypes>(typeList->GetSelection());
	budget->period = static_cast<BudgetPeriods>(periodList->GetSelection());
	budget->amount = amountValue;

	int index = accountList->GetSelection();

	if (index != -1) {
		budget->account = accounts[index];
	}	

	budget->Save();

	Close();

	if (OnClose) {
		OnClose();
	}
}

void BudgetFrame::OnCancel(wxCommandEvent &event) {
	Close();
}