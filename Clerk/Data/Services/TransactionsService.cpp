#include "TransactionsService.h"

TransactionsService::TransactionsService(TransactionsRepository& transactionsRepository, AccountsService& accountsService, TagsService& tagsService):
	_transactionsRepository(transactionsRepository),
	_accountsService(accountsService),
	_tagsService(tagsService)
{
	_eventEmitter = new EventEmitter();
}

TransactionsService::~TransactionsService() {
	delete _eventEmitter;
}

void TransactionsService::OnUpdate(std::function<void()> fn) {
	_eventEmitter->Subscribe(fn);
}

std::shared_ptr<TransactionPresentationModel> TransactionsService::GetById(int id) {
	auto transaction = _transactionsRepository.GetById(id);

	if (transaction) {
		auto model = std::make_shared<TransactionPresentationModel>(*transaction);

		LoadDetails(*model, *transaction);

		return model;
	}

	return nullptr;
}

std::vector<std::shared_ptr<TransactionPresentationModel>> TransactionsService::GetForPeriod(wxDateTime& fromDate, wxDateTime& toDate) {
	auto transactions = _transactionsRepository.GetForPeriod(std::string(fromDate.FormatISODate().ToUTF8()), std::string(toDate.FormatISODate().ToUTF8()));

	std::vector<std::shared_ptr<TransactionPresentationModel>> result;

	std::transform(transactions.begin(), transactions.end(), std::back_inserter(result), [&](const std::shared_ptr<TransactionModel>& transaction) {
		auto model = std::make_shared<TransactionPresentationModel>(*transaction);

		LoadDetails(*model, *transaction);

		return model;
	});

	return result;
}

std::vector<std::shared_ptr<TransactionPresentationModel>> TransactionsService::GetRecents(int count) {
	auto transactions = _transactionsRepository.GetRecents(count);

	std::vector<std::shared_ptr<TransactionPresentationModel>> result;

	std::transform(transactions.begin(), transactions.end(), std::back_inserter(result), [&](const std::shared_ptr<TransactionModel>& transaction) {
		auto model = std::make_shared<TransactionPresentationModel>(*transaction);

		LoadDetails(*model, *transaction);

		return model;
	});

	return result;
}


std::vector<std::shared_ptr<TransactionPresentationModel>> TransactionsService::GetRecents(const AccountPresentationModel& account, int count) {
	auto transactions = _transactionsRepository.GetRecents(account.id, count);

	std::vector<std::shared_ptr<TransactionPresentationModel>> result;

	std::transform(transactions.begin(), transactions.end(), std::back_inserter(result), [&](const std::shared_ptr<TransactionModel>& transaction) {
		auto model = std::make_shared<TransactionPresentationModel>(*transaction);

		LoadDetails(*model, *transaction);

		return model;
	});

	return result;
}

std::vector<std::shared_ptr<TransactionPresentationModel>> TransactionsService::GetDeleted() {
	auto transactions = _transactionsRepository.GetDeleted();

	std::vector<std::shared_ptr<TransactionPresentationModel>> result;

	std::transform(transactions.begin(), transactions.end(), std::back_inserter(result), [&](const std::shared_ptr<TransactionModel>& transaction) {
		auto model = std::make_shared<TransactionPresentationModel>(*transaction);

		LoadDetails(*model, *transaction);

		return model;
	});

	return result;
}

void TransactionsService::Split(TransactionPresentationModel& splitTransaction, TransactionPresentationModel& newTransaction) {
	splitTransaction.fromAmount = splitTransaction.fromAmount - newTransaction.fromAmount;
	splitTransaction.toAmount = splitTransaction.toAmount - newTransaction.toAmount;

	Save(splitTransaction);
}

void TransactionsService::Duplicate(TransactionPresentationModel& transaction) {
	TransactionPresentationModel* copy = new TransactionPresentationModel();

	copy->id = -1;
	copy->fromAccount = transaction.fromAccount;
	copy->toAccount = transaction.toAccount;
	copy->fromAmount = transaction.fromAmount;
	copy->toAmount = transaction.toAmount;
	copy->note = transaction.note;
	copy->tags = transaction.tags;
	copy->date = transaction.date;

	Save(*copy);

	delete copy;
}

void TransactionsService::Save(TransactionPresentationModel& transaction) {
	TransactionModel& model = transaction;

	_transactionsRepository.Save(model);

	transaction.id = model.id;

	delete& model;

	_eventEmitter->Emit();
}

void TransactionsService::Delete(TransactionPresentationModel& transaction) {
	TransactionModel& model = transaction;

	_transactionsRepository.Delete(model);

	delete& model;

	_eventEmitter->Emit();
}

void TransactionsService::LoadDetails(TransactionPresentationModel& model, TransactionModel& transaction) {
	model.fromAccount = _accountsService.GetById(transaction.fromAccountId);
	model.toAccount = _accountsService.GetById(transaction.toAccountId);

	model.tags.clear();
	model.tagsString = wxString("");

	for (auto& tagId : transaction.tagsIds) {
		auto tag = _tagsService.GetById(tagId);

		model.tags.push_back(tag);
		model.tagsString += tag->name + ", ";
	}

	model.tagsString.RemoveLast(2);
}