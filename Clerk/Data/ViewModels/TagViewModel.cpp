#include "TagViewModel.h"

using namespace Clerk::Data;

TagViewModel::TagViewModel(TagModel& tag) {
	this->id = tag.id;
	this->name = wxString::FromUTF8(tag.name.c_str());
	this->count = 0;
}