#pragma once

#include <wx/wx.h>
#include <wx/listctrl.h>
#include "../Data/DataHelper.h"
#include "DataPanel.h"

enum class TrashPanelMenuTypes {
	Restore = 1,
	Delete = 2,
	ClearAll = 3,
};

class TrashPanel : public DataPanel
{
public:
	TrashPanel(wxWindow *parent, wxWindowID id);

	std::shared_ptr<Transaction> GetTransaction();
	void Update();
	void RestoreTransaction();
	void DeleteTransaction();
	void ClearAll();

private:
	wxListCtrl *transactionsList;
	std::vector<std::shared_ptr<Transaction>> transactions;

	void OnListItemClick(wxListEvent &event);
	void OnMenuSelect(wxCommandEvent &event);
};
