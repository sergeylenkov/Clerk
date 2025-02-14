#pragma once

namespace Clerk {
	namespace UI {
		enum class AccountViewModelField {
			Name = 0,
			Type = 1,
			Icon = 2,
			InitialAmount = 3,
			Note = 4,
			Currency = 5,
			CreditLimit = 6,
		};
	}
}

namespace Clerk {
	namespace UI {
		enum class InterestAccrualViewModelField {
			Amount = 0,
			Percent = 1,
			Type = 2
		};
	}
}

namespace Clerk {
	namespace UI {
		enum class AccrualType {
			Amount = 0,
			Percent = 1
		};
	}
}