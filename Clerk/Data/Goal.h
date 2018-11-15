#pragma once

#include <wx/wx.h>
#include <memory>
#include "Model.h"

using namespace std;

class Goal : public Model
{
public:
	int id;
	shared_ptr<wxString> name;
	float amount;
	shared_ptr<wxDateTime> date;
	shared_ptr<wxDateTime> createdDate;
	shared_ptr<wxString> accountIds;

	Goal();
	Goal(int id);
	void Load();
	void Save();
	void Delete();	
};

