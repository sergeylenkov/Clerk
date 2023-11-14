#include "SchedulersService.h"

SchedulersService::SchedulersService(SchedulersRepository& schedulersRepository, AccountsRepository& accountsRepository, ExchangeRatesRepository& exchangeRatesRepository):
	_schedulersRepository(schedulersRepository),
	_accountsRepository(accountsRepository),
	_exchangeRatesRepository(exchangeRatesRepository)
{
}

void SchedulersService::SetBaseCurrency(int id) {
	_baseCurrencyId = id;
}

std::shared_ptr<SchedulerPresentationModel> SchedulersService::GetById(int id) {
	auto scheduler = _schedulersRepository.GetById(id);

	if (scheduler) {
		return std::make_shared<SchedulerPresentationModel>(*scheduler);
	}

	return nullptr;
}

std::vector<std::shared_ptr<SchedulerPresentationModel>> SchedulersService::GetAll() {
	auto schedulers = _schedulersRepository.GetAll();

	std::vector<std::shared_ptr<SchedulerPresentationModel>> result;

	std::transform(schedulers.begin(), schedulers.end(), std::back_inserter(result), [&](const std::shared_ptr<SchedulerModel>& scheduler) {
		auto model = std::make_shared<SchedulerPresentationModel>(*scheduler);

		auto account = _accountsRepository.GetById(scheduler->toAccountId);

		float rate = _exchangeRatesRepository.GetExchangeRate(account->currencyId, _baseCurrencyId);
		model->amount = scheduler->toAmount * rate;

		delete account;

		return model;
	});

	return result;
}

std::vector<std::shared_ptr<SchedulerPresentationModel>> SchedulersService::GetByPeriod(wxDateTime& fromDate, wxDateTime& toDate) {
	auto schedulers = GetAll();
	std::vector<std::shared_ptr<SchedulerPresentationModel>> result;

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