#include <wx/wx.h>
#include <wx/numformatter.h>
#include <vector>
#include <algorithm>
#include "../../Data/PresentationModels/Types.h"

using namespace Clerk::Data;

class BarChart : public wxPanel {
public:
	BarChart(wxWindow *parent, wxWindowID id);
	~BarChart();

	void SetValues(std::vector<StringValueViewModel> values);
	void Draw();

private:
	std::vector<StringValueViewModel> _values;

	void OnPaint(wxPaintEvent& event);
};