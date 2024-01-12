#include "SchedulerViewModel.h"

using namespace Clerk::UI;

SchedulerViewModel::SchedulerViewModel(SchedulersService& schedulersService, AccountsService& accountsService, CurrenciesService& currenciesService, TagsService& tagsService):
	_schedulersService(schedulersService),
	_accountsService(accountsService),
	_currenciesService(currenciesService),
	_tagsService(tagsService)
{
	_id = -1;
	_name = "";
	_fromAmount = 0;
	_toAmount = 0;
	_type = SchedulerType::Monthly;
	_day = 1;
	_week = 1;
	_month = 1;
}

void SchedulerViewModel::SetSchedulerId(int id) {
	auto scheduler = _schedulersService.GetById(id);

	if (scheduler) {
		_id = scheduler->id;
		_name = scheduler->name;
		_fromAccount = scheduler->fromAccount;
		_toAccount = scheduler->toAccount;
		_fromAmount = scheduler->fromAmount;
		_toAmount = scheduler->toAmount;
		_tags = scheduler->tags;
		_type = scheduler->type;
		_day = scheduler->day;
		_week = scheduler->week;
		_month = scheduler->month;

		Update();
	}
}

bool SchedulerViewModel::IsNew() {
	return _id == -1;
}

void SchedulerViewModel::SetName(wxString name) {
	_name = name;

	if (OnUpdate) {
		OnUpdate(SchedulerViewModelField::Name);
	}
}

wxString SchedulerViewModel::GetName() {
	return _name;
}

void SchedulerViewModel::Save() {
	auto scheduler = _schedulersService.GetById(_id);

	if (!scheduler) {
		scheduler = std::make_shared<SchedulerPresentationModel>();
	}

	scheduler->name = _name;

	_schedulersService.Save(*scheduler);
}

void SchedulerViewModel::Update() {
	UpdateFromAccounts();
	UpdateToAccounts();

	if (!_fromAccount) {
		_fromAccount = _fromAccounts[0];
	}

	if (!_toAccount) {
		_toAccount = _toAccounts[0];
	}
}

void SchedulerViewModel::UpdateFromAccounts() {
	auto receipts = _accountsService.GetByType(AccountType::Receipt);
	auto deposits = _accountsService.GetByType(AccountType::Deposit);

	std::sort(receipts.begin(), receipts.end(), [](auto a, auto b) {
		return a->order < b->order;
	});

	std::sort(deposits.begin(), deposits.end(), [](auto a, auto b) {
		return a->order < b->order;
	});

	_fromAccounts.clear();

	for (auto& account : receipts) {
		if (_toAccount && account->id != _toAccount->id) {
			_fromAccounts.push_back(account);
		}
		else {
			_fromAccounts.push_back(account);
		}
	}

	for (auto& account : deposits) {
		if (_toAccount && account->id != _toAccount->id) {
			_fromAccounts.push_back(account);
		}
		else {
			_fromAccounts.push_back(account);
		}
	}
}

void SchedulerViewModel::UpdateToAccounts() {
	auto deposits = _accountsService.GetByType(AccountType::Deposit);
	auto virtuals = _accountsService.GetByType(AccountType::Virtual);
	auto expenses = _accountsService.GetByType(AccountType::Expens);
	auto debts = _accountsService.GetByType(AccountType::Debt);

	std::sort(deposits.begin(), deposits.end(), [](auto a, auto b) {
		return a->order < b->order;
		});

	std::sort(virtuals.begin(), virtuals.end(), [](auto a, auto b) {
		return a->order < b->order;
		});

	std::sort(expenses.begin(), expenses.end(), [](auto a, auto b) {
		return a->order < b->order;
		});

	std::sort(debts.begin(), debts.end(), [](auto a, auto b) {
		return a->order < b->order;
		});

	_toAccounts.clear();

	shared_vector<AccountPresentationModel> filtered;

	filtered.insert(filtered.end(), deposits.begin(), deposits.end());
	filtered.insert(filtered.end(), virtuals.begin(), virtuals.end());

	if (_fromAccount->type == AccountType::Deposit) {
		filtered.insert(filtered.end(), expenses.begin(), expenses.end());
		filtered.insert(filtered.end(), debts.begin(), debts.end());
	}

	for (auto& account : filtered) {
		if (_fromAccount && account->id != _fromAccount->id) {
			_toAccounts.push_back(account);
		}
		else {
			_toAccounts.push_back(account);
		}
	}
}

