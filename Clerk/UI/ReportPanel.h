#include <wx/wx.h>
#include <wx/listctrl.h>
#include <wx/datectrl.h>
#include <wx/dateevt.h>
#include <wx/bmpcbox.h>
#include <algorithm>
#include <map>
#include "DataPanel.h"
#include "../Data/DataHelper.h"
#include "../LineChart.h"
#include "../GraphPopup.h"

using namespace std;

class ReportPanel : public DataPanel
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
	GraphPopup *chartPopup;
	vector<DateValue> values = {};

	void OnAccountSelect(wxCommandEvent &event);
	void OnDateChanged(wxDateEvent &event);
	void ShowPopup();
	void HidePopup();
	void UpdatePopup(int x, int y, int index);
};