#include "AccountsComboBox.h"

AccountsComboBox::AccountsComboBox(wxWindow* parent, wxWindowID id, const wxString& value, const wxPoint& pos, const wxSize& size) : wxComboCtrl(parent, id, value, pos, size, wxCB_READONLY) {
	_accountsList = new CheckboxComboPopup();

	this->SetPopupControl(_accountsList);

	_accountsList->EnableCheckBoxes(true);
	_accountsList->OnItemSelect = std::bind(&AccountsComboBox::OnAccountSelect, this, std::placeholders::_1);

	_selectedIds = {};
	_ignoreSelection = false;
}

AccountsComboBox::~AccountsComboBox() {
	
}

void AccountsComboBox::SetAccounts(shared_vector<AccountPresentationModel> accounts) {
	this->_accounts = accounts;
	UpdateList();
}

void AccountsComboBox::SetSelection(std::set<int> ids) {
	this->_selectedIds = ids;

	UpdateNames();
	UpdateSelection();
}

void AccountsComboBox::UpdateList() {
	wxListItem column;

	column.SetId(0);
	column.SetText(_("Name"));
	column.SetWidth(180);

	_accountsList->InsertColumn(0, column);

	int i = 0;

	for (auto& account : _accounts)
	{
		wxListItem listItem;

		listItem.SetId(i);
		listItem.SetData(account->id);
		
		_accountsList->InsertItem(i, account->name, account->icon);

		i++;
	}
}

void AccountsComboBox::UpdateNames() {
	wxString names = "";

	if (_selectedIds.count(-1) > 0) {
		names = "All";
	} else {
		for (auto& account : _accounts) {
			if (_selectedIds.count(account->id) > 0) {
				names = names + wxString::Format("%s, ", account->name);
			}
		}

		names.RemoveLast(2);
	}

	this->SetValue(names);
	_accountsList->SetStringValue(names);
}

void AccountsComboBox::UpdateSelection() {	
	_ignoreSelection = true;
	int i = 0;

	for (auto& account : _accounts)
	{
		bool checked = _selectedIds.count(account->id) > 0;

		if (_selectedIds.count(-1) > 0) {
			checked = true;
		}

		_accountsList->CheckItem(i, checked);

		i++;
	}

	_ignoreSelection = false;
}

void AccountsComboBox::OnAccountSelect(int index) {
	if (_ignoreSelection) {
		return;
	}

	auto account = _accounts[index];

	if (account->id == -1) {
		if (_selectedIds.count(account->id) == 0) {
			_selectedIds.clear();

			for (auto& account : _accounts) {
				_selectedIds.insert(account->id);
			}
		} else {
			_selectedIds.clear();
		}
	}
	else {
		_selectedIds.erase(-1);

		if (_selectedIds.count(account->id) == 0) {
			_selectedIds.insert(account->id);
		}
		else {
			_selectedIds.erase(account->id);
		}
	}	

	UpdateNames();
	UpdateSelection();

	if (OnChange) {
		OnChange(_selectedIds);
	}
}