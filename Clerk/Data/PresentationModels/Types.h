#pragma once

#include <wx/wx.h>
#include <wx/datetime.h>
#include "CurrencyPresentationModel.h"

namespace Clerk {
	namespace Data {
		struct DashboardProgressValue
		{
			wxString name;
			wxString amount;
			wxString currentAmount;
			wxString remainAmount;
			float percent;
		};

		struct StringValueViewModel {
			wxString string;
			float value;
		};

		struct DateValueViewModel {
			wxDateTime date;
			float value;
		};

		struct CurrencyValueViewModel {
			CurrencyPresentationModel currency;
			float value{};
		};
	}
}