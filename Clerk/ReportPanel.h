#include <wx/wx.h>
#include <wx/listctrl.h>
#include <algorithm>
#include <map>
#include "DataHelper.h"
#include "BarChart.h"

using namespace std;

class ReportPanel : public wxPanel
{
public:
	ReportPanel(wxWindow *parent, wxWindowID id);
	~ReportPanel();

	void Update();

private:
	BarChart *chart;
};