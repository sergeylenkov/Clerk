#include <wx/wx.h>
#include <wx/listctrl.h>
#include <wx/datectrl.h>
#include <wx/dateevt.h>
#include <wx/bmpcbox.h>
#include <algorithm>
#include <map>
#include "../DataPanel.h"
#include "../Charts/LineChart.h"
#include "ExpensesTooltipPopup.h"
#include "../../Data/Settings.h"
#include "../../Data/ViewModels/DateValueViewModel.h"

using namespace Clerk::Data;
using namespace Clerk::Utils;

class ReportBalancePanel : public DataPanel
{
public:
	ReportBalancePanel(wxWindow *parent, DataContext& context);
	~ReportBalancePanel();

	void Update();

private:
	LineChart *chart;
	wxBitmapComboBox *accountList;
	wxComboBox *periodList;
	wxDatePickerCtrl *fromDatePicker;
	wxDatePickerCtrl *toDatePicker;
	std::vector<std::shared_ptr<AccountViewModel>> _accounts;
	ExpensesTooltipPopup *chartPopup;
	std::vector<DateValueViewModel> values;
	wxDateTime periodFromDate;
	wxDateTime periodToDate;

	void OnAccountSelect(wxCommandEvent &event);
	void OnDateChanged(wxDateEvent &event);
	void OnPeriodSelect(wxCommandEvent &event);
	void ShowPopup();
	void HidePopup();
	void UpdatePopup(int x, int y, int index);
	void RestoreFilterSettings();
	void SaveFilterSettings();
	void CalculatePeriod();
};