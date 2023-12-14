#include "NotificationsPopup.h"

NotificationsPopup::NotificationsPopup(wxWindow* parent) : wxPopupTransientWindow(parent, wxBORDER_SIMPLE) {
	_panel = new wxScrolledWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxVSCROLL);
	_panel->SetBackgroundColour(*wxWHITE);

	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

	mainSizer->Add(_panel, 0, wxEXPAND, 0);

	_notificationsSizer = new wxBoxSizer(wxVERTICAL);

	_panel->SetSizer(_notificationsSizer);
	_panel->Layout();

	SetSizer(mainSizer);
	Layout();
}

void NotificationsPopup::Position(wxPoint position, wxSize size) {
	wxPopupWindow::Position(position, size);

	SetSize(size);
	Layout();
}

void NotificationsPopup::Update(shared_vector<AlertPresentationModel> alerts, shared_vector<SchedulerPresentationModel> schedulers) {
	_notificationsSizer->Clear(true);

	std::sort(alerts.begin(), alerts.end(), [](std::shared_ptr<AlertPresentationModel> a, std::shared_ptr<AlertPresentationModel> b) {
		return a->importance > b->importance;
	});

	std::sort(schedulers.begin(), schedulers.end(), [](std::shared_ptr<SchedulerPresentationModel> a, std::shared_ptr<SchedulerPresentationModel> b) {
		return a->nextDate.IsEarlierThan(b->nextDate);
	});

	wxSize size = wxSize(GetSize().GetWidth(), -1);

	for (auto& scheduler : schedulers) {
		NotificationSchedulerPanel* schedulerPanel = new NotificationSchedulerPanel(_panel, wxDefaultPosition, size);
		schedulerPanel->SetScheduler(scheduler);

		schedulerPanel->OnDismiss = [&](std::shared_ptr<SchedulerPresentationModel> scheduler) {
			if (OnDismissScheduler) {
				OnDismissScheduler(scheduler);
			}
		};

		schedulerPanel->OnExec = [&](std::shared_ptr<SchedulerPresentationModel> scheduler) {
			if (OnExecScheduler) {
				OnExecScheduler(scheduler);
			}
		};

		schedulerPanel->OnSkip = [&](std::shared_ptr<SchedulerPresentationModel> scheduler) {
			if (OnSkipScheduler) {
				OnSkipScheduler(scheduler);
			}
		};

		_notificationsSizer->Add(schedulerPanel);
	}

	for (auto& alert : alerts) {
		NotificationAlertPanel* alertPanel = new NotificationAlertPanel(_panel, wxDefaultPosition, size);
		alertPanel->SetAlert(alert);

		alertPanel->OnDismiss = [&](std::shared_ptr<AlertPresentationModel> alert) {
			if (OnDismissAlert) {
				OnDismissAlert(alert);
			}
		};

		_notificationsSizer->Add(alertPanel);
	}

	_notificationsSizer->Layout();
	wxSize popupSize = _notificationsSizer->GetSize();

	if (popupSize.GetHeight() > FromDIP(300)) {
		popupSize.SetHeight(FromDIP(300));
	}

	SetSize(popupSize);

	Layout();
}