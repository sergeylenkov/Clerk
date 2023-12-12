#include "SchedulersService.h"

SchedulersService::SchedulersService(SchedulersRepository& schedulersRepository, AccountsService& accountsService, TagsService& tagsService):
	_schedulersRepository(schedulersRepository),
	_accountsService(accountsService),
	_tagsService(tagsService)
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