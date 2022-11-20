#pragma once

#include <wx/wx.h>
#include <wx/numformatter.h>
#include <vector>
#include <algorithm>
#include <map>
#include <functional>
#include <numeric>
#include "../PresentationModels/Types.h"

using namespace Clerk::Data;
using namespace Clerk::UI;

class LineChart : public wxPanel
{
public:
	LineChart(wxWindow *parent, wxWindowID id);

	void SetValues(std::vector<StringValueViewModel> values);
	void SetDrawAverage(bool value);
	void Draw();

	std::function<void()> OnShowPopup;
	std::function<void()> OnHidePopup;
	std::function<void(int x, int y , int index)> OnUpdatePopup;

private:
	std::vector<StringValueViewModel> values;
	std::vector<std::pair<int, int>> points;
	float maxValue;
	float totalValue;
	int width;
	int height;
	int graphWidth;
	float stepX;
	float stepY;
	int offsetX;
	int offsetY;
	int currentPopupIndex;
	bool drawAverage;

	void DrawGraph();
	void OnPaint(wxPaintEvent& event);
	void OnMouseMove(wxMouseEvent& event);	
};