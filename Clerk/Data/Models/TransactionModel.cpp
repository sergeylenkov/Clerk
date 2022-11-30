#include "TransactionModel.h"

using namespace Clerk::Data;

TransactionModel::TransactionModel()
{
	this->id = -1;
	this->fromAccountId = -1;
	this->toAccountId = -1;
	this->fromAmount = 0;
	this->toAmount = 0;
	this->note = std::wstring();
	this->date = std::wstring();
	this->created = std::wstring();
}