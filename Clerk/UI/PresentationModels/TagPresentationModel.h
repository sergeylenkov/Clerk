#pragma once

#include <wx/wx.h>
#include "../../Data/Models/TagModel.h"

using namespace Clerk::Data;

namespace Clerk {
	namespace UI {
		class TagPresentationModel {
		public:
			TagPresentationModel();
			TagPresentationModel(TagModel& tag);

			int id;
			wxString name;
			int count;

			std::shared_ptr<TagModel> GetModel();
		};
	}
}