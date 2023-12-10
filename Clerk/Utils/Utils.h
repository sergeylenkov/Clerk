#pragma once

#include <wx/wx.h>
#include <wx/numformatter.h>
#include <iostream>
#include <vector>
#include <numeric>

namespace Clerk {
	namespace Utils {
		class Colors
		{
		public:			
			static wxColor ColorForBudget(int percent);
			static wxColor ColorForGoal(int percent);
			static wxColor ColorForDebt(int percent);
			static wxColor ColorForBalance(boolean isReceipt);
		};

		class Format {
		public:
			static wxString Amount(float amount);
			static wxString Amount(float amount, wxString& sign);
		};

		class Periods {
		public:
			enum class Type {
				CurrentWeek = 1,
				PreviousWeek = 2,
				CurrentMonth = 3,
				PreviousMonth = 4,
				CurrentYear = 5,
				PreviousYear = 6
			};

			static void Calculate(Periods::Type type, wxDateTime& fromDate, wxDateTime& toDate);
		};

		class String {
		public:
			static std::string Split(std::vector<std::string> const& vec, std::string delim);
		};
	}
}
