#pragma once

#include <wx/wx.h>
#include "../../Utils/Utils.h"
#include "../../Utils/Icons.h"
#include "StatusbarViewModel.h"

using namespace Clerk::Utils;
using namespace Clerk::Data;
using namespace Clerk::UI;

class Statusbar : public wxPanel
{
public:
	Statusbar(StatusbarViewModel& viewModel, Icons& icons, wxWindow *parent, const wxPoint& pos, const wxSize& size);
	~Statusbar();

private:
	StatusbarViewModel& _viewModel;
	Icons& _icons;
	wxStaticText* _periodLabel;
	wxStaticText* _receiptsLabel;
	wxStaticText* _expensesLabel;
	wxStaticText* _balanceLabel;
	wxStaticText* _exchangeRatesLabel;	

	void Update();
};