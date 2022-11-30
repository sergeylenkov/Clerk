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
			std::wstring date;
			std::wstring note;
			std::wstring created;
			std::vector<int> tagsIds;
		};
	}
}