#include "Report.h"

Report::Report()
{
	this->id = -1;
	this->name = make_shared<wxString>();
	this->note = make_shared<wxString>();
}

Report::Report(int id) : Report()
{
	this->id = id;
	this->name = make_shared<wxString>();
	this->note = make_shared<wxString>();
}