#pragma once

#include <wx/wx.h>
#include <memory>
#include "Model.h"

using namespace std;

class Tag : public Model
{
public:
	Tag();
	Tag(int id);

	int id;
	shared_ptr<wxString> name;
	int count;

	void Load();
	void Save();
	void Delete();
};

