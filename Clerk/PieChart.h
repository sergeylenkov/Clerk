#include <wx/wx.h>
#include <vector>
#include <algorithm>
#include <map>
#include <numeric>

using namespace std;

class PieChart : public wxPanel {
public:
	PieChart(wxWindow *parent, wxWindowID id);
	~PieChart();

	void SetValues(map<wxString, float> values);
	void Draw();

private:
	map<wxString, float> _values;
	void OnPaint(wxPaintEvent& event);
};