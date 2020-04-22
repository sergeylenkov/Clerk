#pragma once

#include <set>
#include <wx/wx.h>
#include <wx/combo.h>
#include "CheckboxComboPopup.h"
#include "../../Data/DataHelper.h"
#include "../../Data/Account.h"

class AccountsComboBox : public wxComboCtrl
{
public:
	AccountsComboBox(wxWindow* parent, wxWindowID id, const wxString& value = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize);
	~AccountsComboBox();

	std::function<void(std::set<int> ids)> OnChange;

	void SetAccounts(std::vector<std::shared_ptr<Account>> accounts);
	void SetSelection(std::set<int> ids);

private:
	CheckboxComboPopup *accountsList;
	std::vector<std::shared_ptr<Account>> accounts;
	std::set<int> selectedIds;

	void UpdateList();
	void UpdateNames();
	void OnAccountSelect(int index);
};

