#include "GoalsService.h"

using namespace Clerk::Data;

GoalsService::GoalsService(GoalsRepository& goalsRepository):
	_goalsRepository(goalsRepository)
{
	_eventEmitter = new EventEmitter();
	_isLoading = false;
}

GoalsService::~GoalsService() {
	delete _eventEmitter;
}

unsigned int GoalsService::Subscribe(std::function<void()> fn) {
	return _eventEmitter->Subscribe(fn);
}

void GoalsService::Unsubscribe(unsigned int subscriptionId) {
	_eventEmitter->Unsubscribe(subscriptionId);
}


std::shared_ptr<GoalPresentationModel> GoalsService::GetById(int id) {
	auto goal = GetFromHash(id);

	if (goal) {
		return goal;
	}

	auto model = _goalsRepository.GetById(id);

	if (model) {
		goal = std::make_shared<GoalPresentationModel>(*model);
		goal->balance = GetBalance(*goal);
		goal->remainAmount = goal->amount - goal->balance;

		if (goal->remainAmount < 0) {
			goal->remainAmount = 0;
		}

		goal->remainPercent = goal->balance / (goal->amount / 100.0);

		AddToHash(goal->id, goal);

		delete model;

		return goal;
	}

	return nullptr;
}

shared_vector<GoalPresentationModel> GoalsService::GetAll() {
	if (_isLoading && GetHashList().size() > 0) {
		return GetHashList();
	}

	auto models = _goalsRepository.GetAll();

	for (auto& model : models) {
		if (!GetFromHash(model->id)) {
			auto goal = std::make_shared<GoalPresentationModel>(*model);
			goal->balance = GetBalance(*goal);
			goal->remainAmount = goal->amount - goal->balance;

			if (goal->remainAmount < 0) {
				goal->remainAmount = 0;
			}

			goal->remainPercent = goal->balance / (goal->amount / 100.0);

			AddToHash(goal->id, goal);
		}
	}

	for (auto p : models) {
		delete p;
	}

	models.erase(models.begin(), models.end());

	_isLoading = true;

	return GetHashList();
}

float GoalsService::GetBalance(GoalPresentationModel& goal) {
	return _goalsRepository.GetBalance(String::Join(goal.accountsIds, ","));
}

std::shared_ptr<GoalPresentationModel> GoalsService::Save(GoalPresentationModel& goal) {
	GoalModel& model = goal;

	int id = _goalsRepository.Save(model);

	delete& model;

	_eventEmitter->Emit();

	RemoveFromHash(id);

	return GetById(id);
}

void GoalsService::Delete(GoalPresentationModel& goal) {
	GoalModel& model = goal;

	_goalsRepository.Delete(model);

	delete& model;

	RemoveFromHash(goal.id);

	_eventEmitter->Emit();
}