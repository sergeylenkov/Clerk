#pragma once

#include <wx/wx.h>
#include <wx/numformatter.h>
#include <iostream>
#include <vector>
#include <numeric>
#include <sstream>
#include <set>
#include "../Data/Enums.h"

using namespace Clerk::Data;

namespace Clerk {
	namespace Utils {
		class Colors
		{
		public:			
			static wxColor ColorForProgress(int percent, bool isReverse);
			static wxColor ColorForBalance(boolean isReceipt);
			static wxColor ColorForImportance(AlertImportance importance);
			static wxColor ColorForBarIndex(int index);
		};

		class Format {
		public:
			static wxString Amount(float amount);
			static wxString Amount(float amount, wxString& sign);
			static wxString Date(const wxDateTime& date);
			static wxString DaysRemain(wxDateTime& date);
		};

		class Periods {
		public:
			enum class Type {
				CurrentWeek = 1,
				PreviousWeek = 2,
				CurrentMonth = 3,
				PreviousMonth = 4,
				ThreeMonths = 5,
				SixMonths = 6,
				CurrentYear = 7,
				PreviousYear = 8
			};

			static void Calculate(Periods::Type type, wxDateTime& fromDate, wxDateTime& toDate);
		};

		class String {
		public:
			static std::vector<int> Split(std::wstring string, char delimiter);
			static std::string Join(std::vector<std::string> const& vec, std::string delimiter);
			static std::string Join(std::vector<int> const& vec, std::string delimiter);
			static std::string Join(std::set<int> const& set, std::string delimiter);
			static bool Search(wxString str, wxString search);
		};
	}
}
