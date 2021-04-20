#pragma once

#include "../Models/Currency.h"

namespace Clerk {
	namespace Data {
		struct CurrencyValueViewModel {
			Currency currency;
			float value {};
		};
	}
}