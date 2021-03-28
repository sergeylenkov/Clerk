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

			std::string name;
			float amount;
			BudgetPeriod period;
			std::string date;
			std::string created;
			std::string accountIds;
		};
	}
}