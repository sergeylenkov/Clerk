#pragma once

#include <wx/wx.h>
#include "CurrencyPresentationModel.h"
#include "../Models/AccountModel.h"
#include "../Enums.h"

namespace Clerk {
	namespace Data {
		class AccountPresentationModel {
		public:
			AccountPresentationModel();
			AccountPresentationModel(AccountModel& model);

			int id;
			wxString name;
			wxString note;
			AccountType type;
			int icon;
			int order;
			float creditLimit;
			bool isCredit = false;
			std::shared_ptr<CurrencyPresentationModel> currency;
			float balance;
			float expenses;
			float receipts;
			boolean isActive = false;
			wxDateTime date;

			operator AccountModel& ();
				
		private:
			AccountModel& GetModel();
		};
	}
}