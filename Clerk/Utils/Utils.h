#pragma once

#include <wx/wx.h>
#include <wx/numformatter.h>
#include "../Data/Currency.h"

class Utils
{
public:
	Utils();
	~Utils();

	static wxString FormatAmount(float amount, Currency *currency);
};

