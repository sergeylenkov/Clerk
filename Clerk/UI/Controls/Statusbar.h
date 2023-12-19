#pragma once

#include <wx/wx.h>
#include <wx/numformatter.h>
#include "../../Utils/Utils.h"
#include "../../Utils/Icons.h"
#include "../ViewModels/StatusViewModel.h"

using namespace Clerk::Utils;
using namespace Clerk::Data;
using namespace Clerk::UI;

class Statusbar : public wxPanel
{
public:
	Statusbar(StatusViewModel& viewModel, Icons& icons, wxWindow *parent, const wxPoint& pos, const wxSize& size);
	~Statusbar();

private:
	StatusViewModel& _viewModel;
	Icons& _icons;
	wxStaticText* _periodLabel;
	wxStaticText* _receiptsLabel;
	wxStaticText* _expensesLabel;
	wxStaticText* _balanceLabel;
	wxStaticText* _exchangeRatesLabel;	

	void Update();
};