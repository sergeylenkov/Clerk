#pragma once

#include <string>
#include <memory>
#include "Model.h"
#include "../Enums.h"

namespace Clerk {
	namespace Data {		
		class AccountModel : public Model
		{
		public:
			AccountModel();

			std::wstring name;
			std::wstring note;
			AccountType type;
			int iconId;
			int orderId;
			int currencyId;
			float creditLimit;
			bool isActive;
			std::wstring created;
		};
	}
}