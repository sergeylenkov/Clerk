#include "TransactionViewModel.h"

using namespace Clerk::Data;

TransactionViewModel::TransactionViewModel(TransactionModel& transaction) {
	this->id = transaction.id;
	this->fromAccount = nullptr;
	this->toAccount = nullptr;
	this->fromAmount = 0;
	this->toAmount = 0;
	this->note = wxString(transaction.note);
	this->date = wxDateTime().ParseFormat(transaction.date);
	this->tagsString = wxString("");
}