#include "BudgetModel.h"

using namespace Clerk::Data;

BudgetModel::BudgetModel()
{
	this->id = -1;
	this->name = std::wstring();
	this->amount = 0;
	this->period = BudgetPeriod::Month;
	this->date = std::wstring();
	this->created = std::wstring();
	this->accountsIds = std::wstring();
}