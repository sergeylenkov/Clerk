#include "AccountViewModel.h"

using namespace Clerk::Data;

AccountViewModel::AccountViewModel() {
	this->id = -1;
	this->name = wxString("");
	this->note = wxString("");
	this->type = AccountType::Deposit;
	this->icon = -1;
	this->order = 0;
	this->creditLimit = 0;
	this->isCredit = false;
	this->balance = 0;
	this->expenses = 0;
	this->receipts = 0;
	this->isActive = false;
	this->date = wxDateTime::Today();
}

AccountViewModel::AccountViewModel(AccountModel& account) {
	this->id = account.id;
	this->name = wxString::FromUTF8(account.name.c_str());
	this->note = wxString::FromUTF8(account.note.c_str());
	this->type = account.type;
	this->icon = account.iconId;
	this->order = account.orderId;
	this->creditLimit = account.creditLimit;
	this->isCredit = account.creditLimit > 0;
	this->balance = 0;
	this->expenses = 0;
	this->receipts = 0;
	this->isActive = account.isActive;
	this->date = wxDateTime::Today();
	this->date.ParseISODate(account.created);
}

AccountViewModel::operator AccountModel& () {
	return *GetModel();
}

AccountModel* AccountViewModel::GetModel() {
	auto model = new AccountModel();

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

	return model;
}