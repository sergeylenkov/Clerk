#pragma once

#include <wx/wx.h>
#include "../Models/CurrencyModel.h"

namespace Clerk {
	namespace Data {
		class CurrencyPresentationModel {
		public:
			CurrencyPresentationModel();
			CurrencyPresentationModel(CurrencyModel& model);

			int id;
			wxString name;
			wxString shortName;
			wxString sign;
		};
	}
}