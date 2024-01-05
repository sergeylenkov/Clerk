#pragma once

#include <wx/wx.h>
#include <wx/combo.h>
#include <set>
#include <functional>
#include "../../Data/PresentationModels/AccountPresentationModel.h"
#include "../../Utils/Types.h"
#include "CheckboxComboPopup.h"

using namespace Clerk::Data;

class AccountsComboBox : public wxComboCtrl
{
public:
	AccountsComboBox(wxWindow* parent, const wxPoint& pos, const wxSize& size, bool showAllButton);

	std::function<void(std::set<int> ids)> OnChange;

	void SetAccounts(shared_vector<AccountPresentationModel> accounts);
	void SetSelection(std::set<int> ids);

private:
	CheckboxComboPopup *_accountsList;
	shared_vector<AccountPresentationModel> _accounts;
	std::set<int> _selectedIds;
	bool _ignoreSelection;
	bool _showAllButton;
	bool _allSelected;

	void UpdateList();
	void UpdateNames();
	void UpdateSelection();
	void OnAccountSelect(wxListItem item);
};

