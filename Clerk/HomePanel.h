#include <wx/wx.h>
#include <wx/listctrl.h>
#include "DataHelper.h"
#include "BarChart.h"
#include "PieChart.h"

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
	BarChart *barChart;
	PieChart *pieChart;
};