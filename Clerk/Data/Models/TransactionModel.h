#pragma once

#include <string>
#include <vector>
#include "Model.h"

namespace Clerk {
	namespace Data {
		class TransactionModel : public Model
		{
		public:
			TransactionModel();

			int toAccountId;
			int fromAccountId;
			float fromAmount;
			float toAmount;
			std::string date;
			std::string note;
			std::string created;
			std::vector<int> tagsIds;
		};
	}
}