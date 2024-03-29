#pragma once

#include "../../Data/Services/AccountsService.h"
#include "../../Data/Services/CurrenciesService.h"
#include "../../Utils/Types.h"
#include "Enums.h"

using namespace Clerk::Data;

namespace Clerk {
	namespace UI {
		class AccountViewModel {
		public:
			AccountViewModel(AccountsService& accountsService, CurrenciesService& currenciesService);

			void SetAccountId(int id);
			void SetAccountType(AccountType type);
			bool IsNew();
			shared_vector<CurrencyPresentationModel> GetCurrencies();
			std::vector<wxString> GetTypes();
			void SetName(wxString name);
			wxString GetName();
			void SetType(AccountType type);
			AccountType GetType();
			void SetAmount(float amount);
			float GetAmount();	
			void SetCreditLimit(float amount);
			float GetCreditLimit();
			void SetCurrency(std::shared_ptr<CurrencyPresentationModel> currency);
			std::shared_ptr<CurrencyPresentationModel> GetCurrency();			
			void SetNote(wxString note);
			wxString GetNote();
			void SetIconIndex(int index);
			int GetIconIndex();
			int GetCurrencyIndex();

			void Save();
			std::function<void(AccountViewModelField field)> OnUpdate;

		private:
			AccountsService& _accountsService;
			CurrenciesService& _currenciesService;
			int _id;
			wxString _name;
			AccountType _type;
			int _iconId;
			shared_vector<CurrencyPresentationModel> _currencies;
			std::shared_ptr<CurrencyPresentationModel> _currency;
			float _amount;
			float _creditLimit;
			wxString _note;
			std::vector<wxString> _types;
		};
	}
}