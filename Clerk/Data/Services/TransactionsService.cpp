#include "TransactionsService.h"

TransactionsService::TransactionsService(TransactionsRepository& transactionsRepository, AccountsService& accountsService, TagsService& tagsService):
	_transactionsRepository(transactionsRepository),
	_accountsService(accountsService),
	_tagsService(tagsService)
{
	_eventEmitter = new EventEmitter();
	_isLoading = false;
}

TransactionsService::~TransactionsService() {
	delete _eventEmitter;
}

unsigned int TransactionsService::Subscribe(std::function<void()> fn) {
	return _eventEmitter->Subscribe(fn);
}

void TransactionsService::Unsubscribe(unsigned int subscriptionId) {
	_eventEmitter->Unsubscribe(subscriptionId);
}

std::shared_ptr<TransactionPresentationModel> TransactionsService::GetById(int id) {
	auto transaction = GetFromHash(id);

	if (transaction) {
		return transaction;
	}

	auto model = _transactionsRepository.GetById(id);

	if (model) {
		transaction = std::make_shared<TransactionPresentationModel>(*model);
		LoadDetails(transaction, *model);

		AddToHash(transaction->id, transaction);

		delete model;

		return transaction;
	}

	return nullptr;
}

shared_vector<TransactionPresentationModel> TransactionsService::GetForPeriod(wxDateTime& fromDate, wxDateTime& toDate) {
	shared_vector<TransactionPresentationModel> result;

	auto models = _transactionsRepository.GetForPeriod(std::string(fromDate.FormatISODate().ToUTF8()), std::string(toDate.FormatISODate().ToUTF8()));

	for (auto& model : models) {
		result.push_back(GetById(model->id));
	}

	for (auto p : models) {
		delete p;
	}

	return result;
}

shared_vector<TransactionPresentationModel> TransactionsService::GetRecents(int count) {
	shared_vector<TransactionPresentationModel> result;

	auto models = _transactionsRepository.GetRecents(count);

	for (auto& model : models) {
		result.push_back(GetById(model->id));
	}

	for (auto p : models) {
		delete p;
	}

	return result;
}


shared_vector<TransactionPresentationModel> TransactionsService::GetRecents(const AccountPresentationModel& account, int count) {
	shared_vector<TransactionPresentationModel> result;

	auto models = _transactionsRepository.GetRecents(account.id, count);

	for (auto& model : models) {
		result.push_back(GetById(model->id));
	}

	for (auto p : models) {
		delete p;
	}

	return result;
}

shared_vector<TransactionPresentationModel> TransactionsService::GetDeleted() {
	std::vector<std::shared_ptr<TransactionPresentationModel>> result;

	auto models = _transactionsRepository.GetDeleted();

	for (auto& model : models) {
		result.push_back(GetById(model->id));
	}

	for (auto p : models) {
		delete p;
	}

	return result;
}

void TransactionsService::Split(TransactionPresentationModel& splitTransaction, const TransactionPresentationModel& newTransaction) {
	splitTransaction.fromAmount = splitTransaction.fromAmount - newTransaction.fromAmount;
	splitTransaction.toAmount = splitTransaction.toAmount - newTransaction.toAmount;

	Save(splitTransaction);
}

std::shared_ptr<TransactionPresentationModel> TransactionsService::Duplicate(const TransactionPresentationModel& transaction) {
	auto copy = std::make_shared<TransactionPresentationModel>();

	copy->id = -1;
	copy->fromAccount = transaction.fromAccount;
	copy->toAccount = transaction.toAccount;
	copy->fromAmount = transaction.fromAmount;
	copy->toAmount = transaction.toAmount;
	copy->note = transaction.note;
	copy->tags = transaction.tags;
	copy->date = transaction.date;

	return Save(*copy);
}

std::shared_ptr<TransactionPresentationModel> TransactionsService::Save(TransactionPresentationModel& transaction) {
	for (auto& tag : transaction.tags) {
		if (tag->id == -1) {
			tag = _tagsService.Save(*tag);
		}
	}

	TransactionModel& model = transaction;

	int id = _transactionsRepository.Save(model);

	delete& model;

	RemoveFromHash(id);

	_eventEmitter->Emit();

	return GetById(id);
}

void TransactionsService::Delete(TransactionPresentationModel& transaction) {
	TransactionModel& model = transaction;

	_transactionsRepository.Delete(model);

	delete& model;

	RemoveFromHash(transaction.id);

	_eventEmitter->Emit();
}

void TransactionsService::LoadDetails(std::shared_ptr<TransactionPresentationModel> transaction, TransactionModel& model) {
	transaction->fromAccount = _accountsService.GetById(model.fromAccountId);
	transaction->toAccount = _accountsService.GetById(model.toAccountId);

	transaction->tags.clear();
	transaction->tagsString = wxString("");

	for (auto& tagId : model.tagsIds) {
		auto tag = _tagsService.GetById(tagId);

		transaction->tags.push_back(tag);
		transaction->tagsString += tag->name + ", ";
	}

	transaction->tagsString.RemoveLast(2);
}