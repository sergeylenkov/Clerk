#include "Settings.h"

void Settings::Open(char *configName) {
	wxFileName path(wxStandardPaths::Get().GetUserDataDir(), configName);
	fileName = path.GetFullPath();

	selectedAccountId = -1;
	selectedTab = 0;
	windowWidth = 1000;
	windowHeight = 800;

	columnsAccounts.push_back({ 0, 0, wxT("From Account"), 200, false });
	columnsAccounts.push_back({ 1, 1, wxT("To Account"), 200, false });
	columnsAccounts.push_back({ 2, 2, wxT("Tags"), 200, false });
	columnsAccounts.push_back({ 3, 3, wxT("Note"), 200, false });
	columnsAccounts.push_back({ 4, 4, wxT("Date"), 200, true });
	columnsAccounts.push_back({ 5, 5, wxT("Amount"), 200, false });

	if (wxFile::Exists(fileName)) {
		FILE *fp = fopen(fileName.char_str(), "rb");
		char readBuffer[65536];
		FileReadStream is(fp, readBuffer, sizeof(readBuffer));

		Document json;
		json.ParseStream(is);

		fclose(fp);

		if (json["SelectedAccount"].IsInt()) {
			selectedAccountId = json["SelectedAccount"].GetInt();
		}

		if (json["SelectedTab"].IsInt()) {
			selectedTab = json["SelectedTab"].GetInt();
		}

		if (json["WindowWidth"].IsInt()) {
			windowWidth = json["WindowWidth"].GetInt();
		}

		if (json["WindowHeight"].IsInt()) {
			windowHeight = json["WindowHeight"].GetInt();
		}

		if (json["ExpandedMenu"].IsArray()) {
			const Value &array = json["ExpandedMenu"];
			
			for (SizeType i = 0; i < array.Size(); i++) {
				expandedMenu[array[i].GetInt()] = true;
			}
		}

		if (json["Tabs"].IsArray()) {
			const Value &array = json["Tabs"];

			for (SizeType i = 0; i < array.Size(); i++) {
				const Value &tab = array[i];

				AddTab(tab["Type"].GetInt(), tab["Id"].GetInt());
			}
		}

		if (json["Filters"].IsArray()) {
			const Value &array = json["Filters"];

			for (SizeType i = 0; i < array.Size(); i++) {
				const Value &filter = array[i];

				wxDateTime fromDate = wxDateTime::Now();
				wxDateTime toDate = wxDateTime::Now();

				fromDate.ParseISODate(wxString::FromUTF8(filter["FromDate"].GetString()));
				toDate.ParseISODate(wxString::FromUTF8(filter["ToDate"].GetString()));
			}
		}

		if (json["ColumnsAccounts"].IsArray()) {
			columnsAccounts.clear();

			const Value &array = json["ColumnsAccounts"];

			for (SizeType i = 0; i < array.Size(); i++) {
				const Value &column = array[i];

				columnsAccounts.push_back({ column["Index"].GetInt(), column["Order"].GetInt(),wxString::FromUTF8(column["Title"].GetString()), column["Width"].GetInt(), column["Sorted"].GetBool() });
			}
		}
	}
}

