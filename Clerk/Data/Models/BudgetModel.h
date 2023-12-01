#pragma once

#include <string>
#include <memory>
#include "Model.h"
#include "../Enums.h"

namespace Clerk {
	namespace Data {
		class BudgetModel : public Model
		{
		public:			
			BudgetModel();

			std::wstring name;
			float amount;
			BudgetPeriod period;
			std::wstring date;
			std::wstring created;
			std::wstring accountsIds;
		};
	}
}