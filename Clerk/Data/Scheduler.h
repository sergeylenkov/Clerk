#pragma once

#include <wx/wx.h>
#include <memory>
#include "Model.h"

using namespace std;

class Scheduler : public Model
{
public:
	int id;
	shared_ptr<wxString> name;	

	Scheduler();
	Scheduler(int id);
	void Load();
	void Save();
	void Delete();
};

