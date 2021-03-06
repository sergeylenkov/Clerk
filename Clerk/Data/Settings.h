#pragma once

#include <wx/wx.h>
#include <wx/filename.h>
#include <wx/stdpaths.h>
#include <map>
#include "rapidjson/document.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/filereadstream.h"
#include <rapidjson/writer.h>
#include "../Utils/Utils.h"

using namespace rapidjson;
using namespace Clerk::Utils;

struct TabSettings {
	int type;
	int id;
};

struct ListColumnsSettings {
	int index;
	int order;
	wxString title;
	int width;
	bool sorted;
};

struct ListFilterSettings {	
	int type;
	int id;
	int period;
	wxDateTime fromDate;
	wxDateTime toDate;
};

enum class ListColumnsTypes {
	All = 0,
	Receipts = 1,
	Deposits = 2,
	Expenses = 3,	
};

struct ReportFilterSettings {	
	int id;
	wxString accountIds;
	int period;
	wxDateTime fromDate;
	wxDateTime toDate;
	bool average;
};

class Settings
{
public:
	~Settings();

	static Settings& GetInstance() {
		static Settings instance;
		return instance;
	}

	void Open(char *configName);
	void Save();

	int GetSelectedAccountId();
	void SetSelectedAccountId(int id);
	int GetWindowWidth();
	int GetWindowHeight();		
	void SetWindowWidth(int width);
	void SetWindowHeight(int height);
	int GetBaseCurrencyId();
	void SetBaseCurrencyId(int id);
	bool IsConvertCurrency();
	void SetConvertCurrency(bool convert);
	bool IsLoadExchangeRates();
	void SetLoadExchangeRates(bool load);
	std::vector<int> GetSelectedExchangeRates();
	void ClearTabs();
	void AddTab(int type, int accountId);
	std::vector<TabSettings> GetTabs();
	void SetSelectedTab(int id);
	int GetSelectedTab();
	void AddExpandedMenu(int type);
	void RemoveExpandedMenu(int type);
	bool IsMenuExpanded(int type);	
	void SetListFilterSettings(int type, int id, int period, wxDateTime fromDate, wxDateTime toDate);
	ListFilterSettings GetListFilterSettings(int type, int id);
	std::vector<ListColumnsSettings> GetTransactionsListColumns(ListColumnsTypes type);
	void SetTransactionsListColumns(ListColumnsTypes type, std::vector<ListColumnsSettings> columns);
	ReportFilterSettings GetReportFilterSettings(int id);
	void SetReportFilterSettings(int id, wxString accountIds, int period, wxDateTime fromDate, wxDateTime toDate);
	void SetReportFilterSettings(int id, wxString accountIds, int period, wxDateTime fromDate, wxDateTime toDate, bool average);

private:
	wxString fileName;
	int selectedAccountId;
	int windowWidth;
	int windowHeight;
	int baseCurrencyId;
	bool convertCurrency;
	bool loadExchangeRates;
	std::vector<int> selectedExchangeRates;
	std::vector<TabSettings> tabs;
	std::map<int, bool> expandedMenu;
	int selectedTab;
	std::vector<ListFilterSettings> transactionListFilterSettings;
	std::map<int, std::vector<ListColumnsSettings>> transactionsListColumnsSettings;
	std::vector<ReportFilterSettings> reportFilterSettings;

	void RestoreDefaultColumns();
};