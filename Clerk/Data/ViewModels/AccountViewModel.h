#pragma once

#include <wx/wx.h>
#include "../Models/Currency.h"
#include "../Models/AccountModel.h"
#include "../Enums.h"

namespace Clerk {
	namespace Data {
		class AccountViewModel {
		public:
			AccountViewModel();
			AccountViewModel(AccountModel& account);

			int id;
			wxString name;
			wxString note;
			AccountType type;
			int icon;
			int order;
			float creditLimit;
			bool isCredit = false;
			std::shared_ptr<Currency> currency;
			float balance;
			float expenses;
			float receipts;
			boolean isActive = false;
			wxDateTime date;

			operator AccountModel& ();

		private:
			AccountModel* GetModel();
		};
	}
}