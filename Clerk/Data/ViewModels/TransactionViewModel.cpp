#include "TransactionViewModel.h"

using namespace Clerk::Data;

Clerk::Data::TransactionViewModel::TransactionViewModel()
{
}

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

TransactionViewModel::operator TransactionModel& () {
	return *GetModel();
}

TransactionModel* TransactionViewModel::GetModel() {
	auto model = new TransactionModel();

	model->id = id;
	model->fromAccountId = fromAccount->id;
	model->toAccountId = fromAccount->id;
	model->note = note;
	model->date = date.FormatISODate();

	std::transform(tags.begin(), tags.end(), std::back_inserter(model->tagsIds), [this](const std::shared_ptr<TagViewModel>& tag) {
		return tag->id;
	});
	

	return model;
}