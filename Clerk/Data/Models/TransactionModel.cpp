#include "TransactionModel.h"

using namespace Clerk::Data;

TransactionModel::TransactionModel()
{
	this->id = -1;
	this->fromAccountId = -1;
	this->toAccountId = -1;
	this->fromAmount = 0;
	this->toAmount = 0;
	this->note = std::wstring(L"");
	this->date = std::wstring(L"");
	this->created = std::wstring(L"");
}