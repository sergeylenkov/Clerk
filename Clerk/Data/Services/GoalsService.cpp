#include "GoalsService.h"

using namespace Clerk::Data;

GoalsService::GoalsService(GoalsRepository& goalsRepository) : _goalsRepository(goalsRepository) {
	_isLoading = false;
}

std::shared_ptr<GoalPresentationModel> GoalsService::GetById(int id) {
	auto goal = GetFromHash(id);

	if (goal) {
		return goal;
	}

	auto model = _goalsRepository.GetById(id);

	if (model) {
		goal = std::make_shared<GoalPresentationModel>(*model);
		goal->balance = _goalsRepository.GetBalance(goal->accountIds);

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
			goal->balance = _goalsRepository.GetBalance(goal->accountIds);

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