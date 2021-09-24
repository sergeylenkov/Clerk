#include "TransactionsService.h"

TransactionsService::TransactionsService(TransactionsRepository& transactionsRepository, AccountsService& accountsService, TagsService& tagsService):
	_transactionsRepository(transactionsRepository),
	_accountsService(accountsService),
	_tagsService(tagsService)
{
}

std::shared_ptr<TransactionViewModel> TransactionsService::GetById(int id) {
	auto transaction = _transactionsRepository.GetById(id);

	if (transaction) {
		auto model = std::make_shared<TransactionViewModel>(*transaction);

		LoadDetails(*model, *transaction);

		return model;
	}

	return nullptr;
}

std::vector<std::shared_ptr<TransactionViewModel>> TransactionsService::GetForPeriod(wxDateTime& fromDate, wxDateTime& toDate) {
	auto transactions = _transactionsRepository.GetForPeriod(std::string(fromDate.FormatISODate().ToUTF8()), std::string(toDate.FormatISODate().ToUTF8()));

	std::vector<std::shared_ptr<TransactionViewModel>> result;

	std::transform(transactions.begin(), transactions.end(), std::back_inserter(result), [this](const std::shared_ptr<TransactionModel>& transaction) {
		auto model = std::make_shared<TransactionViewModel>(*transaction);

		LoadDetails(*model, *transaction);

		return model;
	});

	return result;
}

std::vector<std::shared_ptr<TransactionViewModel>> TransactionsService::GetRecents(int count) {
	auto transactions = _transactionsRepository.GetRecents(count);

	std::vector<std::shared_ptr<TransactionViewModel>> result;

	std::transform(transactions.begin(), transactions.end(), std::back_inserter(result), [this](const std::shared_ptr<TransactionModel>& transaction) {
		auto model = std::make_shared<TransactionViewModel>(*transaction);

		LoadDetails(*model, *transaction);

		return model;
	});

	return result;
}


std::vector<std::shared_ptr<TransactionViewModel>> TransactionsService::GetRecents(const AccountViewModel& account, int count) {
	auto transactions = _transactionsRepository.GetRecents(account.id, count);

	std::vector<std::shared_ptr<TransactionViewModel>> result;

	std::transform(transactions.begin(), transactions.end(), std::back_inserter(result), [this](const std::shared_ptr<TransactionModel>& transaction) {
		auto model = std::make_shared<TransactionViewModel>(*transaction);

		LoadDetails(*model, *transaction);

		return model;
	});

	return result;
}

std::vector<std::shared_ptr<TransactionViewModel>> TransactionsService::GetDeleted() {
	auto transactions = _transactionsRepository.GetDeleted();

	std::vector<std::shared_ptr<TransactionViewModel>> result;

	std::transform(transactions.begin(), transactions.end(), std::back_inserter(result), [this](const std::shared_ptr<TransactionModel>& transaction) {
		auto model = std::make_shared<TransactionViewModel>(*transaction);

		LoadDetails(*model, *transaction);

		return model;
	});

	return result;
}

void TransactionsService::Save(TransactionViewModel& viewModel) {
	TransactionModel& model = viewModel;

	_transactionsRepository.Save(model);

	delete& model;

	if (OnUpdate) {
		OnUpdate();
	}
}

void TransactionsService::LoadDetails(TransactionViewModel& model, TransactionModel& transaction) {
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