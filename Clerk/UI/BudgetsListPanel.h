#include <wx/wx.h>
#include <wx/listctrl.h>
#include "../Data/DataHelper.h"
#include "DataPanel.h"

enum {
	ID_EditBudget = 1,
	ID_DeleteBudget = 2,	
};


class BudgetsListPanel : public DataPanel
{
public:
	BudgetsListPanel(wxWindow *parent, wxWindowID id);

	std::shared_ptr<Budget> GetBudget();
	void Update();
	void EditBudget();
	void DeleteBudget();

	std::function<void(std::shared_ptr<Budget> budget)> OnEditBudget;

private:
	wxListCtrl *budgetsList;
	std::vector<std::shared_ptr<Budget>> budgets;

	void OnListItemClick(wxListEvent &event);	
	void OnListItemDoubleClick(wxListEvent &event);
	void OnMenuSelect(wxCommandEvent &event);
};