#include "TransactionPresentationModel.h"

using namespace Clerk::UI;

TransactionPresentationModel::TransactionPresentationModel() {
	this->id = -1;
	this->fromAccount = nullptr;
	this->toAccount = nullptr;
	this->fromAmount = 0;
	this->toAmount = 0;
	this->note = wxString("");
	this->date = wxDateTime::Today();
	this->created = wxDateTime::Now();
	this->tagsString = wxString("");
}

TransactionPresentationModel::TransactionPresentationModel(TransactionModel& transaction) {
	this->id = transaction.id;
	this->fromAccount = nullptr;
	this->toAccount = nullptr;
	this->fromAmount = transaction.fromAmount;
	this->toAmount = transaction.toAmount;
	this->note = wxString(transaction.note);
	this->date = wxDateTime::Today(); 
	this->date.ParseISODate(transaction.date);
	this->created = wxDateTime::Now();
	this->created.ParseISODate(transaction.created);
	this->tagsString = wxString("");
}

TransactionPresentationModel::operator TransactionModel& () {
	return *GetModel();
}

TransactionModel* TransactionPresentationModel::GetModel() {
	auto model = new TransactionModel();

	model->id = id;
	model->fromAccountId = fromAccount->id;
	model->toAccountId = toAccount->id;
	model->fromAmount = fromAmount;
	model->toAmount = toAmount;
	model->note = note;
	model->date = date.FormatISODate();
	model->created = created.FormatISOCombined(' ');

	std::transform(tags.begin(), tags.end(), std::back_inserter(model->tagsIds), [&](const std::shared_ptr<TagPresentationModel>& tag) {
		return tag->id;
	});
	

	return model;
}