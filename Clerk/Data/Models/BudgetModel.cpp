#include "BudgetModel.h"

using namespace Clerk::Data;

BudgetModel::BudgetModel()
{
	this->id = -1;
	this->name = std::string("");
	this->amount = 0;
	this->period = BudgetPeriod::Month;
	this->date = std::string("");
	this->created = std::string("");
	this->accountIds = std::string("");
}