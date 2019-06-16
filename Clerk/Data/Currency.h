#ifndef Currency_h
#define Currency_h

#include <wx/wx.h>
#include <memory>
#include "Model.h"

using namespace std;

class Currency : public Model
{
public:
	Currency();
	Currency(int id);

	int id;
	shared_ptr<wxString> name;
	shared_ptr<wxString> shortName;
	shared_ptr<wxString> sign;

	void Load();
	void Save();
};

#endif