#include <wx/wx.h>
#include <vector>
#include <algorithm>
#include <map>
#include <numeric>

class PieChart : public wxPanel {
public:
	PieChart(wxWindow *parent);
	~PieChart();

	void SetValues(std::map<wxString, float> values);
	void Draw();

private:
	std::map<wxString, float> _values;
	void OnPaint(wxPaintEvent& event);
};