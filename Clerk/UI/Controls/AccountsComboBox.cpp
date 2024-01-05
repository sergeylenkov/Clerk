#include "AccountsComboBox.h"

AccountsComboBox::AccountsComboBox(wxWindow* parent, const wxPoint& pos, const wxSize& size, bool showAllButton):
	wxComboCtrl(parent, wxID_ANY, "", pos, size, wxCB_READONLY),
	_showAllButton(showAllButton)
{
	_accountsList = new CheckboxComboPopup();

	SetPopupControl(_accountsList);

	_accountsList->EnableCheckBoxes(true);
	_accountsList->OnItemSelect = std::bind(&AccountsComboBox::OnAccountSelect, this, std::placeholders::_1);

	_selectedIds = {};
	_ignoreSelection = false;
	_allSelected = false;
}

void AccountsComboBox::SetAccounts(shared_vector<AccountPresentationModel> accounts) {
	_accounts = accounts;
	UpdateList();
}

void AccountsComboBox::SetSelection(std::set<int> ids) {
	_selectedIds = ids;

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

	if (_showAllButton) {
		wxListItem listItem;
				
		listItem.SetId(i);
		listItem.SetText(_("All"));
		listItem.SetData(-1);

		_accountsList->InsertItem(listItem);

		i++;
	}

	for (auto& account : _accounts)
	{
		wxListItem listItem;

		listItem.SetId(i);
		listItem.SetText(account->name);
		listItem.SetData(account->id);
		
		_accountsList->InsertItem(listItem);

		i++;
	}
}

void AccountsComboBox::UpdateNames() {
	wxString names = "";

	if (_allSelected) {
		names = _("All");
	} else {
		for (auto& account : _accounts) {
			if (_selectedIds.count(account->id) > 0) {
				names = names + wxString::Format("%s, ", account->name);
			}
		}

		names.RemoveLast(2);
	}

	SetValue(names);
	_accountsList->SetStringValue(names);
}

void AccountsComboBox::UpdateSelection() {	
	_ignoreSelection = true;
	int i = 0;

	if (_showAllButton) {
		_accountsList->CheckItem(i, _allSelected);
		i++;
	}
	
	for (auto& account : _accounts)
	{
		bool checked = _selectedIds.count(account->id) > 0;
		_accountsList->CheckItem(i, checked);

		i++;
	}

	_ignoreSelection = false;
}

void AccountsComboBox::OnAccountSelect(wxListItem item) {
	if (_ignoreSelection) {
		return;
	}
	
	int accountId = item.GetData();
	bool checked = _selectedIds.count(accountId) > 0;

	if (_showAllButton && accountId == -1) {
		_selectedIds.clear();
		_allSelected = !_allSelected;

		if (_allSelected) {
			for (auto& account : _accounts)
			{
				_selectedIds.insert(account->id);
			}
		}
	}
	else {
		_allSelected = false;

		if (_selectedIds.count(accountId) == 0) {
			_selectedIds.insert(accountId);
		}
		else {
			_selectedIds.erase(accountId);
		}
	}

	UpdateNames();
	UpdateSelection();

	if (OnChange) {
		OnChange(_selectedIds);
	}
}