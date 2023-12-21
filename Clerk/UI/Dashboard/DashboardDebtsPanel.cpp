#include "DashboardDebtsPanel.h"

DashboardDebtsPanel::DashboardDebtsPanel(wxWindow *parent) : wxPanel(parent) {
	SetDoubleBuffered(true);
	SetBackgroundStyle(wxBG_STYLE_CUSTOM);
	Bind(wxEVT_PAINT, &DashboardDebtsPanel::OnPaint, this);
}

void DashboardDebtsPanel::SetViewModel(DashboardViewModel* viewModel) {
	_viewModel = viewModel;

	_viewModel->OnUpdate([=]() {
		Update();
	});

	Update();
}

void DashboardDebtsPanel::Update()
{
	auto _debts = _viewModel->GetDepts();

	_values.clear();
	_totalValue = 0;

	for (auto& account : _debts) {
		if (account->isCredit) {
			float amount = account->expenses;
			float currentAmount = account->creditLimit + amount;
			float remainPercent = abs(currentAmount / account->creditLimit) * 100.0;

			_values.push_back({ account->name, Format::Amount(account->creditLimit), Format::Amount(currentAmount),  Format::Amount(abs(amount)), remainPercent });

			_totalValue = _totalValue + abs(amount);
		}
		else {
			float amount = account->expenses;
			float currentAmount = account->receipts;
			float remainAmount = amount - currentAmount;
			float remainPercent = (currentAmount / amount) * 100.0;

			_values.push_back({ account->name, Format::Amount(amount), Format::Amount(currentAmount),  Format::Amount(remainAmount), remainPercent });

			_totalValue = _totalValue + remainAmount;
		}
	}

	int height = 170 + (_debts.size() * 30);

	SetMinSize(wxSize(-1, height));
	Refresh();
}

void DashboardDebtsPanel::Draw(wxPaintDC &dc) {
	int width = 0;
	int height = 0;

	DoGetSize(&width, &height);

	dc.SetBackground(wxColor(255, 255, 255));
	dc.Clear();

	wxFont titleFont = GetFont();
	titleFont.SetPointSize(12);	

	dc.SetFont(titleFont);
	dc.DrawText(_("Debts"), wxPoint(0, 0));

	wxFont amountFont = GetFont();

	dc.SetFont(amountFont);
	dc.SetTextForeground(wxColor(120, 120, 120));

	wxString value = wxNumberFormatter::ToString(_totalValue, 2);
	wxSize size = dc.GetTextExtent(value);

	dc.DrawText(value, wxPoint(width - size.GetWidth(), 5));

	wxFont font = GetFont();
	font.SetPointSize(8);

	dc.SetFont(font);

	int columnWidth0 = 0;
	int columnWidth1 = 0;
	int columnWidth2 = 0;

	for (auto &value : _values) {
		wxSize size = dc.GetTextExtent(value.name);

		if (size.GetWidth() > columnWidth0) {
			columnWidth0 = size.GetWidth();
		}

		size = dc.GetTextExtent(value.amount);

		if (size.GetWidth() > columnWidth1) {
			columnWidth1 = size.GetWidth();
		}

		size = dc.GetTextExtent(value.remainAmount);

		if (size.GetWidth() > columnWidth2) {
			columnWidth2 = size.GetWidth();
		}
	}

	int y = 40;

	dc.SetTextForeground(wxColor(151, 151, 151));
	dc.DrawText("0%", wxPoint(columnWidth0 + 20, y));
	dc.DrawText("100%", wxPoint(width - columnWidth1 - columnWidth2 - 65, y));

	size = dc.GetTextExtent(_("Debt"));
	dc.DrawText(_("Debt"), wxPoint(width - columnWidth2 - size.GetWidth() - 20, y));

	size = dc.GetTextExtent(_("Remain"));
	dc.DrawText(_("Remain"), wxPoint(width - size.GetWidth(), y));

	y = y + 20;

	dc.SetPen(wxPen(wxColor(203, 203, 203), 1));
	dc.DrawLine(0, y, width, y);

	int x = 0;
	y = y + 40;

	int progressWidth = width - columnWidth0 - columnWidth1 - columnWidth2 - 60;
	int progressX = columnWidth0 + 20;

	for (auto &value : _values) {
		dc.SetTextForeground(wxColor(0, 0, 0));

		dc.SetFont(font);
		dc.DrawText(value.name, wxPoint(0, y));

		wxSize size = dc.GetTextExtent(value.amount);
		x = width - columnWidth2 - size.GetWidth() - 20;

		dc.DrawText(value.amount, wxPoint(x, y));

		size = dc.GetTextExtent(value.remainAmount);
		dc.DrawText(value.remainAmount, wxPoint(width - size.GetWidth(), y));

		int progressY = y + size.GetHeight() / 2;
		int percentWidth = (progressWidth / 100) * value.percent;

		if (percentWidth > progressWidth) {
			percentWidth = progressWidth;
		}

		dc.SetPen(wxPen(wxColor(216, 216, 216), 1));
		dc.SetBrush(wxBrush(wxColor(216, 216, 216)));

		dc.DrawRectangle(progressX, progressY, progressWidth, 4);

		wxColor color = Colors::ColorForProgress(value.percent, true);

		dc.SetPen(wxPen(color, 1));
		dc.SetBrush(wxBrush(color));

		dc.DrawRectangle(progressX, progressY, percentWidth, 4);

		size = dc.GetTextExtent(value.currentAmount);

		int amountX = percentWidth - (size.GetWidth() / 2);

		if (amountX < 0) {
			amountX = 0;
		}
		else if (amountX + size.GetWidth() > progressWidth) {
			amountX = progressWidth - size.GetWidth();
		}

		dc.DrawText(value.currentAmount, wxPoint(progressX + amountX, y + 14));

		y = y + 40;
	}
}

void DashboardDebtsPanel::OnPaint(wxPaintEvent& event) {
	wxPaintDC dc(this);
	Draw(dc);
}