#include "SchedulersService.h"

SchedulersService::SchedulersService(SchedulersRepository& schedulersRepository, AccountsService& accountsService, TagsService& tagsService, TransactionsService& transactionsService) :
	_schedulersRepository(schedulersRepository),
	_accountsService(accountsService),
	_tagsService(tagsService),
	_transactionsService(transactionsService)
{
	_eventEmitter = new EventEmitter();
	_isLoading = false;
}

SchedulersService::~SchedulersService() {
	delete _eventEmitter;
}

unsigned int SchedulersService::Subscribe(std::function<void()> fn) {
	return _eventEmitter->Subscribe(fn);
}

void SchedulersService::Unsubscribe(unsigned int subscriptionId) {
	_eventEmitter->Unsubscribe(subscriptionId);
}

std::shared_ptr<SchedulerPresentationModel> SchedulersService::GetById(int id) {
	auto scheduler = GetFromHash(id);

	if (scheduler) {
		return scheduler;
	}

	auto model = _schedulersRepository.GetById(id);

	if (model) {
		scheduler = std::make_shared<SchedulerPresentationModel>(*model);
		LoadDetails(scheduler, *model);

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

	auto models = _schedulersRepository.GetAll();

	for (auto& model : models) {
		if (!GetFromHash(model->id)) {
			auto scheduler = std::make_shared<SchedulerPresentationModel>(*model);
			LoadDetails(scheduler, *model);

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

shared_vector<SchedulerPresentationModel> SchedulersService::GetActiveByPeriod(wxDateTime& fromDate, wxDateTime& toDate) {
	auto schedulers = GetAll();
	shared_vector<SchedulerPresentationModel> result;

	std::copy_if(schedulers.begin(), schedulers.end(), std::back_inserter(result), [fromDate, toDate](const std::shared_ptr<SchedulerPresentationModel>& scheduler) {
		return scheduler->isActive && scheduler->nextDate.IsBetween(fromDate, toDate);
	});

	return result;
}

shared_vector<SchedulerPresentationModel> SchedulersService::GetActiveOnDate(wxDateTime& date) {
	auto schedulers = GetAll();
	shared_vector<SchedulerPresentationModel> result;

	std::copy_if(schedulers.begin(), schedulers.end(), std::back_inserter(result), [date](const std::shared_ptr<SchedulerPresentationModel>& scheduler) {
		if (scheduler->isActive) {
			return scheduler->nextDate.IsBetween(scheduler->previousDate, date);
		}

		return false;
	});

	return result;
}

void SchedulersService::Run(SchedulerPresentationModel& scheduler) {
	scheduler.previousDate = wxDateTime::Now();
	scheduler.nextDate = wxDateTime::Now();

	scheduler.nextDate = CalculateNextDate(scheduler);
	scheduler.isActive = true;

	Save(scheduler);

	_eventEmitter->Emit();
}

void SchedulersService::Pause(SchedulerPresentationModel& scheduler) {
	scheduler.isActive = false;

	Save(scheduler);

	_eventEmitter->Emit();
}

void SchedulersService::Execute(SchedulerPresentationModel& scheduler) {
	auto transaction = std::make_shared<TransactionPresentationModel>();

	transaction->fromAccount = scheduler.fromAccount;
	transaction->toAccount = scheduler.toAccount;
	transaction->fromAmount = scheduler.fromAmount;
	transaction->toAmount = scheduler.toAmount;
	transaction->tags = scheduler.tags;
	transaction->date = scheduler.nextDate;

	_transactionsService.Save(*transaction);

	scheduler.nextDate = CalculateNextDate(scheduler);

	Save(scheduler);

	_eventEmitter->Emit();
}

void SchedulersService::Skip(SchedulerPresentationModel& scheduler) {
	scheduler.nextDate = CalculateNextDate(scheduler);

	Save(scheduler);

	_eventEmitter->Emit();
}

std::shared_ptr<SchedulerPresentationModel> SchedulersService::Save(SchedulerPresentationModel& scheduler) {
	SchedulerModel& model = scheduler;

	int id = _schedulersRepository.Save(model);

	delete& model;

	_eventEmitter->Emit();

	RemoveFromHash(id);

	return GetById(id);
}

void SchedulersService::Delete(SchedulerPresentationModel& scheduler) {
	SchedulerModel& model = scheduler;

	_schedulersRepository.Delete(model);

	delete& model;

	RemoveFromHash(scheduler.id);

	_eventEmitter->Emit();
}

void SchedulersService::LoadDetails(std::shared_ptr<SchedulerPresentationModel> transaction, SchedulerModel& model) {
	transaction->fromAccount = _accountsService.GetById(model.fromAccountId);
	transaction->toAccount = _accountsService.GetById(model.toAccountId);

	transaction->tags.clear();

	for (auto& tagId : model.tagsIds) {
		auto tag = _tagsService.GetById(tagId);

		transaction->tags.push_back(tag);
	}
}

wxDateTime SchedulersService::CalculateNextDate(SchedulerPresentationModel& scheduler) {
	scheduler.previousDate = scheduler.nextDate;

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