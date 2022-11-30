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

			std::wstring name;
			AlertType type;
			AlertPeriod period;
			AlertCondition condition;
			float amount;
			std::wstring accountIds;
			std::wstring created;
		};
	}
}
