#include "SchedulersPanel.h"

SchedulersPanel::SchedulersPanel(wxWindow *parent, wxWindowID id) : DataPanel(parent, id) {
	list = new wxListCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxBORDER_NONE);
	list->Bind(wxEVT_LIST_ITEM_RIGHT_CLICK, &SchedulersPanel::OnListItemClick, this);
	list->Bind(wxEVT_LIST_ITEM_ACTIVATED, &SchedulersPanel::OnListItemDoubleClick, this);

	wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

	mainSizer->Add(list, 1, wxALL | wxEXPAND, 0);

	this->SetSizer(mainSizer);
	this->Layout();
}

shared_ptr<Scheduler> SchedulersPanel::GetScheduler() {
	long itemIndex = -1;

	for (;;) {
		itemIndex = list->GetNextItem(itemIndex, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);

		if (itemIndex == -1) {
			break;
		}

		return schedulers[itemIndex];
	}

	return NULL;
}

void SchedulersPanel::Update() {
	list->ClearAll();
	schedulers = DataHelper::GetInstance().GetSchedulers();

	wxListItem column;

	column.SetId(0);
	column.SetText(_("Name"));
	column.SetWidth(200);

	list->InsertColumn(0, column);

	/*wxListItem column1;

	column1.SetId(1);
	column1.SetText(_("Current"));
	column1.SetWidth(200);

	budgetsList->InsertColumn(1, column1);

	wxListItem column2;

	column2.SetId(2);
	column2.SetText(_("Limit"));
	column2.SetWidth(200);

	budgetsList->InsertColumn(2, column2);

	wxListItem column3;

	column3.SetId(3);
	column3.SetText(_("Remain"));
	column3.SetWidth(200);

	budgetsList->InsertColumn(3, column3);

	int i = 0;

	for each (auto budget in budgets)
	{
		float currentAmount = 0.0;
		wxDateTime toDate = wxDateTime::Now();
		wxDateTime fromDate = wxDateTime::Now();

		if (budget->period == BudgetPeriods::Month) {
			fromDate.SetDay(1);
		}

		if (budget->type == BudgetTypes::Limit) {
			if (!budget->account) {
				currentAmount = DataHelper::GetInstance().GetExpenses(&fromDate, &toDate);
			}
			else {
				currentAmount = DataHelper::GetInstance().GetExpensesSumForAccount(budget->account.get(), &fromDate, &toDate);
			}
		}

		float remainAmount = budget->amount - currentAmount;
		float remainPercent = currentAmount / (budget->amount / 100.0);

		wxListItem listItem;

		listItem.SetId(i);
		listItem.SetData(budget->id);

		if (remainAmount < 0 || remainPercent < 10) {
			listItem.SetMask(wxLIST_MASK_TEXT);
			listItem.SetTextColour(wxColour(255, 0, 0));
		}

		budgetsList->InsertItem(listItem);
		budgetsList->SetItem(i, 0, *budget->name);
		budgetsList->SetItem(i, 1, wxString::Format("%.2f", currentAmount));
		budgetsList->SetItem(i, 2, wxString::Format("%.2f", budget->amount));
		budgetsList->SetItem(i, 3, wxString::Format("%.2f", budget->amount - currentAmount));

		i++;
	}*/
}

void SchedulersPanel::EditScheduler() {
	if (OnEditScheduler) {
		auto scheduler = GetScheduler();

		OnEditScheduler(scheduler);
	}
}

void SchedulersPanel::DeleteScheduler() {
	auto scheduler = GetScheduler();

	if (scheduler) {
		scheduler->Delete();
		Update();
	}
}

void SchedulersPanel::OnListItemClick(wxListEvent &event) {
	wxMenu *menu = new wxMenu;

	menu->Append(static_cast<int>(SchedulersPanelMenuTypes::Edit), wxT("Edit..."));
	menu->AppendSeparator();
	menu->Append(static_cast<int>(SchedulersPanelMenuTypes::Delete), wxT("Delete..."));

	void *data = reinterpret_cast<void *>(event.GetItem().GetData());
	menu->SetClientData(data);

	menu->Bind(wxEVT_COMMAND_MENU_SELECTED, &SchedulersPanel::OnMenuSelect, this);

	list->PopupMenu(menu, event.GetPoint());

	delete menu;
}

void SchedulersPanel::OnListItemDoubleClick(wxListEvent &event) {
	if (OnEditScheduler) {
		auto budget = GetScheduler();
		OnEditScheduler(budget);
	}
}

void SchedulersPanel::OnMenuSelect(wxCommandEvent &event) {
	switch (static_cast<SchedulersPanelMenuTypes>(event.GetId())) {
	case SchedulersPanelMenuTypes::Edit:
		EditScheduler();
		break;

	case SchedulersPanelMenuTypes::Delete:
		DeleteScheduler();
		break;

	default:
		break;
	}
}