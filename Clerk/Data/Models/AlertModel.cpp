#include "AlertModel.h"

using namespace Clerk::Data;

AlertModel::AlertModel()
{
	this->id = -1;
	this->name = std::string("");
	this->amount = 0;
	this->type = AlertType::Balance;
	this->period = AlertPeriod::Month;
	this->condition = AlertCondition::Equal;
	this->accountIds = std::string("");
	this->created = std::string("");
}