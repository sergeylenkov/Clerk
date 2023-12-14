#include "NotificationsButton.h"

using namespace Clerk::UI;

NotificationsButton::NotificationsButton(NotificationsViewModel& viewModel, wxWindow* parent, const wxPoint& pos, const wxSize& size):
	ToolbarButton(parent, wxID_ANY, pos, size), 
	_viewModel(viewModel)
{
	_image = wxBitmap(wxT("ICON_BELL"), wxBITMAP_TYPE_PNG_RESOURCE);

	_popup = new NotificationsPopup(this);

	_popup->OnDismissAlert = [&](std::shared_ptr<AlertPresentationModel> alert) {
		_viewModel.DismissAlert(*alert);
	};

	_popup->OnDismissScheduler = [&](std::shared_ptr<SchedulerPresentationModel> scheduler) {
		_viewModel.DismissScheduler(*scheduler);
	};

	_popup->OnExecScheduler = [&](std::shared_ptr<SchedulerPresentationModel> scheduler) {
		_viewModel.ExecScheduler(*scheduler);
	};

	_popup->OnSkipScheduler = [&](std::shared_ptr<SchedulerPresentationModel> scheduler) {
		_viewModel.SkipScheduler(*scheduler);
	};

	_viewModel.OnUpdate([&]() {
		Update();
	});

	Update();

	Bind(wxEVT_PAINT, &NotificationsButton::OnPaint, this);
	Bind(wxEVT_BUTTON, &NotificationsButton::OnClick, this);
}

NotificationsButton::~NotificationsButton() {
	delete& _viewModel;
}

void NotificationsButton::Update()
{
	if (_viewModel.IsActive()) {
		_image = wxBitmap(wxT("ICON_BELL_ACTIVE"), wxBITMAP_TYPE_PNG_RESOURCE);
	} else {
		_image = wxBitmap(wxT("ICON_BELL"), wxBITMAP_TYPE_PNG_RESOURCE);
	}

	if (_popup->IsShown()) {
		if (_viewModel.IsActive()) {
			_popup->Update(_viewModel.GetActiveAlerts(), _viewModel.GetActiveSchedulers());
		} else {
			_popup->Hide();
		}
	}

	Refresh();
}

void NotificationsButton::OnPaint(wxPaintEvent& event)
{
	ToolbarButton::OnPaint(event);

	wxPaintDC dc(this);
	wxSize size = GetSize();
	wxSize imageSize = _image.GetSize();

	int x = (size.GetWidth() - imageSize.GetWidth()) / 2;
	int y = (size.GetHeight() - imageSize.GetHeight()) / 2;

	dc.DrawBitmap(_image, x, y, true);
}

void NotificationsButton::OnClick(wxCommandEvent& event)
{
	if (!_viewModel.IsActive()) {
		return;
	}

	wxPoint position = GetScreenPosition();
	wxSize size = GetSize();
	wxSize panelSize = wxSize(FromDIP(300), -1);

	_popup->Position(wxPoint(position.x + size.GetWidth(), (position.y - panelSize.GetHeight()) + size.GetHeight()), panelSize);
	_popup->Update(_viewModel.GetActiveAlerts(), _viewModel.GetActiveSchedulers());
	_popup->Show();

}