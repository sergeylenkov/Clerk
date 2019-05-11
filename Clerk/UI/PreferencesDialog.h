#pragma once

#include <wx/wx.h>
#include "../Data/DataHelper.h"
#include "../Data/Settings.h"

class PreferencesDialog : public wxFrame
{
public:
	PreferencesDialog(wxFrame *parent, const wxChar *title, int xpos, int ypos, int width, int height);

private:
	wxStaticText *currencyLabel;
	wxComboBox *currencyList;
	wxCheckBox *convertCurrenciesCheckBox;
	wxButton *okButton;
	wxButton *cancelButton;
	vector<std::shared_ptr<Currency>> currencies;

	void OnOK(wxCommandEvent &event);
	void OnCancel(wxCommandEvent &event);
	void OnKeyDown(wxKeyEvent &event);
};
