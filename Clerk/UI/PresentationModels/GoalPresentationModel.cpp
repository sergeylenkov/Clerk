#include "GoalPresentationModel.h"

using namespace Clerk::UI;

GoalPresentationModel::GoalPresentationModel(GoalModel& goal) {
	this->id = goal.id;
	this->name = wxString::FromUTF8(goal.name.c_str());
	this->amount = goal.amount;
	this->created = wxDateTime::Today();	
	this->date.ParseISODate(goal.date);

	std::stringstream ss(goal.accountIds);

	int i;

	while (ss >> i)
	{
		this->accountIds.push_back(i);

		if (ss.peek() == ',') {
			ss.ignore();
		}
	}
}