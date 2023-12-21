#include "GoalPresentationModel.h"

using namespace Clerk::UI;

GoalPresentationModel::GoalPresentationModel() {
	id = -1;
	name = wxString("");
	amount = 0;	
	date = wxDateTime::Today();	
	balance = 0;
	remainAmount = 0;
	remainPercent = 0;
	created = wxDateTime::Now();
}

GoalPresentationModel::GoalPresentationModel(GoalModel& model) {
	id = model.id;
	name = model.name;
	amount = model.amount;
	date = wxDateTime::Today().ParseISODate(model.date);
	balance = 0;
	remainAmount = 0;
	remainPercent = 0;
	accountsIds = String::Split(model.accountIds, ',');
	created = wxDateTime::Now().ParseISODate(model.created);
}