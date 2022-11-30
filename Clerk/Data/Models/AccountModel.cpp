#include "AccountModel.h"

using namespace Clerk::Data;

AccountModel::AccountModel()
{
	this->id = -1;
	this->name = std::wstring(L"");
	this->note = std::wstring(L"");
	this->type = AccountType::Deposit;
	this->iconId = 0;
	this->orderId = 1000;
	this->currencyId = 152;
	this->creditLimit = 0;
	this->isActive = true;
	this->created = std::wstring(L"");
}