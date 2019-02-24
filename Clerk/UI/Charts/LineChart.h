#pragma once

#include <wx/wx.h>
#include <wx/numformatter.h>
#include <vector>
#include <algorithm>
#include <map>
#include <functional>
#include "../../Data/DataHelper.h"

using namespace std;

class LineChart : public wxPanel
{
public:
	LineChart(wxWindow *parent, wxWindowID id);
	~LineChart();

	void SetValues(std::vector<StringValue> values);
	void Draw();

	std::function<void()> OnShowPopup;
	std::function<void()> OnHidePopup;
	std::function<void(int x, int y , int index)> OnUpdatePopup;

private:
	std::vector<StringValue> _values;
	float stepX;
	float stepY;
	int offsetX;
	int offsetY;
	int currentPopupIndex;

	void OnPaint(wxPaintEvent& event);
	void OnMouseMove(wxMouseEvent& event);	
};