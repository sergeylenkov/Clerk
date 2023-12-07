#include "CheckboxComboPopup.h"

void CheckboxComboPopup::Init() {
    this->Bind(wxEVT_LIST_ITEM_CHECKED, &CheckboxComboPopup::OnCheck, this);
    this->Bind(wxEVT_LIST_ITEM_UNCHECKED, &CheckboxComboPopup::OnCheck, this);
}

bool CheckboxComboPopup::Create(wxWindow *parent) {
    return wxListCtrl::Create(parent, 1, wxPoint(0, 0), wxDefaultSize, wxLC_REPORT | wxLC_NO_HEADER);
}

wxWindow *CheckboxComboPopup::GetControl() {
    return this;
}

void CheckboxComboPopup::SetStringValue(const wxString &s) {
    _value = s;
}

wxString CheckboxComboPopup::GetStringValue() const {
    return _value;
}

void CheckboxComboPopup::OnCheck(wxListEvent &event) {
    if (OnItemSelect) {
        OnItemSelect(event.GetIndex());
    }
}

void CheckboxComboPopup::OnMouseMove(wxMouseEvent& event) {
    event.Skip();
}

void CheckboxComboPopup::OnMouseClick(wxMouseEvent& WXUNUSED(event)) {
    Dismiss();
}