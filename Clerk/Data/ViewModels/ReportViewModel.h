#pragma once

#include <wx/wx.h>
#include "../Models/ReportModel.h"

namespace Clerk {
	namespace Data {
		class ReportViewModel {
		public:
			ReportViewModel(ReportModel& report);

			int id;
			wxString name;
		};
	}
}