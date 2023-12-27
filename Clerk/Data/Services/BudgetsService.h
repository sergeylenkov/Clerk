#pragma once

#include "../../Utils/Types.h"
#include "../../Utils/EventEmitter.h"
#include "../Repositories/BudgetsRepository.h"
#include "../PresentationModels/BudgetPresentationModel.h"
#include "HashService.h"

namespace Clerk {
	namespace Data {
		class BudgetsService : HashService<std::shared_ptr<BudgetPresentationModel>> {
		public:
			BudgetsService(BudgetsRepository& budgetsRepository);
			~BudgetsService();

			std::shared_ptr<BudgetPresentationModel> GetById(int id);
			shared_vector<BudgetPresentationModel> GetAll();
			void UpdateBalance();			

			std::shared_ptr<BudgetPresentationModel> Save(BudgetPresentationModel& budget);
			void Delete(BudgetPresentationModel& budget);

			unsigned int Subscribe(std::function<void()> fn);
			void Unsubscribe(unsigned int subscriptionId);

		private:
			BudgetsRepository& _budgetsRepository;
			EventEmitter* _eventEmitter;

			float GetExpenses(BudgetPresentationModel& budget, std::string& fromDate, std::string& toDate);
		};
	}
}

