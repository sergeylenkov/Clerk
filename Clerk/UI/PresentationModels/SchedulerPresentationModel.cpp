#include "SchedulerPresentationModel.h"

using namespace Clerk::UI;

SchedulerPresentationModel::SchedulerPresentationModel(SchedulerModel& scheduler) {
	this->id = scheduler.id;
	this->name = wxString::FromUTF8(scheduler.name);
	this->type = scheduler.type;
	this->day = scheduler.day;
	this->month = scheduler.month;
	this->week = scheduler.week;
	this->previousDate = wxDateTime();
	this->nextDate = wxDateTime();
	this->isActive = scheduler.active;
	this->fromAmount = scheduler.fromAmount;
	this->toAmount = scheduler.toAmount;
	this->amount = 0;

	this->previousDate.ParseISODate(scheduler.previousDate);
	this->nextDate.ParseISODate(scheduler.nextDate);

	switch (scheduler.type)
	{
	case SchedulerType::Daily:
		this->typeName = wxString("Daily");
		break;
	case SchedulerType::Weekly:
		this->typeName = wxString("Weekly");
		break;
	case SchedulerType::Monthly:
		this->typeName = wxString("Monthly");
		break;
	case SchedulerType::Yearly:
		this->typeName = wxString("Yearly");
		break;
	default:
		this->typeName = wxString("");
		break;

	}
}