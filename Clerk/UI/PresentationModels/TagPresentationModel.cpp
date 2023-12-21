#include "TagPresentationModel.h"

using namespace Clerk::UI;

TagPresentationModel::TagPresentationModel() {
	id = -1;
	name = "";
	count = 0;
}

TagPresentationModel::TagPresentationModel(TagModel& tag) {
	id = tag.id;
	name = wxString(tag.name);
	count = 0;
}

TagPresentationModel::operator TagModel& () {
	return GetModel();
}

TagModel& TagPresentationModel::GetModel() {
	TagModel* model = new TagModel();

	model->id = id;
	model->name = name;
	
	return *model;
}