void Settings::Save() {
	Document json;

	json.SetObject();

	json.AddMember("WindowWidth", windowWidth, json.GetAllocator());
	json.AddMember("WindowHeight", windowHeight, json.GetAllocator());
	json.AddMember("SelectedAccount", selectedAccountId, json.GetAllocator());
	json.AddMember("SelectedTab", selectedTab, json.GetAllocator());	

	Value menuJson(kArrayType);

	for (const auto &value : expandedMenu)
	{
		if (value.second) {
			menuJson.PushBack(value.first, json.GetAllocator());
		}
	}

	json.AddMember("ExpandedMenu", menuJson, json.GetAllocator());

	if (tabs.size() > 0) {
		Value tabsJson(kArrayType);

		for each (auto tab in tabs) {
			Value tabJson(kObjectType);

			tabJson.AddMember("Type", tab.type, json.GetAllocator());
			tabJson.AddMember("Id", tab.id, json.GetAllocator());

			tabsJson.PushBack(tabJson, json.GetAllocator());
		}

		json.AddMember("Tabs", tabsJson, json.GetAllocator());
	}

	if (filterSettings.size() > 0) {
		Value settingsJson(kArrayType);

		for each (auto settings in filterSettings)
		{
			Value filterJson(kObjectType);

			filterJson.AddMember("Type", settings.type, json.GetAllocator());
			filterJson.AddMember("Id", settings.id, json.GetAllocator());
			filterJson.AddMember("Period", settings.period, json.GetAllocator());

			wxString dateString = settings.fromDate.FormatISODate();			
			Value string(dateString.c_str(), json.GetAllocator());

			filterJson.AddMember("FromDate", string, json.GetAllocator());

			dateString = settings.toDate.FormatISODate();			
			string.SetString(dateString.c_str(), json.GetAllocator());

			filterJson.AddMember("ToDate", string, json.GetAllocator());

			settingsJson.PushBack(filterJson, json.GetAllocator());
		}

		json.AddMember("Filters", settingsJson, json.GetAllocator());
	}

	Value columnsJson(kArrayType);

	for each (auto column in columnsAccounts)
	{
		Value columnJson(kObjectType);

		Value string(column.title.c_str(), json.GetAllocator());

		columnJson.AddMember("Index", column.index, json.GetAllocator());
		columnJson.AddMember("Order", column.order, json.GetAllocator());
		columnJson.AddMember("Title", string, json.GetAllocator());
		columnJson.AddMember("Width", column.width, json.GetAllocator());
		columnJson.AddMember("Sorted", column.sorted, json.GetAllocator());

		columnsJson.PushBack(columnJson, json.GetAllocator());
	}

	json.AddMember("ColumnsAccounts", columnsJson, json.GetAllocator());

	FILE *fp = fopen(fileName.char_str(), "wb"); 
	char writeBuffer[65536];

	FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));
	Writer<FileWriteStream> writer(os);

	json.Accept(writer);

	fclose(fp);
}

Settings::~Settings() {
	
}

int Settings::GetSelectedAccountId() {
	return selectedAccountId;
}

int Settings::GetWindowWidth() {
	return windowWidth;
}

int Settings::GetWindowHeight() {
	return windowHeight;
}

void Settings::SetSelectedAccountId(int id) {
	selectedAccountId = id;
}

void Settings::SetWindowWidth(int width) {
	windowWidth = width;
}

void Settings::SetWindowHeight(int height) {
	windowHeight = height;
}

void Settings::ClearTabs() {
	tabs.clear();
}

void Settings::AddTab(int type, int id) {
	TabSettings tab = { type, id };
	tabs.push_back(tab);
}

std::vector<TabSettings> Settings::GetTabs() {
	return tabs;
}

void Settings::SetSelectedTab(int id) {
	selectedTab = id;
}

int Settings::GetSelectedTab() {
	return selectedTab;
}

void Settings::AddExpandedMenu(int type) {
	expandedMenu[type] = true;
}

void Settings::RemoveExpandedMenu(int type) {
	expandedMenu[type] = false;
}

bool Settings::IsMenuExpanded(int type) {
	if (expandedMenu.find(type) != expandedMenu.end()) {
		return expandedMenu[type];
	}

	return false;
}

void Settings::SetListFilterSettings(int type, int id, int period, wxDateTime fromDate, wxDateTime toDate) {
	bool found = false;

	for (auto &settings : filterSettings)
	{		
		if (settings.type == type && settings.id == id) {
			settings.period = period;
			settings.fromDate = fromDate;
			settings.toDate = toDate;

			found = true;
			break;
		}
	}

	if (!found) {
		filterSettings.push_back({ type, id, period, fromDate, toDate });
	}
}

ListFilterSettings Settings::GetListFilterSettings(int type, int id) {
	wxDateTime fromDate = wxDateTime::Now();
	wxDateTime toDate = wxDateTime::Now();

	fromDate.SetDay(1);
	toDate.SetToLastMonthDay();

	ListFilterSettings result = { 0, 0, 3, fromDate, toDate };

	for (auto &settings : filterSettings)
	{
		if (settings.type == type && settings.id == id) {
			return settings;
		}
	}

	return result;
}

std::vector<ListColumnsSettings> Settings::GetColumns(TreeMenuItemTypes type) {
	if (type != TreeMenuItemTypes::MenuAccount) {
		return columnsAccounts;
	}

	return columnsAccounts;
}

void Settings::SetColumns(TreeMenuItemTypes type, std::vector<ListColumnsSettings> settings) {
	columnsAccounts = settings;
}