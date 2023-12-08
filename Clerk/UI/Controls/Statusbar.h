#pragma once

#include <wx/wx.h>
#include <wx/numformatter.h>
#include "../../Utils/Utils.h"
#include "../ViewModels/StatusViewModel.h"

using namespace Clerk::Utils;
using namespace Clerk::Data;
using namespace Clerk::UI;

class Statusbar : public wxPanel
{
public:
	Statusbar(StatusViewModel& viewModel, wxWindow *parent, const wxPoint& pos, const wxSize& size);
	~Statusbar();

private:
	StatusViewModel& _viewModel;
	wxStaticText* _periodLabel;
	wxStaticText* _receiptsLabel;
	wxStaticText* _expensesLabel;
	wxStaticText* _balanceLabel;
	wxStaticText* _exchangeRatesLabel;	

	void Update();
};