#include <wx/wx.h>
#include <wx/numformatter.h>
#include <vector>
#include <algorithm>
#include "../../Data/DataHelper.h"

class BarChart : public wxPanel {
public:
	BarChart(wxWindow *parent, wxWindowID id);
	~BarChart();

	void SetValues(std::vector<StringValue> values);
	void Draw();

private:
	std::vector<StringValue> _values;
	float stepX;
	float stepY;
	int offsetX;
	int offsetY;

	void OnPaint(wxPaintEvent& event);
};