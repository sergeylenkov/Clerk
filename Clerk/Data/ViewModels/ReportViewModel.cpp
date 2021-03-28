#include "ReportViewModel.h"

using namespace Clerk::Data;

ReportViewModel::ReportViewModel(ReportModel& report) {
	this->id = report.id;
	this->name = wxString::FromUTF8(report.name.c_str());
}