#include "ExpensesTooltipPopup.h"

ExpensesTooltipPopup::ExpensesTooltipPopup(wxWindow *parent) : wxPopupWindow(parent) {
	wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

	panel = new ExpensesTooltipPanel(this);
	mainSizer->Add(panel, 1, wxEXPAND, 0);
	
	this->SetSizer(mainSizer);
	this->Layout();
}

ExpensesTooltipPopup::~ExpensesTooltipPopup() {
}

void ExpensesTooltipPopup::Update(wxString title, std::vector<StringValueViewModel> values) {
	float total = 0;

	for (unsigned int i = 0; i < values.size(); i++) {
		total = total + values[i].value;
	}

	std::sort(values.begin(), values.end(), [](StringValueViewModel a, StringValueViewModel b) {
		return a.value > b.value;
	});

	
	panel->SetTotal(total);
	panel->SetTitle(title);
	panel->SetValues(values);
		
	panel->Update();

	SetClientSize(panel->GetMinSize());
}

ExpensesTooltipPanel::ExpensesTooltipPanel(wxWindow *parent) : wxPanel(parent) {
	total = 0;
	this->Bind(wxEVT_PAINT, &ExpensesTooltipPanel::OnPaint, this);
}

void ExpensesTooltipPanel::SetTitle(wxString title) {
	this->title = title;
}

void ExpensesTooltipPanel::SetTotal(float total) {
	this->total = total;
}

void ExpensesTooltipPanel::SetValues(std::vector<StringValueViewModel> values) {
	this->values = values;	
}

void ExpensesTooltipPanel::Update()
{
	int height = 0;
	int width = 0;
	int maxName = title.Length();
	int maxValue = total;

	
	wxFont titleFont = this->GetFont();
	titleFont.SetWeight(wxFONTWEIGHT_BOLD);

	wxClientDC dc(this);
	dc.SetFont(titleFont);

	wxSize nameSize = dc.GetTextExtent(title);
	wxSize valueSize = dc.GetTextExtent(Format::Amount(total));

	width = nameSize.GetWidth() + valueSize.GetWidth() + 20;
	height = height + nameSize.GetHeight() + 10;

	dc.SetFont(this->GetFont());

	for (auto &value : values) {
		nameSize = dc.GetTextExtent(value.string);
		valueSize = dc.GetTextExtent(Format::Amount(value.value));

		int lineWidth = nameSize.GetWidth() + valueSize.GetWidth() + 20;

		if (lineWidth > width) {
			width = lineWidth;
		}

		height = height + nameSize.GetHeight() + 5;
	}

	this->SetMinSize(wxSize(width, height));

	Refresh();
}

void ExpensesTooltipPanel::Draw(wxPaintDC &dc) {
	int width = 0;
	int height = 0;

	this->DoGetSize(&width, &height);

	dc.SetBackground(wxColor(255, 255, 255));
	dc.Clear();

	dc.SetPen(wxPen(wxColor(200, 200, 200)));
	dc.DrawRectangle(0, 0, width, height);

	wxFont accountFont = this->GetFont();
	wxFont amountFont = this->GetFont();

	dc.SetFont(amountFont);
	dc.SetTextForeground(wxColor(120, 120, 120));

	int y = 5;

	wxFont titleFont = this->GetFont();
	titleFont.SetWeight(wxFONTWEIGHT_BOLD);

	dc.SetFont(titleFont);
	dc.SetTextForeground(wxColor(0, 0, 0));
	dc.DrawText(title, wxPoint(5, y));

	wxString amount = Format::Amount(total);
	wxSize size = dc.GetTextExtent(amount);

	dc.SetTextForeground(wxColor(60, 60, 60));
	dc.DrawText(amount, wxPoint(width - size.GetWidth() - 5, y));

	y = size.GetHeight() + 10;

	for (auto &value : values) {
		dc.SetFont(accountFont);
		dc.SetTextForeground(wxColor(0, 0, 0));
		dc.DrawText(value.string, wxPoint(5, y));

		dc.SetFont(amountFont);

		amount = Format::Amount(value.value);
		size = dc.GetTextExtent(amount);

		dc.SetTextForeground(wxColor(60, 60, 60));
		dc.DrawText(amount, wxPoint(width - size.GetWidth() - 5, y));

		y = y + size.GetHeight() + 5;
	}
}

void ExpensesTooltipPanel::OnPaint(wxPaintEvent& event) {
	wxPaintDC dc(this);
	Draw(dc);
}