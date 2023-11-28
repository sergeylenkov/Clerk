#pragma once

#include <set>
#include <wx/wx.h>
#include <wx/combo.h>
#include "./CheckboxComboPopup.h"
#include "../PresentationModels/AccountPresentationModel.h"
#include "../../Utils/Types.h"

using namespace Clerk::Data;
using namespace Clerk::UI;

class AccountsComboBox : public wxComboCtrl
{
public:
	AccountsComboBox(wxWindow* parent, wxWindowID id, const wxString& value = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize);
	~AccountsComboBox();

	std::function<void(std::set<int> ids)> OnChange;

	void SetAccounts(shared_vector<AccountPresentationModel> accounts);
	void SetSelection(std::set<int> ids);

private:
	CheckboxComboPopup *_accountsList;
	shared_vector<AccountPresentationModel> _accounts;
	std::set<int> _selectedIds;
	bool _ignoreSelection;

	void UpdateList();
	void UpdateNames();
	void UpdateSelection();
	void OnAccountSelect(int index);
};

