#include "TagPresentationModel.h"

using namespace Clerk::UI;

TagPresentationModel::TagPresentationModel() {
	this->id = -1;
	this->name = "";
	this->count = 0;
}

TagPresentationModel::TagPresentationModel(TagModel& tag) {
	this->id = tag.id;
	this->name = wxString(tag.name);
	this->count = 0;
}