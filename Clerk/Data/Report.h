#ifndef Report_h
#define Report_h

#include <wx/wx.h>
#include <memory>
#include "Model.h"
#include "Currency.h"

using namespace std;

class Report : public Model
{
public:
	int id;
	shared_ptr<wxString> name;
	shared_ptr<wxString> note;

	Report();
	Report(int id);
};

#endif
