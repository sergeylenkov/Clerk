#pragma once

#include <wx/wx.h>
#include <wx/numformatter.h>
#include <vector>
#include <algorithm>
#include <map>
#include <functional>
#include <numeric>
#include "../../Data/PresentationModels/Types.h"
#include "../../Utils/Utils.h"

using namespace Clerk::Data;
using namespace Clerk::Utils;

class GroupedBarChart : public wxPanel
{
public:
	GroupedBarChart(wxWindow* parent);

	void SetValues(std::vector<StringValuesViewModel> values);
	void Draw();

	std::function<void()> OnShowPopup;
	std::function<void()> OnHidePopup;
	std::function<void(int x, int y, int index)> OnUpdatePopup;

private:
	std::vector<StringValuesViewModel> _values;
	std::vector<std::pair<int, int>> _points;
	float _maxValue;
	float _totalValue;
	int _width;
	int _height;
	int _graphWidth;
	int _barGraphWidth;
	float _stepX;
	float _stepY;
	float _barStepX;
	int _offsetX;
	int _offsetY;
	int _barOffsetX;
	int _currentPopupIndex;
	int _barSize;
	int _barOffset;

	void DrawGraph();
	void DrawBars(wxClientDC& dc, std::vector<float> values, int x);
	void OnPaint(wxPaintEvent& event);
	void OnMouseMove(wxMouseEvent& event);
};