shared_vector<AccountPresentationModel> SchedulerViewModel::GetFromAccounts() {
	return _fromAccounts;
}

shared_vector<AccountPresentationModel> SchedulerViewModel::GetToAccounts() {
	return _toAccounts;
}

int SchedulerViewModel::GetFromAccountIndex() {
	for (unsigned int i = 0; i < _fromAccounts.size(); i++) {
		if (_fromAccount->id == _fromAccounts[i]->id) {
			return i;
		}
	}

	return 0;
}

void SchedulerViewModel::SetFromAccount(int index) {
	_fromAccount = _fromAccounts[index];

	const auto result = std::find_if(_toAccounts.begin(), _toAccounts.end(), [&](auto account) {
		return account->id == _fromAccount->id;
		});

	if (result != _toAccounts.end()) {
		UpdateToAccounts();

		if (OnUpdate) {
			OnUpdate(SchedulerViewModelField::FromAccount);
		}
	}
}

int SchedulerViewModel::GetToAccountIndex() {
	for (unsigned int i = 0; i < _toAccounts.size(); i++) {
		if (_toAccount->id == _toAccounts[i]->id) {
			return i;
		}
	}

	return 0;
}

void SchedulerViewModel::SetToAccount(int index) {
	_toAccount = _toAccounts[index];

	const auto result = std::find_if(_fromAccounts.begin(), _fromAccounts.end(), [&](auto account) {
		return account->id == _toAccount->id;
		});

	if (result != _fromAccounts.end()) {
		UpdateFromAccounts();

		if (OnUpdate) {
			OnUpdate(SchedulerViewModelField::ToAccount);
		}
	}
}

float SchedulerViewModel::GetFromAmount() {
	return _fromAmount;
}

void SchedulerViewModel::SetFromAmount(float amount) {
	_fromAmount = amount;

	if (_toAmount == 0) {
		float rate = 1;

		if (Settings::GetInstance().IsConvertCurrency()) {
			rate = _currenciesService.GetExchangeRate(*_fromAccount->currency, *_toAccount->currency);
		}

		_toAmount = _fromAmount * rate;
	}

	if (_fromAmount == amount && OnUpdate) {
		OnUpdate(SchedulerViewModelField::FromAmount);
	}
}

float SchedulerViewModel::GetToAmount() {
	return _toAmount;
}

void SchedulerViewModel::SetToAmount(float amount) {
	_toAmount = amount;

	if (_toAmount == amount && OnUpdate) {
		OnUpdate(SchedulerViewModelField::ToAmount);
	}
}

shared_vector<TagPresentationModel> SchedulerViewModel::GetTags() {
	return _tags;
}

void SchedulerViewModel::SetTags(shared_vector<TagPresentationModel> tags) {
	_tags = tags;

	if (OnUpdate) {
		OnUpdate(SchedulerViewModelField::Tags);
	}
}

SchedulerType SchedulerViewModel::GetType() {
	return _type;
}

void SchedulerViewModel::SetType(SchedulerType type) {
	_type = type;

	if (OnUpdate) {
		OnUpdate(SchedulerViewModelField::Type);
	}
}

int SchedulerViewModel::GetDay() {
	return _day;
}

void SchedulerViewModel::SetDay(int day) {
	_day = day;

	if (OnUpdate) {
		OnUpdate(SchedulerViewModelField::Day);
	}
}

int SchedulerViewModel::GetWeek() {
	return _week;
}

void SchedulerViewModel::SetWeek(int week) {
	_week = week;

	if (OnUpdate) {
		OnUpdate(SchedulerViewModelField::Week);
	}
}

int SchedulerViewModel::GetMonth() {
	return _month;
}

void SchedulerViewModel::SetMonth(int month) {
	_month = month;

	if (OnUpdate) {
		OnUpdate(SchedulerViewModelField::Month);
	}
}