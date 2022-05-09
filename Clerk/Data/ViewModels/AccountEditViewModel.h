#pragma once

#include "./AccountViewModel.h"
#include "../Services/AccountsService.h"
#include "../Repositories/CurrenciesRepository.h"

namespace Clerk {
	namespace Data {
		class AccountEditViewModel {
		public:
			AccountEditViewModel(AccountsService& accountsService, CurrenciesRepository& currenciesRepository);

			void SetAccountId(int id);
			std::vector<std::shared_ptr<Currency>> GetCurrencies();			
			std::vector<wxString> GetTypes();
			void SetName(wxString name);
			wxString GetName();
			void SetType(AccountType type);
			AccountType GetType();
			void SetAmount(float amount);
			float GetAmount();			
			std::shared_ptr<Currency> GetCurrency();
			void SetCurrency(std::shared_ptr<Currency> currency);
			void SetNote(wxString note);
			wxString GetNote();
			void SetIconIndex(int index);
			int GetIconIndex();
			int GetCurrencyIndex();

			void Save();
			std::function<void()> OnUpdate;

		private:
			AccountsService& _accountsService;
			CurrenciesRepository& _currenciesRepository;
			int _id;
			wxString _name;
			AccountType _type;
			int _iconId;
			std::vector<std::shared_ptr<Currency>> _currencies;
			std::shared_ptr<Currency> _currency;
			float _amount;
			wxString _note;
			std::vector<wxString> _types;

			void Update();
		};
	}
}