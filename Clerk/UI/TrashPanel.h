#pragma once

#include <wx/wx.h>
#include <wx/listctrl.h>
#include "DataPanel.h"
#include "../Data/ViewModels/TransactionViewModel.h"

using namespace Clerk::Data;

enum class TrashPanelMenuTypes {
	Restore = 1,
	Delete = 2,
	ClearAll = 3,
};

class TrashPanel : public DataPanel
{
public:
	TrashPanel(wxWindow *parent, DataContext& context);

	std::shared_ptr<TransactionViewModel> GetTransaction();
	void Update();
	void RestoreTransaction();
	void DeleteTransaction();
	void ClearAll();

private:
	wxListCtrl *transactionsList;
	std::vector<std::shared_ptr<TransactionViewModel>> _transactions;

	void OnListItemClick(wxListEvent &event);
	void OnMenuSelect(wxCommandEvent &event);
};
