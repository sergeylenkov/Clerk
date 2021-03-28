#include "SchedulerModel.h"

SchedulerModel::SchedulerModel()
{
	this->id = -1;
	this->name = std::string("");	
	this->type = SchedulerType::Daily;
	this->day = 1;
	this->week = 1;
	this->month = 1;
	this->fromAmount = 0;
	this->toAmount = 0;
	this->tags = std::string("");
	this->previousDate = std::string("");
	this->nextDate = std::string("");
	this->active = false;
}