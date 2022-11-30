#include "DropDownButton.h"

DropDownButton::DropDownButton(wxWindow *parent, wxWindowID id, const wxString& label, const wxPoint& pos, const wxSize& size) : wxPanel(parent, id, pos, size, wxBORDER_NONE | wxTAB_TRAVERSAL, "DropDownButton")
{
	menu = new wxMenu();
	this->label = label;
	image = wxBitmap(wxT("ICON_NEW_TRANSACTION"), wxBITMAP_TYPE_PNG_RESOURCE);

	wxSize defaultSize = wxButton::GetDefaultSize();

	wxSize textSize = GetTextExtent(label);
	textSize.SetWidth(textSize.GetWidth() + arrowButtonWidth + 16);

	int width = textSize.GetWidth() + 16;

	defaultSize.SetWidth(width);

	if (size == wxDefaultSize)
	{
		defaultSize.SetHeight(defaultSize.GetHeight());
	}
	else {
		if (size.GetWidth() != -1) {
			defaultSize.SetWidth(size.GetWidth());			
		}

		if (size.GetHeight() != -1) {
			defaultSize.SetHeight(size.GetHeight());
		}
	}

	SetMinSize(defaultSize);

	Bind(wxEVT_PAINT, &DropDownButton::OnPaint, this);
	Bind(wxEVT_LEFT_UP, &DropDownButton::OnLeftButtonUp, this);
	Bind(wxEVT_LEFT_DOWN, &DropDownButton::OnLeftButtonDown, this);
	Bind(wxEVT_KILL_FOCUS, &DropDownButton::OnKillFocus, this);
	Bind(wxEVT_LEAVE_WINDOW, &DropDownButton::OnMouseLeave, this);
	Bind(wxEVT_ENTER_WINDOW, &DropDownButton::OnMouseEnter, this);
	Bind(wxEVT_MENU_CLOSE, &DropDownButton::OnMenuClose, this);
}

DropDownButton::~DropDownButton()
{
	delete menu;
}

wxMenu* DropDownButton::GetMenu()
{
	return menu;
}

void DropDownButton::ClearMenu() {
	wxMenuItemList items = menu->GetMenuItems();

	wxMenuItemList::iterator iter;

	for (iter = items.begin(); iter != items.end(); ++iter)
	{
		wxMenuItem* item = *iter;
		menu->Destroy(item);
	}
}

void DropDownButton::OnKillFocus(wxFocusEvent& event)
{
	state = wxCONTROL_NONE;
	Refresh();
}

void DropDownButton::OnMouseLeave(wxMouseEvent& event)
{
	state = wxCONTROL_NONE;	
	Refresh();
}

void DropDownButton::OnMouseEnter(wxMouseEvent& event)
{
	state = wxCONTROL_CURRENT;
	Refresh();
}

void DropDownButton::OnLeftButtonUp(wxMouseEvent& event)
{	
	state = wxCONTROL_NONE;	
	Refresh();

	int x = -1;
	int y = -1;

	event.GetPosition(&x, &y);

	if (x < (GetSize().GetWidth() - arrowButtonWidth))
	{
		wxEvtHandler *eventHandler = GetEventHandler();

		eventHandler->CallAfter([=]()
		{
			wxCommandEvent evt(wxEVT_BUTTON, this->GetId());
			evt.SetEventObject(this);
			GetEventHandler()->ProcessEvent(evt);
		});
	}
}

void DropDownButton::OnLeftButtonDown(wxMouseEvent& event)
{
	int x = -1;
	int y = -1;

	event.GetPosition(&x, &y);

	if (x >= (GetSize().GetWidth() - arrowButtonWidth))
	{
		state = wxCONTROL_NONE;
		IsMenuVisible = true;

		Refresh();

		wxSize size = GetSize();
		wxPoint position;
		position.x = 0;
		position.y = size.GetHeight();

		PopupMenu(menu, position);
	}
	else
	{
		state = wxCONTROL_PRESSED;
		Refresh();
	}
}

void DropDownButton::OnPaint(wxPaintEvent& event)
{
	wxPaintDC dc(this);

	wxSize size = GetSize();
	const int width = size.GetWidth() - arrowButtonWidth;

	dc.SetBackground(wxColor(255, 255, 255));
	dc.Clear();	

	wxRect r1;
	r1.x = 0;
	r1.y = 0;
	r1.width = width + 2;
	r1.height = size.GetHeight();
	
	if (state == wxCONTROL_NONE && !IsMenuVisible) {
		dc.SetPen(wxPen(wxColor(255, 255, 255), 1));
		dc.SetBrush(wxBrush(wxColor(255, 255, 255), wxBRUSHSTYLE_SOLID));		
	}
	else if (state == wxCONTROL_CURRENT) {
		dc.SetPen(wxPen(wxColor(169, 211, 230), 1));
		dc.SetBrush(wxBrush(wxColor(228, 242, 250), wxBRUSHSTYLE_SOLID));
	}
	else if (state == wxCONTROL_PRESSED) {
		dc.SetPen(wxPen(wxColor(169, 211, 230), 1));
		dc.SetBrush(wxBrush(wxColor(202, 231, 245), wxBRUSHSTYLE_SOLID));
	}
	else if (IsMenuVisible) {
		dc.SetPen(wxPen(wxColor(169, 211, 230), 1));
		dc.SetBrush(wxBrush(wxColor(202, 231, 245), wxBRUSHSTYLE_SOLID));
	}
	else {
		dc.SetPen(wxPen(wxColor(169, 211, 230), 1));
		dc.SetBrush(wxBrush(wxColor(202, 231, 245), wxBRUSHSTYLE_SOLID));
	}
	
	dc.DrawRectangle(0, 0, size.GetWidth(), size.GetHeight());	
	dc.DrawBitmap(image, 5, (size.GetHeight() - 16) / 2, true);

	r1.x = 26;
	r1.y += (size.GetHeight() - GetCharHeight()) / 2;	

	dc.DrawLabel(label, r1, wxALIGN_LEFT);

	wxRect r2;
	r2.x = width - 2;
	r2.y = 0;
	r2.width = arrowButtonWidth;
	r2.height = size.GetHeight();

	if (state == wxCONTROL_CURRENT || state == wxCONTROL_PRESSED || IsMenuVisible) {
		dc.SetBrush(wxBrush(wxColor(202, 231, 245), wxBRUSHSTYLE_SOLID));
		dc.DrawRectangle(width - 2, 0, 1, size.GetHeight());
	}

	wxRendererNative::Get().DrawDropArrow(this, dc, r2);
}

bool DropDownButton::Enable(bool enable)
{
	IsEnabled = enable;
	wxPanel::Enable(IsEnabled);

	if (IsEnabled)
	{
		state = wxCONTROL_NONE;
	}
	else
	{
		state = wxCONTROL_DISABLED;
	}


	Refresh();

	return enable;
}

void DropDownButton::OnMenuClose(wxMenuEvent& event) {
	IsMenuVisible = false;
	state = wxCONTROL_NONE;

	Refresh();
}