#pragma once

#include <wx/datetime.h>
#include "../Repositories/SchedulersRepository.h"
#include "../Repositories/AccountsRepository.h"
#include "../Repositories/ExchangeRatesRepository.h"
#include "../../UI/PresentationModels/SchedulerPresentationModel.h"

using namespace Clerk::Data;
using namespace Clerk::UI;

namespace Clerk {
	namespace Data {
		class SchedulersService {
		public:
			SchedulersService(SchedulersRepository& schedulersRepository, AccountsRepository& accountsRepository,  ExchangeRatesRepository& exchangeRatesRepository);

			void SetBaseCurrency(int id);
			std::shared_ptr<SchedulerPresentationModel> GetById(int id);
			std::vector<std::shared_ptr<SchedulerPresentationModel>> GetAll();
			std::vector<std::shared_ptr<SchedulerPresentationModel>> GetByPeriod(wxDateTime& fromDate, wxDateTime& toDate);
			void Run(const SchedulerPresentationModel& scheduler);
			void Pause(const SchedulerPresentationModel& scheduler);
			void Execute(const SchedulerPresentationModel& scheduler);

		private:
			SchedulersRepository& _schedulersRepository;
			AccountsRepository& _accountsRepository;
			ExchangeRatesRepository& _exchangeRatesRepository;

			int _baseCurrencyId = 0;

			wxDateTime CalculateNextDate(const SchedulerPresentationModel& scheduler);
		};
	}
}

