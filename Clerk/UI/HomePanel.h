#include <wx/wx.h>
#include <wx/listctrl.h>
#include <algorithm>
#include <map>
#include "../Data/DataHelper.h"
#include "DataPanel.h"

using namespace std;

class HomePanel : public DataPanel 
{
public:
	HomePanel(wxWindow *parent, wxWindowID id);
	~HomePanel();

	void Update();

private:
	wxStaticText *titleLabel;
	wxStaticText *expensesLabel;
	wxStaticText *receiptsLabel;
	wxStaticText *accountsLabel;
	wxListCtrl *accountsList;
	wxStaticText *expensesListLabel;
	wxListCtrl *expensesList;
};