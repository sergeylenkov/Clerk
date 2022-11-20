#pragma once

#include <wx/wx.h>
#include <wx/popupwin.h>
#include <wx/listctrl.h>
#include <wx/numformatter.h>
#include <vector>
#include <memory>
#include <algorithm>
#include "../PresentationModels//Types.h"
#include "../../Utils/Utils.h"

using namespace Clerk::Utils;
using namespace Clerk::Data;
using namespace Clerk::UI;

class ExpensesTooltipPanel : public wxPanel
{
public:
	ExpensesTooltipPanel(wxWindow *parent);

	void SetTitle(wxString title);
	void SetTotal(float total);
	void SetValues(std::vector<StringValueViewModel> values);
	void Update();

private:
	wxString title;
	float total;
	std::vector<StringValueViewModel> values;

	void Draw(wxPaintDC &dc);
	void OnPaint(wxPaintEvent& event);
};

class ExpensesTooltipPopup : public wxPopupWindow
{
public:
	ExpensesTooltipPopup(wxWindow *parent);
	~ExpensesTooltipPopup();

	void Update(wxString title, std::vector<StringValueViewModel> values);

private:	
	ExpensesTooltipPanel *panel;
};
