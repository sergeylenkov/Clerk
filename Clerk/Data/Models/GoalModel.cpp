#include "GoalModel.h"

using namespace Clerk::Data;

GoalModel::GoalModel()
{
	this->id = -1;
	this->name = std::string("");
	this->amount = 0;
	this->date = std::string("");
	this->created = std::string("");
	this->accountIds = std::string("");
}