#pragma once

#include <wx/wx.h>
#include <wx/numformatter.h>
#include "../Data/Currency.h"

class Utils
{
public:
	Utils();
	~Utils();

	static wxString FormatAmount(float amount);
	static wxString FormatAmount(float amount, Currency *currency);
	static wxColor ColorForBudget(int percent);
	static wxColor ColorForGoal(int percent);
	static wxColor ColorForDebt(int percent);
};

