#pragma once

#include <wx/wx.h>
#include "../UI/PresentationModels/CurrencyPresentationModel.h"
#include "../Data/Settings.h"

using namespace Clerk::Data;
using namespace Clerk::UI;

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
	std::vector<std::shared_ptr<CurrencyPresentationModel>> currencies;

	void OnOK(wxCommandEvent &event);
	void OnCancel(wxCommandEvent &event);
	void OnKeyDown(wxKeyEvent &event);
};
