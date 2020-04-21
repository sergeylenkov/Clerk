#include "AccountsComboBox.h"

AccountsComboBox::AccountsComboBox(wxWindow* parent, wxWindowID id, const wxString& value, const wxPoint& pos, const wxSize& size) : wxComboCtrl(parent, id, value, pos, size, wxCB_READONLY) {
	accountsList = new CheckboxComboPopup();

	this->SetPopupControl(accountsList);

	accountsList->EnableCheckBoxes(true);
	accountsList->SetImageList(DataHelper::GetInstance().accountsImageList, wxIMAGE_LIST_SMALL);

	accountsList->OnItemSelect = std::bind(&AccountsComboBox::OnAccountSelect, this, std::placeholders::_1);

	selectedIds = {};
}

AccountsComboBox::~AccountsComboBox() {
	
}

void AccountsComboBox::SetAccounts(vector<shared_ptr<Account>> accounts) {
	this->accounts = accounts;
	UpdateList();
}

void AccountsComboBox::UpdateList() {
	wxListItem column;

	column.SetId(0);
	column.SetText(_("Name"));
	column.SetWidth(180);

	accountsList->InsertColumn(0, column);

	int i = 0;

	for (auto& account : accounts)
	{
		wxListItem listItem;

		listItem.SetId(i);
		listItem.SetData(account->id);

		accountsList->InsertItem(i, *account->name, account->iconId);

		i++;
	}
}

void AccountsComboBox::OnAccountSelect(int index) {
	if (selectedIds.count(index) == 0) {
		selectedIds.insert(index);
	}
	else {
		selectedIds.erase(index);
	}

	wxString names = "";

	for (auto id : selectedIds) {
		auto account = accounts[id];

		names = names + wxString::Format("%s, ", *account->name);
	}

	names.RemoveLast(2);

	this->SetValue(names);
	accountsList->SetStringValue(names);
}