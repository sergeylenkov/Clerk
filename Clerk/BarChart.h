#include <wx/wx.h>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;

class BarChart : public wxPanel {
public:
	BarChart(wxWindow *parent, wxWindowID id);
	~BarChart();

	void SetValues(map<wxString, float> values);
	void Draw();

private:
	map<wxString, float> _values;
	void OnPaint(wxPaintEvent& event);
};