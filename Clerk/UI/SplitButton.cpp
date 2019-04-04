#include "SplitButton.h"
#include <wx/renderer.h>

SplitButton::SplitButton(wxWindow *parent, wxWindowID id, const wxString& label, const wxPoint& pos, const wxSize& size) : wxPanel(parent, id, pos, size, wxBORDER_NONE | wxTAB_TRAVERSAL, "DropDownButton"),
	m_label(label)
{
	m_colorNormal = GetForegroundColour();
	m_colorDisabled = GetForegroundColour().MakeDisabled();

	if (size == wxDefaultSize)
	{
		wxSize defaultSize = wxButton::GetDefaultSize();

		wxSize textSize = GetTextExtent(m_label);
		textSize.SetWidth(textSize.GetWidth() + m_arrowButtonWidth + 20);
		SetMinSize(wxSize(textSize.GetWidth(), defaultSize.GetHeight()));
	}

	Bind(wxEVT_PAINT, &SplitButton::OnPaint, this);
	Bind(wxEVT_LEFT_UP, &SplitButton::OnLeftButtonUp, this);
	Bind(wxEVT_LEFT_DOWN, &SplitButton::OnLeftButtonDown, this);
	Bind(wxEVT_KILL_FOCUS, &SplitButton::OnKillFocus, this);
	Bind(wxEVT_LEAVE_WINDOW, &SplitButton::OnMouseLeave, this);
	Bind(wxEVT_ENTER_WINDOW, &SplitButton::OnMouseEnter, this);

	m_pMenu = new wxMenu();
	m_pMenu->Append(1, wxT("Transaction 1"));
	m_pMenu->Append(1, wxT("Transaction 2"));
}

SplitButton::~SplitButton()
{
	delete m_pMenu;
	m_pMenu = nullptr;
}

wxMenu* SplitButton::GetSplitButtonMenu()
{
	return m_pMenu;
}

void SplitButton::OnKillFocus(wxFocusEvent& event)
{
	m_stateButton = wxCONTROL_CURRENT;
	m_stateMenu = wxCONTROL_CURRENT;
	Refresh();

	event.Skip();
}

void SplitButton::OnMouseLeave(wxMouseEvent& event)
{
	m_stateButton = 0;
	m_stateMenu = 0;
	Refresh();

	event.Skip();
}

void SplitButton::OnMouseEnter(wxMouseEvent& event)
{
	m_stateButton = wxCONTROL_CURRENT;
	m_stateMenu = wxCONTROL_CURRENT;
	Refresh();

	event.Skip();
}

void SplitButton::OnLeftButtonUp(wxMouseEvent& event)
{
	m_stateButton = 0;
	m_stateMenu = 0;

	Refresh();

	int x = -1;
	int y = -1;
	event.GetPosition(&x, &y);

	if (x < (GetSize().GetWidth() - m_arrowButtonWidth))
	{
		wxEvtHandler* pEventHandler = GetEventHandler();
		wxASSERT(pEventHandler);

		pEventHandler->CallAfter([=]()
		{
			wxCommandEvent evt(wxEVT_BUTTON, this->GetId());
			evt.SetEventObject(this);
			GetEventHandler()->ProcessEvent(evt);
		});
	}

	m_bLButtonDown = false;

	event.Skip();
}

void SplitButton::OnLeftButtonDown(wxMouseEvent& event)
{
	m_bLButtonDown = true;

	int x = -1;
	int y = -1;
	event.GetPosition(&x, &y);

	if (x >= (GetSize().GetWidth() - m_arrowButtonWidth))
	{
		m_stateButton = 0;
		m_stateMenu = wxCONTROL_PRESSED;
		Refresh();

		wxSize size = GetSize();
		wxPoint position;
		position.x = 0;
		position.y = size.GetHeight();
		PopupMenu(m_pMenu, position);

		m_stateMenu = 0;
		Refresh();
	}
	else
	{
		m_stateButton = wxCONTROL_PRESSED;
		m_stateMenu = wxCONTROL_PRESSED;
		Refresh();
	}

	event.Skip();
}

void SplitButton::OnPaint(wxPaintEvent& WXUNUSED(event))
{
	wxPaintDC dc(this);
	wxSize size = GetSize();
	const int width = size.GetWidth() - m_arrowButtonWidth;

	// Draw first part of button
	wxRect r1;
	r1.x = 0;
	r1.y = 0;
	r1.width = width + 2;
	r1.height = size.GetHeight();

	wxRendererNative::Get().DrawPushButton(this, dc, r1, m_stateButton);

	SetForegroundColour(m_bIsEnable ? m_colorNormal : m_colorDisabled);

	r1.y += (size.GetHeight() - GetCharHeight()) / 2;
	dc.DrawLabel(m_label, r1, wxALIGN_CENTER_HORIZONTAL);

	// Draw second part of button
	wxRect r2;
	r2.x = width - 2;
	r2.y = 0;
	r2.width = m_arrowButtonWidth;
	r2.height = size.GetHeight();

	wxRendererNative::Get().DrawPushButton(this, dc, r2, m_stateMenu);
	wxRendererNative::Get().DrawDropArrow(this, dc, r2, m_stateMenu);
}

bool SplitButton::Enable(bool enable)
{
	m_bIsEnable = enable;
	wxPanel::Enable(m_bIsEnable);

	if (m_bIsEnable)
	{
		m_stateButton = 0;
		m_stateMenu = 0;
	}
	else
	{
		m_stateButton = wxCONTROL_DISABLED;
		m_stateMenu = wxCONTROL_DISABLED;
	}

	wxPaintEvent event;
	ProcessEvent(event);
	Refresh();

	return enable;
}