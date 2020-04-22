#include "AccountsComboBox.h"

AccountsComboBox::AccountsComboBox(wxWindow* parent, wxWindowID id, const wxString& value, const wxPoint& pos, const wxSize& size) : wxComboCtrl(parent, id, value, pos, size, wxCB_READONLY) {
	accountsList = new CheckboxComboPopup();

	this->SetPopupControl(accountsList);

	accountsList->EnableCheckBoxes(true);
	accountsList->SetImageList(DataHelper::GetInstance().accountsImageList, wxIMAGE_LIST_SMALL);

	accountsList->OnItemSelect = std::bind(&AccountsComboBox::OnAccountSelect, this, std::placeholders::_1);

	selectedIds = {};
	ignoreSelection = false;
}

AccountsComboBox::~AccountsComboBox() {
	
}

void AccountsComboBox::SetAccounts(vector<shared_ptr<Account>> accounts) {
	this->accounts = accounts;
	UpdateList();
}

void AccountsComboBox::SetSelection(std::set<int> ids) {
	this->selectedIds = ids;

	UpdateNames();
	UpdateSelection();
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

void AccountsComboBox::UpdateNames() {
	wxString names = "";

	if (selectedIds.count(-1) > 0) {
		names = "All";
	} else {
		for (auto& account : accounts) {
			if (selectedIds.count(account->id) > 0) {
				names = names + wxString::Format("%s, ", *account->name);
			}
		}

		names.RemoveLast(2);
	}

	this->SetValue(names);
	accountsList->SetStringValue(names);
}

void AccountsComboBox::UpdateSelection() {	
	ignoreSelection = true;
	int i = 0;

	for (auto& account : accounts)
	{
		bool checked = selectedIds.count(account->id) > 0;

		if (selectedIds.count(-1) > 0) {
			checked = true;
		}

		accountsList->CheckItem(i, checked);

		i++;
	}

	ignoreSelection = false;
}

void AccountsComboBox::OnAccountSelect(int index) {
	if (ignoreSelection) {
		return;
	}

	auto account = accounts[index];

	if (account->id == -1) {
		if (selectedIds.count(account->id) == 0) {
			selectedIds.clear();

			for (auto& account : accounts) {
				selectedIds.insert(account->id);
			}
		} else {
			selectedIds.clear();
		}
	}
	else {
		selectedIds.erase(-1);

		if (selectedIds.count(account->id) == 0) {
			selectedIds.insert(account->id);
		}
		else {
			selectedIds.erase(account->id);
		}
	}	

	UpdateNames();
	UpdateSelection();

	if (OnChange) {
		OnChange(selectedIds);
	}
}