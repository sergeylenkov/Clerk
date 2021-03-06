#pragma once

#include <wx/wx.h>
#include <wx/numformatter.h>
#include "../../Data/ViewModels/StatusViewModel.h"

using namespace Clerk::Data;

class Statusbar : public wxPanel
{
public:
	Statusbar(wxWindow *parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize);
	~Statusbar();
	void SetViewModel(StatusViewModel* viewModel);

private:
	wxStaticText* _periodLabel;
	wxStaticText* _receiptsLabel;
	wxStaticText* _expensesLabel;
	wxStaticText* _balanceLabel;
	wxStaticText* _exchangeRatesLabel;
	StatusViewModel* _viewModel{};

	void Update();
};