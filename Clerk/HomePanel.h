#include <wx/wx.h>
#include <wx/listctrl.h>
#include <algorithm>
#include <map>
#include "DataHelper.h"

using namespace std;

class HomePanel : public wxPanel 
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