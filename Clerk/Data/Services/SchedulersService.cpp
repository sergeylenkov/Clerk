#include "SchedulersService.h"

SchedulersService::SchedulersService(SchedulersRepository& schedulersRepository, AccountsService& accountsService, CurrenciesService& currenciesService):
	_schedulersRepository(schedulersRepository),
	_accountsService(accountsService),
	_currenciesService(currenciesService)
{
	_isLoading = false;
}

std::shared_ptr<SchedulerPresentationModel> SchedulersService::GetById(int id) {
	auto scheduler = GetFromHash(id);

	if (scheduler) {
		return scheduler;
	}

	auto model = _schedulersRepository.GetById(id);

	if (model) {
		scheduler = std::make_shared<SchedulerPresentationModel>(*model);

		auto account = _accountsService.GetById(model->toAccountId);
		CurrencyPresentationModel& baseCurrency = *_currenciesService.GetBaseCurrency();

		float rate = _currenciesService.GetExchangeRate(*account->currency, baseCurrency);
		scheduler->amount = model->toAmount * rate;

		AddToHash(scheduler->id, scheduler);

		delete model;

		return scheduler;
	}

	return nullptr;
}

shared_vector<SchedulerPresentationModel> SchedulersService::GetAll() {
	if (_isLoading && GetHashList().size() > 0) {
		return GetHashList();
	}

	CurrencyPresentationModel& baseCurrency = *_currenciesService.GetBaseCurrency();
	auto models = _schedulersRepository.GetAll();

	for (auto& model : models) {
		if (!GetFromHash(model->id)) {
			auto scheduler = std::make_shared<SchedulerPresentationModel>(*model);

			auto account = _accountsService.GetById(model->toAccountId);

			float rate = _currenciesService.GetExchangeRate(*account->currency, baseCurrency);
			scheduler->amount = model->toAmount * rate;

			AddToHash(scheduler->id, scheduler);
		}
	}

	for (auto p : models) {
		delete p;
	}

	models.erase(models.begin(), models.end());

	_isLoading = true;

	return GetHashList();
}

shared_vector<SchedulerPresentationModel> SchedulersService::GetByPeriod(wxDateTime& fromDate, wxDateTime& toDate) {
	auto schedulers = GetAll();
	shared_vector<SchedulerPresentationModel> result;

	std::copy_if(schedulers.begin(), schedulers.end(), std::back_inserter(result), [fromDate, toDate](const std::shared_ptr<SchedulerPresentationModel>& scheduler) {
		return scheduler->isActive && scheduler->nextDate.IsBetween(fromDate, toDate);
	});

	std::sort(result.begin(), result.end(), [](std::shared_ptr<SchedulerPresentationModel> a, std::shared_ptr<SchedulerPresentationModel> b) {
		return a->nextDate.IsEarlierThan(b->nextDate);
	});

	return result;
}

void SchedulersService::Run(const SchedulerPresentationModel& scheduler) {
	auto nextDate = CalculateNextDate(scheduler);

	auto model = _schedulersRepository.GetById(scheduler.id);

	model->active = true;
	model->previousDate = scheduler.nextDate.FormatISODate();
	model->nextDate = nextDate.FormatISODate();

	_schedulersRepository.Save(*model);
}

void SchedulersService::Pause(const SchedulerPresentationModel& scheduler) {
	auto model = _schedulersRepository.GetById(scheduler.id);
	model->active = false;

	_schedulersRepository.Save(*model);
}

wxDateTime SchedulersService::CalculateNextDate(const SchedulerPresentationModel& scheduler) {
	auto date = wxDateTime(scheduler.previousDate);

	if (scheduler.type == SchedulerType::Daily) {
		date.Add(wxDateSpan::Days(scheduler.day));
	}

	if (scheduler.type == SchedulerType::Weekly) {
		date.Add(wxDateSpan::Weeks(scheduler.week));
		date.SetToNextWeekDay(static_cast<wxDateTime::WeekDay>(scheduler.day));
	}

	if (scheduler.type == SchedulerType::Monthly) {
		date.Add(wxDateSpan::Months(scheduler.month));
		date.SetDay(scheduler.day);
	}

	if (scheduler.type == SchedulerType::Yearly) {
		date.SetDay(scheduler.day);
		date.SetMonth(static_cast<wxDateTime::Month>(scheduler.month));

		if (date.IsEarlierThan(wxDateTime::Now())) {
			date.SetYear(date.GetYear() + 1);
		}
	}

	return date;
}

void SchedulersService::Execute(const SchedulerPresentationModel& scheduler) {
	/*auto transaction = new Transaction();

	transaction->fromAccount = fromAccount;
	transaction->toAccount = toAccount;
	transaction->fromAmount = fromAmount;
	transaction->toAmount = toAmount;
	//transaction->SetTagsString(*tags.get());
	transaction->paidAt = nextDate;

	//transaction->Save();

	delete transaction;*/
}