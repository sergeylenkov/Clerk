#pragma once

#include <wx/wx.h>
#include "../Models/TagModel.h"

namespace Clerk {
	namespace Data {
		class TagPresentationModel {
		public:
			TagPresentationModel();
			TagPresentationModel(TagModel& tag);

			int id;
			wxString name;
			int count;

			operator TagModel& ();

		private:
			TagModel& GetModel();
		};
	}
}