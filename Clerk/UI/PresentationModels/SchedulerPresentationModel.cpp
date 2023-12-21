#include "SchedulerPresentationModel.h"

using namespace Clerk::UI;

SchedulerPresentationModel::SchedulerPresentationModel() {
	id = -1;
	name = wxString("");
	type = SchedulerType::Monthly;
	day = 1;
	month = 1;
	week = 1;
	previousDate = wxDateTime::Today();
	nextDate = wxDateTime::Today();
	isActive = false;
	fromAccount = nullptr;
	toAccount = nullptr;
	fromAmount = 0;
	toAmount = 0;	
	created = wxDateTime::Now();
}

SchedulerPresentationModel::SchedulerPresentationModel(SchedulerModel& scheduler) {
	id = scheduler.id;
	name = scheduler.name;
	type = scheduler.type;
	day = scheduler.day;
	month = scheduler.month;
	week = scheduler.week;

	previousDate = wxDateTime::Today();
	previousDate.ParseISODate(scheduler.previousDate);

	nextDate = wxDateTime::Today();
	nextDate.ParseISODate(scheduler.nextDate);

	isActive = scheduler.active;
	fromAccount = nullptr;
	toAccount = nullptr;
	fromAmount = scheduler.fromAmount;
	toAmount = scheduler.toAmount;	

	created = wxDateTime::Now();
	created.ParseISODate(scheduler.created);

	typeName = wxString("");

	switch (scheduler.type)
	{
	case SchedulerType::Daily:
		typeName = _("Daily");
		break;
	case SchedulerType::Weekly:
		typeName = _("Weekly");
		break;
	case SchedulerType::Monthly:
		typeName = _("Monthly");
		break;
	case SchedulerType::Yearly:
		typeName = _("Yearly");
		break;
	}
}

SchedulerPresentationModel::operator SchedulerModel& () {
	return GetModel();
}

SchedulerModel& SchedulerPresentationModel::GetModel() {
	SchedulerModel* model = new SchedulerModel();

	model->id = id;
	model->name = name;
	model->type = type;
	model->day = day;
	model->month = month;
	model->week = week;
	model->active = isActive;
	model->toAccountId = toAccount->id;
	model->fromAccountId = fromAccount->id;
	model->fromAmount = fromAmount;
	model->toAmount = toAmount;
	model->previousDate = previousDate.FormatISODate();
	model->nextDate = nextDate.FormatISODate();
	model->created = created.FormatISOCombined(' ');

	std::vector<std::string> res;

	for (auto& tag : tags) {
		res.push_back(std::to_string(tag->id));
	}

	std::string ids = String::Join(res, ",");

	model->tagsIds = std::wstring(ids.begin(), ids.end());

	return *model;
}