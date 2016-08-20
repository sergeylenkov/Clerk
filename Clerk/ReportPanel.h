#include <wx/wx.h>
#include <wx/listctrl.h>
#include <wx/datectrl.h>
#include <wx/dateevt.h>
#include <wx/bmpcbox.h>
#include <algorithm>
#include <map>
#include "DataHelper.h"
#include "LineChart.h"

using namespace std;

class ReportPanel : public wxPanel
{
public:
	ReportPanel(wxWindow *parent, wxWindowID id);
	~ReportPanel();

	void Update();

private:
	LineChart *chart;
	wxBitmapComboBox *accountList;
	wxDatePickerCtrl *fromDatePicker;
	wxDatePickerCtrl *toDatePicker;
	vector<shared_ptr<Account>> accounts;
	wxImageList *accountsImageList;

	void OnAccountSelect(wxCommandEvent &event);
	void OnDateChanged(wxDateEvent &event);
};