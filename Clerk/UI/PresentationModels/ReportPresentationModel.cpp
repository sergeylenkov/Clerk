#include "ReportPresentationModel.h"

using namespace Clerk::UI;

ReportPresentationModel::ReportPresentationModel(ReportModel& report) {
	this->id = report.id;
	this->name = report.name;
}