#pragma once

#include <wx/datetime.h>
#include "../Repositories/SchedulersRepository.h"
#include "../Repositories/AccountsRepository.h"
#include "../Repositories/ExchangeRatesRepository.h"
#include "../ViewModels/SchedulerViewModel.h"

using namespace Clerk::Data;

namespace Clerk {
	namespace Data {
		class SchedulersService {
		public:
			SchedulersService(SchedulersRepository& schedulersRepository, AccountsRepository& accountsRepository,  ExchangeRatesRepository& exchangeRatesRepository);

			void SetBaseCurrency(int id);
			std::shared_ptr<SchedulerViewModel> GetById(int id);
			std::vector<std::shared_ptr<SchedulerViewModel>> GetAll();
			std::vector<std::shared_ptr<SchedulerViewModel>> GetByPeriod(wxDateTime& fromDate, wxDateTime& toDate);
			void Run(const SchedulerViewModel& scheduler);
			void Pause(const SchedulerViewModel& scheduler);			
			void Execute(const SchedulerViewModel& scheduler);			

		private:
			SchedulersRepository& _schedulersRepository;
			AccountsRepository& _accountsRepository;
			ExchangeRatesRepository& _exchangeRatesRepository;

			int _baseCurrencyId = 0;

			wxDateTime CalculateNextDate(const SchedulerViewModel& scheduler);
		};
	}
}

