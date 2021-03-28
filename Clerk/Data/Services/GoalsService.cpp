#include "GoalsService.h"

using namespace Clerk::Data;

GoalsService::GoalsService(GoalsRepository& goalsRepository) : _goalsRepository(goalsRepository) {

}

std::shared_ptr<GoalViewModel> GoalsService::GetById(int id) {
	auto goal = _goalsRepository.GetById(id);

	if (goal) {
		return std::make_shared<GoalViewModel>(*goal);
	}

	return nullptr;
}

std::vector<std::shared_ptr<GoalViewModel>> GoalsService::GetAll() {
	auto goals = _goalsRepository.GetAll();

	std::vector<std::shared_ptr<GoalViewModel>> result;

	std::transform(goals.begin(), goals.end(), std::back_inserter(result), [=](const std::shared_ptr<GoalModel>& goal) {
		auto model = std::make_shared<GoalViewModel>(*goal);
		model->balance = _goalsRepository.GetBalance(goal->accountIds);

		return model;
	});

	return result;
}