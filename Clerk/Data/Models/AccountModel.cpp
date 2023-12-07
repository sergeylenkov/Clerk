#include "AccountModel.h"

using namespace Clerk::Data;

AccountModel::AccountModel()
{
	id = -1;
	name = std::wstring();
	note = std::wstring();
	type = AccountType::Deposit;
	iconId = 0;
	orderId = 1000;
	currencyId = 152;
	creditLimit = 0;
	isActive = true;
	created = std::wstring();
}