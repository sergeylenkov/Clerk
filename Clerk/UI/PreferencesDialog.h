#pragma once

#include <wx/wx.h>
#include "../Data/Models/Currency.h"
#include "../Data/Settings.h"

using namespace Clerk::Data;

class PreferencesDialog : public wxFrame
{
public:
	PreferencesDialog(wxFrame *parent, const wxChar *title, int xpos, int ypos, int width, int height);

private:
	wxStaticText *currencyLabel;
	wxComboBox *currencyList;
	wxCheckBox *convertCurrenciesCheckBox;
	wxCheckBox *loadExchangeRatesCheckBox;
	wxButton *okButton;
	wxButton *cancelButton;
	std::vector<std::shared_ptr<Currency>> currencies;

	void OnOK(wxCommandEvent &event);
	void OnCancel(wxCommandEvent &event);
	void OnKeyDown(wxKeyEvent &event);
};
