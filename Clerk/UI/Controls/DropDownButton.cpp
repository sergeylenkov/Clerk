#include "DropDownButton.h"

DropDownButton::DropDownButton(wxWindow *parent, const wxBitmap& bitmap, const wxString& label, const wxPoint& pos, const wxSize& size) :
	ToolbarButton(parent, wxID_ANY, pos, size)
{
	_arrowButtonWidth = FromDIP(16);
	_menu = new wxMenu();
	_label = label;
	_image = bitmap;

	wxSize defaultSize = wxButton::GetDefaultSize();

	wxSize textSize = GetTextExtent(label);
	wxSize imageSize = _image.GetSize();

	int width = FromDIP(15) + imageSize.GetWidth() + textSize.GetWidth() + _arrowButtonWidth;

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
	Bind(wxEVT_MENU_CLOSE, &DropDownButton::OnMenuClose, this);
}

DropDownButton::~DropDownButton()
{
	delete _menu;
}

wxMenu* DropDownButton::GetMenu()
{
	return _menu;
}

void DropDownButton::ClearMenu() {
	wxMenuItemList items = _menu->GetMenuItems();

	wxMenuItemList::iterator iter;

	for (iter = items.begin(); iter != items.end(); ++iter)
	{
		wxMenuItem* item = *iter;
		_menu->Destroy(item);
	}
}

void DropDownButton::OnLeftButtonUp(wxMouseEvent& event)
{	
	_state = wxCONTROL_NONE;	
	Refresh();

	int x = -1;
	int y = -1;

	event.GetPosition(&x, &y);

	if (x < (GetSize().GetWidth() - _arrowButtonWidth))
	{
		ProcessClick();
	}
}

void DropDownButton::OnLeftButtonDown(wxMouseEvent& event)
{
	int x = -1;
	int y = -1;

	event.GetPosition(&x, &y);

	if (x >= (GetSize().GetWidth() - _arrowButtonWidth))
	{
		_state = wxCONTROL_NONE;
		_isMenuVisible = true;

		Refresh();

		wxSize size = GetSize();
		wxPoint position;
		position.x = 0;
		position.y = size.GetHeight();

		PopupMenu(_menu, position);
	}
	else
	{
		ToolbarButton::OnLeftButtonDown(event);
	}
}

void DropDownButton::OnPaint(wxPaintEvent& event)
{
	ToolbarButton::OnPaint(event);

	wxPaintDC dc(this);

	wxSize size = GetSize();
	
	if (_state == wxCONTROL_NONE && !_isMenuVisible) {
		dc.SetPen(wxPen(wxColor(255, 255, 255), 1));
		dc.SetBrush(wxBrush(wxColor(255, 255, 255), wxBRUSHSTYLE_SOLID));

		dc.DrawRectangle(0, 0, size.GetWidth(), size.GetHeight());
	} else if (_isMenuVisible) {
		dc.SetPen(wxPen(wxColor(169, 211, 230), 1));
		dc.SetBrush(wxBrush(wxColor(202, 231, 245), wxBRUSHSTYLE_SOLID));

		dc.DrawRectangle(0, 0, size.GetWidth(), size.GetHeight());
	}	
	
	wxSize imageSize = _image.GetSize();

	int x = (size.GetWidth() - imageSize.GetWidth()) / 2;
	int y = (size.GetHeight() - imageSize.GetHeight()) / 2;

	dc.DrawBitmap(_image, FromDIP(5), y, true);

	wxSize textSize = GetTextExtent(_label);

	x = imageSize.GetWidth() + FromDIP(10);
	y = (size.GetHeight() - textSize.GetHeight()) / 2;

	dc.DrawText(_label, x, y);

	x = x + textSize.GetWidth() + FromDIP(5);	

	if (_state == wxCONTROL_CURRENT || _state == wxCONTROL_PRESSED || _isMenuVisible) {
		dc.SetPen(wxPen(wxColor(169, 211, 230), 1));
		dc.DrawRectangle(x, 0, 1, size.GetHeight());
	}

	wxRect arrowRect;
	arrowRect.x = x;
	arrowRect.y = 0;
	arrowRect.width = _arrowButtonWidth;
	arrowRect.height = size.GetHeight();

	wxRendererNative::Get().DrawDropArrow(this, dc, arrowRect);
}

void DropDownButton::OnMenuClose(wxMenuEvent& event) {
	_isMenuVisible = false;
	_state = wxCONTROL_NONE;

	Refresh();
}