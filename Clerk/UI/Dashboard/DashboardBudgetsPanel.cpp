#include "DashboardBudgetsPanel.h"

DashboardBudgetsPanel::DashboardBudgetsPanel(wxWindow *parent) : wxPanel(parent) {
	this->SetDoubleBuffered(true);
	this->SetBackgroundStyle(wxBG_STYLE_CUSTOM);
	this->Bind(wxEVT_PAINT, &DashboardBudgetsPanel::OnPaint, this);
}

void DashboardBudgetsPanel::SetViewModel(DashboardViewModel* viewModel) {
	_viewModel = viewModel;

	_viewModel->OnUpdate([=]() {
		Update();
	});

	Update();
}

void DashboardBudgetsPanel::Update()
{
	_budgets = _viewModel->GetBudgets();

	wxDateTime fromDate = wxDateTime::Now();
	wxDateTime toDate = wxDateTime::Now();

	fromDate.SetDay(1);
	toDate.SetToLastMonthDay();

	_daysCount = toDate.GetDay();
	_currentDay = wxDateTime::Now().GetDay();

	int height = 170 + (_viewModel->GetBudgets().size() * 40);
	this->SetMinSize(wxSize(-1, height));

	Refresh();
}

void DashboardBudgetsPanel::Draw(wxPaintDC &dc) {
	int width = 0;
	int height = 0;

	this->DoGetSize(&width, &height);

	dc.SetBackground(wxColor(255, 255, 255));
	dc.Clear();

	wxFont titleFont = this->GetFont();
	titleFont.SetPointSize(12);

	dc.SetFont(titleFont);
	dc.DrawText("Budgets", wxPoint(0, 0));

	wxFont font = this->GetFont();
	font.SetPointSize(8);

	dc.SetFont(font);

	int columnWidth0 = 0;
	int columnWidth1 = 0;
	int columnWidth2 = 0;
	
	for (auto& budget : _budgets) {
		float remainAmount = budget->amount - budget->balance;

		wxSize size = dc.GetTextExtent(budget->name);

		if (size.GetWidth() > columnWidth0) {
			columnWidth0 = size.GetWidth();
		}

		size = dc.GetTextExtent(wxNumberFormatter::ToString(budget->amount, 2));

		if (size.GetWidth() > columnWidth1) {
			columnWidth1 = size.GetWidth();
		}

		size = dc.GetTextExtent(wxNumberFormatter::ToString(remainAmount, 2));

		if (size.GetWidth() > columnWidth2) {
			columnWidth2 = size.GetWidth();
		}
	}

	int y = 40;
	
	dc.SetTextForeground(wxColor(151, 151, 151));
	dc.DrawText("0%", wxPoint(columnWidth0 + 20, y));
	dc.DrawText("100%", wxPoint(width - columnWidth1 - columnWidth2 - 65, y));

	wxSize size = dc.GetTextExtent("Budget");
	dc.DrawText("Budget", wxPoint(width - columnWidth2 - size.GetWidth() - 20, y));

	size = dc.GetTextExtent("Remain");
	dc.DrawText("Remain", wxPoint(width - size.GetWidth(), y));

	y = y + 20;

	dc.SetPen(wxPen(wxColor(203, 203, 203), 1));
	dc.DrawLine(0, y, width, y);

	int x = 0;
	y = y + 40;

	int progressWidth = width - columnWidth0 - columnWidth1 - columnWidth2 - 60;
	int progressX = columnWidth0 + 20;

	for (auto &budget : _budgets) {
		float remainPercent = (budget->balance / budget->amount) * 100.0;
		float remainAmount = budget->amount - budget->balance;

		wxString amount = wxNumberFormatter::ToString(budget->amount, 2);
		wxString balance = wxNumberFormatter::ToString(budget->balance, 2);
		wxString remain = wxNumberFormatter::ToString(remainAmount, 2);

		dc.SetTextForeground(wxColor(0, 0, 0));

		dc.SetFont(font);
		dc.DrawText(budget->name, wxPoint(0, y));

		wxSize size = dc.GetTextExtent(amount);
		x = width - columnWidth2 - size.GetWidth() - 20;

		dc.DrawText(amount, wxPoint(x, y));

		size = dc.GetTextExtent(remain);
		dc.DrawText(remain, wxPoint(width - size.GetWidth(), y));

		int progressY = y + size.GetHeight() / 2;
		int percentWidth = (progressWidth / 100.0) * remainPercent;

		if (percentWidth > progressWidth) {
			percentWidth = progressWidth;
		}

		dc.SetPen(wxPen(wxColor(216, 216, 216), 1));
		dc.SetBrush(wxBrush(wxColor(216, 216, 216)));

		dc.DrawRectangle(progressX, progressY, progressWidth, 4);

		wxColor color = Colors::ColorForBudget(remainPercent);

		dc.SetPen(wxPen(color, 1));
		dc.SetBrush(wxBrush(color));

		dc.DrawRectangle(progressX, progressY, percentWidth, 4);

		size = dc.GetTextExtent(balance);

		int amountX = percentWidth - (size.GetWidth() / 2);

		if (amountX < 0) {
			amountX = 0;
		}
		else if (amountX + size.GetWidth() > progressWidth) {
			amountX = progressWidth - size.GetWidth();
		}

		dc.DrawText(balance, wxPoint(progressX + amountX, y + 14));

		y = y + 40;
	}

	float dayStep = (float)progressWidth / (_daysCount - 1);
	x = dayStep * (_currentDay - 1);

	dc.SetPen(wxPen(wxColor(204, 204, 204), 1));
	dc.DrawLine(progressX + x, 100, progressX + x, y);

	wxString monthStr = wxDateTime::Now().Format("%b %d").Upper();
	size = dc.GetTextExtent(monthStr);

	int rectWidth = size.GetWidth() + 10;
	int rectHeight = size.GetHeight() + 5;

	dc.SetPen(wxPen(wxColor(153, 153, 153), 1));
	dc.SetBrush(wxBrush(wxColor(153, 153, 153)));
	dc.DrawRectangle((progressX + x) - (rectWidth / 2), y, rectWidth, rectHeight);
	
	dc.SetTextForeground(wxColor(255, 255, 255));
	dc.DrawText(monthStr, wxPoint((progressX + x) - (size.GetWidth() / 2), y + 2));
}

void DashboardBudgetsPanel::OnPaint(wxPaintEvent& event) {
	wxPaintDC dc(this);
	Draw(dc);
}