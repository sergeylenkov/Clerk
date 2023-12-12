#pragma once

#include <wx/datetime.h>
#include "../Repositories/SchedulersRepository.h"
#include "../Services/AccountsService.h"
#include "../Services/TagsService.h"
#include "../../UI/PresentationModels/SchedulerPresentationModel.h"
#include "../../Utils/Types.h"
#include "./HashService.h"

using namespace Clerk::Data;
using namespace Clerk::UI;

namespace Clerk {
	namespace Data {
		class SchedulersService : HashService<std::shared_ptr<SchedulerPresentationModel>> {
		public:
			SchedulersService(SchedulersRepository& schedulersRepository, AccountsService& accountsService, TagsService& tagsService);
			~SchedulersService();

			std::shared_ptr<SchedulerPresentationModel> GetById(int id);
			shared_vector<SchedulerPresentationModel> GetAll();
			shared_vector<SchedulerPresentationModel> GetActiveByPeriod(wxDateTime& fromDate, wxDateTime& toDate);
			void Run(const SchedulerPresentationModel& scheduler);
			void Pause(const SchedulerPresentationModel& scheduler);
			void Execute(const SchedulerPresentationModel& scheduler);
			std::shared_ptr<SchedulerPresentationModel> Save(SchedulerPresentationModel& alert);
			void Delete(SchedulerPresentationModel& alert);

			unsigned int Subscribe(std::function<void()> fn);
			void Unsubscribe(unsigned int subscriptionId);

		private:
			SchedulersRepository& _schedulersRepository;
			AccountsService& _accountsService;
			TagsService& _tagsService;
			EventEmitter* _eventEmitter;

			wxDateTime CalculateNextDate(const SchedulerPresentationModel& scheduler);
			void LoadDetails(std::shared_ptr<SchedulerPresentationModel> transaction, SchedulerModel& model);
		};
	}
}

