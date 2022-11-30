#include "GoalModel.h"

using namespace Clerk::Data;

GoalModel::GoalModel()
{
	this->id = -1;
	this->name = std::wstring();
	this->amount = 0;
	this->date = std::wstring();
	this->created = std::wstring();
	this->accountIds = std::wstring();
}