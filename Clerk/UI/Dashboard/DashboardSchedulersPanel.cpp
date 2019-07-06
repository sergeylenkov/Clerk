#include "DashboardSchedulersPanel.h"

DashboardSchedulersPanel::DashboardSchedulersPanel(wxWindow *parent, wxWindowID id) : wxPanel(parent, id) {
	this->Bind(wxEVT_PAINT, &DashboardSchedulersPanel::OnPaint, this);
}

void DashboardSchedulersPanel::SetSchedulers(std::vector<std::shared_ptr<Scheduler>> schedulers) {
	this->schedulers = schedulers;
	Update();
}

void DashboardSchedulersPanel::Update()
{
	values.clear();
	maxValue = 0;

	for (auto scheduler : schedulers) {
		wxString date = scheduler->nextDate->FormatISODate();

		if (values[date]) {
			values[date] = values[date] + scheduler->toAmount;
		}
		else {
			values[date] = scheduler->toAmount;
		}

		if (values[date] > maxValue) {
			maxValue = values[date];
		}
	}

	int height = 200 + (schedulers.size() * 40);
	this->SetMinSize(wxSize(-1, height));

	Refresh();
}

void DashboardSchedulersPanel::Draw(wxPaintDC &dc) {
	dc.SetBackground(wxColor(255, 255, 255));
	dc.Clear();

	wxFont titleFont = this->GetFont();
	titleFont.SetPointSize(12);

	dc.SetFont(titleFont);
	dc.DrawText("Schedulers", wxPoint(0, 0));

	DrawCalendar(dc);
	DrawTable(dc);
}

void DashboardSchedulersPanel::DrawCalendar(wxPaintDC &dc) {
	int width = 0;
	int height = 0;

	this->DoGetSize(&width, &height);

	wxFont dayFont = this->GetFont();
	dayFont.SetPointSize(8);

	wxFont todayFont = this->GetFont();
	todayFont.SetPointSize(8);
	todayFont.SetWeight(wxFONTWEIGHT_BOLD);

	int x = 0;
	int y = 20;

	int dayWidth = width / 30;
	int lineWidth = dayWidth * 30;
	float amountStep = 50 / maxValue;
	int lineY = y + 80;

	dc.SetPen(wxPen(wxColor(213, 213, 213), 1));
	dc.DrawLine(0, lineY, lineWidth, lineY);

	wxDateTime day = wxDateTime::Now();
	int month = 0;

	for (int i = 0; i < 30; i++) {
		dc.SetTextForeground(wxColor(151, 151, 151));
		dc.SetFont(dayFont);

		if (i == 0) {
			dc.SetTextForeground(wxColor(10, 199, 117));
			dc.SetFont(todayFont);
		}
		else if (day.GetWeekDay() == 6 || day.GetWeekDay() == 0) {
			dc.SetTextForeground(wxColor(221, 221, 221));
		}

		wxString dayStr = wxString::Format("%d", day.GetDay());
		wxString monthStr = day.Format("%b").Upper();

		x = i * dayWidth;

		wxSize size = dc.GetTextExtent(dayStr);
		int offset = (dayWidth - size.GetWidth()) / 2;
		int tickX = x + (dayWidth / 2);

		dc.SetPen(wxPen(wxColor(151, 151, 151), 1));
		dc.DrawLine(tickX, lineY + 1, tickX, lineY + 3);
		dc.DrawText(dayStr, wxPoint(x + offset, lineY + 5));

		if (month != day.GetMonth()) {
			month = day.GetMonth();

			dc.SetTextForeground(wxColor(151, 151, 151));
			dc.SetFont(dayFont);
			dc.DrawText(monthStr, wxPoint(x + offset, lineY + 20));
		}

		if (values[day.FormatISODate()]) {
			int height = values[day.FormatISODate()] * amountStep;

			if (height < 10) {
				height = 10;
			}

			dc.SetPen(wxPen(wxColor(10, 199, 117), 3));
			dc.DrawLine(tickX, lineY, tickX, lineY - height);
		}

		day.Add(wxDateSpan(0, 0, 0, 1));
	}
}

void DashboardSchedulersPanel::DrawTable(wxPaintDC &dc) {
	int width = 0;
	int height = 0;

	this->DoGetSize(&width, &height);

	int dayWidth = width / 30;
	int lineWidth = dayWidth * 30;
	int x = 0;
	int y = 170;
	int padding = 10;

	wxFont font = this->GetFont();
	font.SetPointSize(8);

	int firstColumnWidth = 0;

	for (auto scheduler : schedulers) {
		wxString date = scheduler->nextDate->Format("%a, %b %d");
		wxSize size = dc.GetTextExtent(date);

		if (size.GetWidth() > firstColumnWidth) {
			firstColumnWidth = size.GetWidth();
		}
	}

	for (auto scheduler : schedulers) {
		dc.SetPen(wxPen(wxColor(203, 203, 203), 1));
		dc.DrawLine(0, y, lineWidth, y);

		dc.DrawLine(firstColumnWidth + 10, y + 4, firstColumnWidth + 10, y + 30);

		y = y + padding;

		wxString date = scheduler->nextDate->Format("%a, %b %d");
		wxString amount = wxNumberFormatter::ToString(scheduler->toAmount, 2);

		dc.SetTextForeground(wxColor(0, 0, 0));
		dc.SetFont(font);
		dc.DrawText(date, wxPoint(0, y));

		dc.DrawText(*scheduler->name, wxPoint(firstColumnWidth + 30, y));

		wxSize size = dc.GetTextExtent(amount);
		dc.DrawText(amount, wxPoint(lineWidth - size.GetWidth(), y));

		y = y + size.GetHeight() + padding;
	}
}

void DashboardSchedulersPanel::OnPaint(wxPaintEvent& event) {
	wxPaintDC dc(this);
	Draw(dc);
}