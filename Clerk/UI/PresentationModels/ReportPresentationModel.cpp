#include "ReportPresentationModel.h"

using namespace Clerk::UI;

ReportPresentationModel::ReportPresentationModel(ReportModel& report) {
	this->id = report.id;
	this->name = wxString::FromUTF8(report.name.c_str());
}