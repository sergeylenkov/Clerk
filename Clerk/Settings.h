#pragma once

#include <wx/stdpaths.h>
#include <wx/fileconf.h>
#include <map>
#include "rapidjson/document.h"
#include "rapidjson/filewritestream.h"
#include <rapidjson/writer.h>

using namespace rapidjson;

struct TabSettings {
	int type;
	int id;
};

struct ListColumnsSettings {
	int index;
	std::string key;
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
	int GetWindowWidth();
	int GetWindowHeight();	
	void SetSelectedAccountId(int id);
	void SetWindowWidth(int width);
	void SetWindowHeight(int height);
	void SetFromPeriodDate(wxDateTime date);
	void SetToPeriodDate(wxDateTime date);
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

private:
	wxFileConfig *config;
	wxString fileName;
	int selectedAccountId;
	int windowWidth;
	int windowHeight;	
	std::vector<TabSettings> tabs;
	std::map<int, bool> expandedMenu;
	int selectedTab;
	std::map <std::string, int> columnsSettings;
	std::vector<ListFilterSettings> filterSettings;
};