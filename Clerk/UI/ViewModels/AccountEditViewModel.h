#pragma once

#include "../PresentationModels/CurrencyPresentationModel.h"
#include "../../Data/Services/AccountsService.h"
#include "../../Data/Services/CurrenciesService.h"

namespace Clerk {
	namespace UI {
		class AccountEditViewModel {
		public:
			AccountEditViewModel(AccountsService& accountsService, CurrenciesService& currenciesService);

			void SetAccountId(int id);
			std::vector<std::shared_ptr<CurrencyPresentationModel>> GetCurrencies();
			std::vector<wxString> GetTypes();
			void SetName(wxString name);
			wxString GetName();
			void SetType(AccountType type);
			AccountType GetType();
			void SetAmount(float amount);
			float GetAmount();			
			std::shared_ptr<CurrencyPresentationModel> GetCurrency();
			void SetCurrency(std::shared_ptr<CurrencyPresentationModel> currency);
			void SetNote(wxString note);
			wxString GetNote();
			void SetIconIndex(int index);
			int GetIconIndex();
			int GetCurrencyIndex();

			void Save();
			std::function<void()> OnUpdate;

		private:
			AccountsService& _accountsService;
			CurrenciesService& _currenciesService;
			int _id;
			wxString _name;
			AccountType _type;
			int _iconId;
			std::vector<std::shared_ptr<CurrencyPresentationModel>> _currencies;
			std::shared_ptr<CurrencyPresentationModel> _currency;
			float _amount;
			wxString _note;
			std::vector<wxString> _types;

			void Update();
		};
	}
}