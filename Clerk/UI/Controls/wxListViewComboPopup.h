#pragma once

#include <wx/wx.h>
#include <wx/combo.h>
#include <wx/listctrl.h>

class wxListViewComboPopup : public wxListView, public wxComboPopup
{
    virtual void Init()
    {
        m_value = -1;
    }
    // Create popup control
    virtual bool Create(wxWindow* parent)
    {
        return wxListView::Create(parent, 1, wxPoint(0, 0), wxDefaultSize);
    }
    // Return pointer to the created control
    virtual wxWindow* GetControl() { return this; }
    // Translate string into a list selection
    virtual void SetStringValue(const wxString& s)
    {
        int n = wxListView::FindItem(-1, s);
        if (n >= 0 && n < wxListView::GetItemCount())
            wxListView::Select(n);
    }
    // Get list selection as a string
    virtual wxString GetStringValue() const
    {
        if (m_value >= 0)
            return wxListView::GetItemText(m_value);
        return wxEmptyString;
    }
    // Do mouse hot-tracking (which is typical in list popups)
    void OnMouseMove(wxMouseEvent& event)
    {
        // TODO: Move selection to cursor
    }
    // On mouse left up, set the value and close the popup
    void OnMouseClick(wxMouseEvent& WXUNUSED(event))
    {
        m_value = wxListView::GetFirstSelected();
        // TODO: Send event as well
        Dismiss();
    }
protected:
    int m_value; // current item index
};


