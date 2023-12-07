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
	std::vector<StringValueViewModel> _values;
	std::vector<std::pair<int, int>> _points;
	float _maxValue;
	float _totalValue;
	int _width;
	int _height;
	int _graphWidth;
	float _stepX;
	float _stepY;
	int _offsetX;
	int _offsetY;
	int _currentPopupIndex;
	bool _drawAverage;

	void DrawGraph();
	void OnPaint(wxPaintEvent& event);
	void OnMouseMove(wxMouseEvent& event);	
};