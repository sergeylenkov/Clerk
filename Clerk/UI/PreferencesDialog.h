#pragma once

#include <wx/wx.h>
#include <utility>
#include "../Data/DataContext.h"
#include "../Utils/Types.h"
#include "../Utils/Settings/Settings.h"

using namespace Clerk::Data;
using namespace Clerk::Utils;

class PreferencesDialog : public wxFrame
{
public:
	PreferencesDialog(wxFrame *parent, const wxChar *title, int xpos, int ypos, int width, int height, DataContext& context);

private:
	DataContext& _context;
	shared_vector<CurrencyPresentationModel> _currencies;
	std::vector<Language> _languages;

	wxComboBox* _languagesList;
	wxComboBox *_currenciesList;
	wxCheckBox *_convertCurrenciesCheckBox;
	wxCheckBox *_loadExchangeRatesCheckBox;	
	wxCheckBox* _showCreditFundsCheckBox;
	wxCheckBox* _showBaseCurrencyAmountCheckBox;

	void OnOK(wxCommandEvent &event);
	void OnCancel(wxCommandEvent &event);
	void OnKeyDown(wxKeyEvent &event);
};
