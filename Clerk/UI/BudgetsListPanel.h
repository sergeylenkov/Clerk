#include <wx/wx.h>
#include <wx/listctrl.h>
#include "../Data/DataHelper.h"
#include "DataPanel.h"

enum {
	ID_EditBudget = 10,
	ID_DeleteBudget = 11,	
};


class BudgetsListPanel : public DataPanel
{
public:
	BudgetsListPanel(wxWindow *parent, wxWindowID id);

	shared_ptr<Budget> GetBudget();
	void Update();
	void EditBudget();
	void DeleteBudget();

	std::function<void()> OnEditBudget;

private:
	wxListCtrl *budgetsList;
	vector<shared_ptr<Budget>> budgets;

	void OnListItemClick(wxListEvent &event);	
	void OnListItemDoubleClick(wxListEvent &event);
	void OnMenuSelect(wxCommandEvent &event);
};