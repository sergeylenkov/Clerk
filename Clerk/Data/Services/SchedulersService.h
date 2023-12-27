#pragma once

#include <wx/datetime.h>
#include "../../Utils/Types.h"
#include "../Repositories/SchedulersRepository.h"
#include "../Services/AccountsService.h"
#include "../Services/TagsService.h"
#include "../Services/TransactionsService.h"
#include "../PresentationModels/SchedulerPresentationModel.h"
#include "../PresentationModels/TransactionPresentationModel.h"
#include "HashService.h"

namespace Clerk {
	namespace Data {
		class SchedulersService : HashService<std::shared_ptr<SchedulerPresentationModel>> {
		public:
			SchedulersService(SchedulersRepository& schedulersRepository, AccountsService& accountsService, TagsService& tagsService, TransactionsService& transactionsService);
			~SchedulersService();

			std::shared_ptr<SchedulerPresentationModel> GetById(int id);
			shared_vector<SchedulerPresentationModel> GetAll();
			shared_vector<SchedulerPresentationModel> GetActiveByPeriod(wxDateTime& fromDate, wxDateTime& toDate);
			shared_vector<SchedulerPresentationModel> GetActiveOnDate(wxDateTime& date);
			void Run(SchedulerPresentationModel& scheduler);
			void Pause(SchedulerPresentationModel& scheduler);
			void Execute(SchedulerPresentationModel& scheduler);
			void Skip(SchedulerPresentationModel& scheduler);
			std::shared_ptr<SchedulerPresentationModel> Save(SchedulerPresentationModel& scheduler);
			void Delete(SchedulerPresentationModel& scheduler);

			unsigned int Subscribe(std::function<void()> fn);
			void Unsubscribe(unsigned int subscriptionId);

		private:
			SchedulersRepository& _schedulersRepository;
			AccountsService& _accountsService;
			TagsService& _tagsService;
			TransactionsService& _transactionsService;
			EventEmitter* _eventEmitter;

			wxDateTime CalculateNextDate(SchedulerPresentationModel& scheduler);
			void LoadDetails(std::shared_ptr<SchedulerPresentationModel> scheduler, SchedulerModel& model);
		};
	}
}

