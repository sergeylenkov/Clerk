#pragma once

#include <wx/wx.h>
#include "../Models/ReportModel.h"

namespace Clerk {
	namespace Data {
		class ReportPresentationModel {
		public:
			ReportPresentationModel(ReportModel& report);

			int id;
			wxString name;
		};
	}
}