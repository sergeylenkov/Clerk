#pragma once

#include <wx/wx.h>
#include "../../Data/Models/CurrencyModel.h"

using namespace Clerk::Data;

namespace Clerk {
	namespace UI {
		class CurrencyPresentationModel {
		public:
			CurrencyPresentationModel();
			CurrencyPresentationModel(CurrencyModel& tag);

			int id;
			wxString name;
			wxString shortName;
			wxString sign;
		};
	}
}