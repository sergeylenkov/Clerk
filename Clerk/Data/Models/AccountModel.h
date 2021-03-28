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

			std::string name;
			std::string note;
			AccountType type;
			int iconId;
			int orderId;
			int currencyId;
			float creditLimit;
			bool isActive;
			std::string created;
		};
	}
}