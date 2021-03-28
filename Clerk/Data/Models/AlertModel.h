#pragma once

#include <string>
#include "Model.h"
#include "../Enums.h"

namespace Clerk {
	namespace Data {
		class AlertModel : public Model
		{
		public:
			AlertModel();

			std::string name;
			AlertType type;
			AlertPeriod period;
			AlertCondition condition;
			float amount;
			std::string accountIds;
			std::string created;
		};
	}
}
