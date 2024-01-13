#include "SchedulerModel.h"

SchedulerModel::SchedulerModel()
{
	id = -1;
	name = std::wstring();	
	type = SchedulerType::Daily;
	day = 1;
	week = 1;
	month = 1;
	fromAccountId = -1;
	toAccountId = -1;	
	fromAmount = 0;
	toAmount = 0;
	tagsIds = std::wstring();
	previousDate = std::wstring();
	nextDate = std::wstring();
	active = false;
	created = std::wstring();
}