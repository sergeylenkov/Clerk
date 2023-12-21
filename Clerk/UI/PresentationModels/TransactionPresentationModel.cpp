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

TransactionPresentationModel::TransactionPresentationModel(TransactionModel& model) {
	id = model.id;
	fromAccount = nullptr;
	toAccount = nullptr;
	fromAmount = model.fromAmount;
	toAmount = model.toAmount;
	note = wxString(model.note);

	date = wxDateTime::Today();
	date.ParseISODate(model.date);

	created = wxDateTime::Now();
	created.ParseISODate(model.created);

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