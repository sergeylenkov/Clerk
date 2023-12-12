#include "TransactionPresentationModel.h"

using namespace Clerk::UI;

TransactionPresentationModel::TransactionPresentationModel() {
	id = -1;
	fromAccount = nullptr;
	toAccount = nullptr;
	fromAmount = 0;
	toAmount = 0;
	note = wxString("");
	date = wxDateTime::Today();
	created = wxDateTime::Now();
	tagsString = wxString("");
}

TransactionPresentationModel::TransactionPresentationModel(TransactionModel& transaction) {
	id = transaction.id;
	fromAccount = nullptr;
	toAccount = nullptr;
	fromAmount = transaction.fromAmount;
	toAmount = transaction.toAmount;
	note = wxString(transaction.note);

	date = wxDateTime::Today(); 
	date.ParseISODate(transaction.date);

	created = wxDateTime::Now();
	created.ParseISODate(transaction.created);

	tagsString = wxString("");
}

TransactionPresentationModel::operator TransactionModel& () {
	return GetModel();
}

TransactionModel& TransactionPresentationModel::GetModel() {
	TransactionModel* model = new TransactionModel();

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

	return *model;
}