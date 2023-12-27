#include "ReportPresentationModel.h"

using namespace Clerk::Data;

ReportPresentationModel::ReportPresentationModel(ReportModel& report) {
	this->id = report.id;
	this->name = report.name;
}