#pragma once

#include <wx/wx.h>
#include <wx/filename.h>
#include <wx/stdpaths.h>
#include <map>
#include "rapidjson/document.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/filereadstream.h"
#include <rapidjson/writer.h>
#include "TreeMenuItemData.h"

using namespace rapidjson;

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
	std::vector<ListColumnsSettings> GetColumns(int type);
	void SetColumns(int type, std::vector<ListColumnsSettings> columns);

private:
	wxString fileName;
	int selectedAccountId;
	int windowWidth;
	int windowHeight;	
	std::vector<TabSettings> tabs;
	std::map<int, bool> expandedMenu;
	int selectedTab;
	std::vector<ListFilterSettings> filterSettings;
	std::map<int, std::vector<ListColumnsSettings>> columnsSettings;

	void RestoreDefaultColumns();
};