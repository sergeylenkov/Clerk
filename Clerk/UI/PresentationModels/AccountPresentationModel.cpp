#include "AccountPresentationModel.h"

using namespace Clerk::UI;

AccountPresentationModel::AccountPresentationModel() {
	id = -1;
	name = wxString("");
	note = wxString("");
	type = AccountType::Deposit;
	icon = -1;
	order = 1000;
	creditLimit = 0;
	isCredit = false;
	balance = 0;
	expenses = 0;
	receipts = 0;
	isActive = true;
	date = wxDateTime::Today();
}

AccountPresentationModel::AccountPresentationModel(AccountModel& account) {
	id = account.id;
	name = wxString(account.name);
	note = wxString(account.note);
	type = account.type;
	icon = account.iconId;
	order = account.orderId;
	creditLimit = account.creditLimit;
	isCredit = account.creditLimit > 0;
	balance = 0;
	expenses = 0;
	receipts = 0;
	isActive = account.isActive;
	date = wxDateTime::Today();
	date.ParseISODate(account.created);
}

AccountPresentationModel::operator AccountModel &() {
	return GetModel();
}

AccountModel& AccountPresentationModel::GetModel() {
	AccountModel* model = new AccountModel();

	model->id = id;
	model->name = name;
	model->note = note;
	model->type = type;
	model->iconId = icon;
	model->orderId = order;	
	model->creditLimit = creditLimit;
	model->isActive = isActive;
	model->currencyId = currency->id;
	model->created = date.FormatISODate();

	return *model;
}