#ifndef LineChart_h
#define LineChart_h

#include <wx/wx.h>
#include <vector>
#include <algorithm>
#include <map>
#include <functional>
#include "DataHelper.h"

using namespace std;

class LineChart : public wxPanel
{
public:
	LineChart(wxWindow *parent, wxWindowID id);
	~LineChart();

	void SetValues(std::vector<DateValue> values);
	void Draw();

	std::function<void()> OnShowPopup;
	std::function<void()> OnHidePopup;
	std::function<void(int x, int y , int index)> OnUpdatePopup;

private:
	std::vector<DateValue> _values;
	float stepX;
	float stepY;
	int offsetX;
	int offsetY;
	int currentPopupIndex;

	void OnPaint(wxPaintEvent& event);
	void OnMouseMove(wxMouseEvent& event);
	void OnMouseEnter(wxMouseEvent& event);
	void OnMouseExit(wxMouseEvent& event);
};

#endif