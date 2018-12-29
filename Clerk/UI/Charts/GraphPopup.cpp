#include "GraphPopup.h"

GraphPopup::GraphPopup(wxWindow *parent) : wxPopupWindow(parent) {
	wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

	list = new wxListCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_NO_HEADER | wxLC_REPORT);
	mainSizer->Add(list, 1, wxEXPAND, 0);

	this->SetSizer(mainSizer);
	this->Layout();
}

GraphPopup::~GraphPopup() {
}

void GraphPopup::Update(vector<StringValue> values) {
	float total = 0;

	for (unsigned int i = 0; i < values.size(); i++) {
		total = total + values[i].value;
	}

	sort(values.begin(), values.end(), [](StringValue a, StringValue b) {
		return a.value > b.value;
	});

	list->ClearAll();

	wxListItem col;

	col.SetId(0);
	col.SetText(wxT("Name"));
	col.SetWidth(100);

	list->InsertColumn(0, col);

	wxListItem col2;

	col2.SetId(1);
	col2.SetText(wxT("Amount"));
	col2.SetWidth(100);
	col2.SetAlign(wxLIST_FORMAT_RIGHT);

	list->InsertColumn(1, col2);

	wxListItem listItem;

	listItem.SetId(0);	
	listItem.SetFont(this->GetFont().Bold());

	list->InsertItem(listItem);

	list->SetItem(0, 0, wxT("Total"));
	list->SetItem(0, 1, wxString::Format("%.2f", total));

	for (unsigned int i = 0; i < values.size(); i++) {
		wxListItem listItem;

		listItem.SetId(i + 1);

		list->InsertItem(listItem);

		list->SetItem(i + 1, 0, values[i].string);
		list->SetItem(i + 1, 1, wxString::Format("%.2f", values[i].value));
	}

	wxSize size(200, 200);
	wxRect rect;

	list->GetItemRect(0, rect);

	size.y = (values.size() + 1) * rect.GetHeight() + 5;

	list->SetSize(size);

	if (size.y > 400) {
		size.y = 400;
		list->SetColumnWidth(1, 90);
	}

	SetClientSize(size);
}