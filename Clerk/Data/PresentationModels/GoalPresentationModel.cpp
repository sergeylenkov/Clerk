#include "GoalPresentationModel.h"

using namespace Clerk::Data;

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

	date = wxDateTime::Today();
	date.ParseISODate(model.date);

	balance = 0;
	remainAmount = 0;
	remainPercent = 0;
	accountsIds = String::Split(model.accountIds, ',');

	created = wxDateTime::Now();
	created.ParseISODate(model.created);
}

GoalPresentationModel::operator GoalModel& () {
	return GetModel();
}

GoalModel& GoalPresentationModel::GetModel() {
	GoalModel* model = new GoalModel();

	model->id = id;
	model->name = name;
	model->amount = amount;
	model->date = date.FormatISODate();
	model->created = created.FormatISOCombined(' ');

	std::string ids = String::Join(accountsIds, ",");

	model->accountIds = std::wstring(ids.begin(), ids.end());

	return *model;
}