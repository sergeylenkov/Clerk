#pragma once

#include <wx/wx.h>
#include "wx/dataview.h"
#include "../../Utils/Utils.h"

using namespace Clerk::Utils;

class DataViewProgressRender : public wxDataViewCustomRenderer
{
public:
	DataViewProgressRender(bool isReverse);

	virtual bool Render(wxRect rect, wxDC *dc, int state) wxOVERRIDE;
	virtual wxSize GetSize() const wxOVERRIDE;
	virtual bool SetValue(const wxVariant &value) wxOVERRIDE;
	virtual bool GetValue(wxVariant &WXUNUSED(value)) const wxOVERRIDE;

private:
	float _value{};
	bool _isReverse;
};