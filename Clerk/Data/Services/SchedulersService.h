#pragma once

#include <wx/datetime.h>
#include "../Repositories/SchedulersRepository.h"
#include "../Services/CurrenciesService.h"
#include "../Services/AccountsService.h"
#include "../../UI/PresentationModels/SchedulerPresentationModel.h"
#include "../../Utils/Types.h"
#include "./HashService.h"

using namespace Clerk::Data;
using namespace Clerk::UI;

namespace Clerk {
	namespace Data {
		class SchedulersService : HashService<std::shared_ptr<SchedulerPresentationModel>> {
		public:
			SchedulersService(SchedulersRepository& schedulersRepository, AccountsService& accountsService, CurrenciesService& crrenciesService);

			std::shared_ptr<SchedulerPresentationModel> GetById(int id);
			shared_vector<SchedulerPresentationModel> GetAll();
			shared_vector<SchedulerPresentationModel> GetByPeriod(wxDateTime& fromDate, wxDateTime& toDate);
			void Run(const SchedulerPresentationModel& scheduler);
			void Pause(const SchedulerPresentationModel& scheduler);
			void Execute(const SchedulerPresentationModel& scheduler);

		private:
			SchedulersRepository& _schedulersRepository;
			AccountsService& _accountsService;
			CurrenciesService& _currenciesService;

			wxDateTime CalculateNextDate(const SchedulerPresentationModel& scheduler);
		};
	}
}

