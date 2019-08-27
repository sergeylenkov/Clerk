#pragma once

#include <wx/wx.h>
#include <wx/numformatter.h>
#include "../Data/Currency.h"

enum class PeriodTypes {
	CurrentWeek = 1,
	PreviousWeek = 2,
	CurrentMonth = 3,
	PreviousMonth = 4,
	CurrentYear = 5,
	PreviousYear = 6
};


class Utils
{
public:
	Utils();
	~Utils();

	static wxString FormatAmount(float amount);
	static wxString FormatAmount(float amount, Currency &currency);
	static wxColor ColorForBudget(int percent);
	static wxColor ColorForGoal(int percent);
	static wxColor ColorForDebt(int percent);
	static void CalculatePeriod(PeriodTypes type, wxDateTime &fromDate, wxDateTime &toDate);
};

