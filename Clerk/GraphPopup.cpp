#include "GraphPopup.h"

GraphPopup::GraphPopup(wxWindow *parent) :wxPopupWindow(parent) {
	panel = new wxScrolledWindow(this, wxID_ANY);
	panel->SetBackgroundColour(*wxLIGHT_GREY);

	list = new wxListCtrl(panel, wxID_ANY, wxPoint(0, 0), wxSize(200, 200), wxLC_REPORT);

	wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
	topSizer->Add(list, 0, wxALL, 1);

	panel->SetAutoLayout(true);
	panel->SetSizer(topSizer);

	topSizer->Fit(panel);

	SetClientSize(panel->GetSize());
}

GraphPopup::~GraphPopup() {
}

void GraphPopup::Update(vector<StringValue> values) {
	float total = 0;

	for (unsigned int i = 0; i < values.size() - 1; i++) {
		total = total + values[i].value;
	}

	sort(values.begin(), values.end(), [](StringValue a, StringValue b) {
		return a.value > b.value;
	});

	list->ClearAll();

	wxListItem col;

	col.SetId(0);
	col.SetText(_("Total"));
	col.SetWidth(100);

	list->InsertColumn(0, col);

	wxListItem col2;

	col2.SetId(1);
	col2.SetText(wxString::Format("%.2f", total));
	col2.SetWidth(80);
	col2.SetAlign(wxLIST_FORMAT_RIGHT);

	list->InsertColumn(1, col2);

	for (unsigned int i = 0; i < values.size(); i++) {
		wxListItem listItem;

		listItem.SetId(i);

		list->InsertItem(listItem);

		list->SetItem(i, 0, values[i].string);
		list->SetItem(i, 1, wxString::Format("%.2f", values[i].value));
	}
}
