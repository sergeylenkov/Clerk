#include "ReportChartTooltipPanel.h"

ReportChartTooltipPanel::ReportChartTooltipPanel(wxWindow *parent) : wxPanel(parent) {
	total = 0;
	this->Bind(wxEVT_PAINT, &ReportChartTooltipPanel::OnPaint, this);
}

void ReportChartTooltipPanel::SetTitle(wxString title) {
	this->title = title;
}

void ReportChartTooltipPanel::SetTotal(float total) {
	this->total = total;
}

void ReportChartTooltipPanel::SetValues(std::vector<StringValueViewModel> values) {
	this->values = values;	
}

void ReportChartTooltipPanel::Update()
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

void ReportChartTooltipPanel::Draw(wxPaintDC &dc) {
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

void ReportChartTooltipPanel::OnPaint(wxPaintEvent& event) {
	wxPaintDC dc(this);
	Draw(dc);
}