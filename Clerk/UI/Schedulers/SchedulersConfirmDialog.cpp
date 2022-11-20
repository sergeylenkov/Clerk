#include "SchedulersConfirmDialog.h"

SchedulersConfirmDialog::SchedulersConfirmDialog(wxFrame *parent, const wxChar *title, int x, int y, int width, int height) : wxFrame(parent, -1, title, wxPoint(x, y), wxSize(width, height), wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX)) {
	SetBackgroundColour(wxColor(*wxWHITE));

	this->SetSizeHints(wxDefaultSize, wxDefaultSize);
	this->SetIcon(wxICON(APP_ICON));

	wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

	wxStaticText *titleLabel = new wxStaticText(this, wxID_ANY, wxT("Confirm scheduler transactions:"), wxDefaultPosition, wxDefaultSize, 0);	
	mainSizer->Add(titleLabel, 0, wxALL, 10);

	list = new wxListCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT);
	mainSizer->Add(list, 1, wxLEFT | wxRIGHT | wxEXPAND, 10);

	wxPanel *buttonsPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(-1, -1), wxTAB_TRAVERSAL);
	wxBoxSizer *horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	skipAllButton = new wxButton(buttonsPanel, wxID_ANY, wxT("Skip All"), wxDefaultPosition, wxDefaultSize, 0);
	horizontalSizer->Add(skipAllButton, 0, wxALIGN_LEFT | wxALL, 5);

	skipButton = new wxButton(buttonsPanel, wxID_ANY, wxT("Skip"), wxDefaultPosition, wxDefaultSize, 0);
	horizontalSizer->Add(skipButton, 0, wxALIGN_LEFT | wxALL, 5);

	horizontalSizer->Add(0, 0, 1, wxEXPAND, 5);

	applyAllButton = new wxButton(buttonsPanel, wxID_ANY, wxT("Apply All"), wxDefaultPosition, wxDefaultSize, 0);
	horizontalSizer->Add(applyAllButton, 0, wxALL, 5);

	buttonsPanel->SetSizer(horizontalSizer);
	buttonsPanel->Layout();

	horizontalSizer->Fit(buttonsPanel);

	mainSizer->Add(buttonsPanel, 0, wxALL | wxEXPAND, 5);

	this->SetSizer(mainSizer);
	this->Layout();

	this->Centre(wxBOTH);

	skipAllButton->Bind(wxEVT_BUTTON, &SchedulersConfirmDialog::OnSkipAll, this);
	skipButton->Bind(wxEVT_BUTTON, &SchedulersConfirmDialog::OnSkip, this);
	applyAllButton->Bind(wxEVT_BUTTON, &SchedulersConfirmDialog::OnApplyAll, this);
	Bind(wxEVT_CLOSE_WINDOW, &SchedulersConfirmDialog::OnCloseWindow, this);
}

SchedulersConfirmDialog::~SchedulersConfirmDialog()
{
}

void SchedulersConfirmDialog::SetSchedulers(std::vector<std::shared_ptr<SchedulerPresentationModel>> schedulers) {
	_schedulers = schedulers;
	UpdateList();
}

void SchedulersConfirmDialog::UpdateList() {
	list->ClearAll();

	wxListItem column;

	column.SetId(0);
	column.SetText(_("Name"));
	column.SetWidth(280);

	list->InsertColumn(0, column);

	wxListItem column1;

	column1.SetId(1);
	column1.SetText(_("Amount"));
	column1.SetWidth(120);
	column1.SetAlign(wxLIST_FORMAT_RIGHT);

	list->InsertColumn(1, column1);

	int i = 0;

	for (auto &scheduler : _schedulers)
	{
		wxListItem listItem;

		listItem.SetId(i);
		listItem.SetData(scheduler->id);

		list->InsertItem(listItem);

		list->SetItem(i, 0, scheduler->name);
		list->SetItem(i, 1, wxString::Format("%.2f", scheduler->fromAmount));

		i++;
	}
}

int SchedulersConfirmDialog::SelectedItemIndex() {
	long itemIndex = -1;

	for (;;) {
		itemIndex = list->GetNextItem(itemIndex, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);

		if (itemIndex == -1) {
			break;
		}

		return itemIndex;
	}

	return itemIndex;
}

void SchedulersConfirmDialog::OnSkipAll(wxCommandEvent &event) {
	for (auto &scheduler : _schedulers)
	{
		//TODO moved method to interactor
		//scheduler->CalculateNextDate();
		//scheduler->Save();
	}

	Close();
}

void SchedulersConfirmDialog::OnSkip(wxCommandEvent &event) {
	int index = SelectedItemIndex();

	if (index != -1) {
		//TODO moved method to interactor
		//schedulers[index]->CalculateNextDate();
		//schedulers[index]->Save();

		_schedulers.erase(_schedulers.begin() + index);

		list->DeleteItem(index);
	}
}

void SchedulersConfirmDialog::OnApplyAll(wxCommandEvent &event) {
	for (auto &scheduler : _schedulers)
	{
		//TODO moved method to interactor
		//scheduler->Execute();
		//scheduler->CalculateNextDate();
		//scheduler->Save();
	}

	Close();
}

void SchedulersConfirmDialog::OnCloseWindow(wxCloseEvent &event) {
	if (OnClose) {
		OnClose();
	}

	Destroy();
}