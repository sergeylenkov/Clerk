#pragma once

#include <wx/wx.h>
#include <wx/combo.h>
#include <wx/listctrl.h>

class CheckboxComboPopup : public wxListCtrl, public wxComboPopup
{
public:
    virtual void Init() override;
    virtual bool Create(wxWindow *parent) override;
    virtual wxWindow *GetControl() override;
    virtual void SetStringValue(const wxString &s) override;
    virtual wxString GetStringValue() const override;
    
    std::function<void(wxListItem item)> OnItemSelect;

private:
    wxString _value;

    void OnMouseMove(wxMouseEvent& event);
    void OnMouseClick(wxMouseEvent& event);
    void OnCheck(wxListEvent& event);
};

