#include "Settings.h"

void Settings::Open(char *configName) {
	wxFileName path(wxStandardPaths::Get().GetUserDataDir(), configName);
	fileName = path.GetFullPath();

	selectedAccountId = -1;
	selectedTab = 0;
	windowWidth = 1000;
	windowHeight = 800;

	RestoreDefaultColumns();	

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
			const Value &values = json["Tabs"];

			for (SizeType i = 0; i < values.Size(); i++) {
				const Value &value = values[i];

				AddTab(value["Type"].GetInt(), value["Id"].GetInt());
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

		if (json["Columns"].IsArray()) {
			const Value &types = json["Columns"];

			for (SizeType i = 0; i < types.Size(); i++) {
				const Value &type = types[i];

				int columnsType = type["Type"].GetInt();
				const Value &values = type["Columns"];

				std::vector<ListColumnsSettings> columns;

				for (SizeType i = 0; i < values.Size(); i++) {
					const Value &value = values[i];

					columns.push_back({ value["Index"].GetInt(), value["Order"].GetInt(), wxString::FromUTF8(value["Title"].GetString()), value["Width"].GetInt(), value["Sorted"].GetBool() });
				}

				columnsSettings[columnsType] = columns;
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

	Value settingsJson(kArrayType);

	for (const auto &value : columnsSettings)
	{
		auto columns = columnsSettings[value.first];

		Value columnsTypeJson(kObjectType);
		Value columnsJson(kArrayType);

		for each (auto column in columns)
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

		columnsTypeJson.AddMember("Type", value.first, json.GetAllocator());
		columnsTypeJson.AddMember("Columns", columnsJson, json.GetAllocator());

		settingsJson.PushBack(columnsTypeJson, json.GetAllocator());
	}

	json.AddMember("Columns", settingsJson, json.GetAllocator());

	FILE *fp = fopen(fileName.char_str(), "wb"); 
	char writeBuffer[65536];

	FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));
	Writer<FileWriteStream> writer(os);

	json.Accept(writer);

	fclose(fp);
}

Settings::~Settings() {
	
}

void Settings::RestoreDefaultColumns() {
	std::vector<ListColumnsSettings> columns;

	columns.push_back({ 0, 0, wxT("From Account"), 100, false });
	columns.push_back({ 1, 1, wxT("To Account"), 100, false });
	columns.push_back({ 2, 2, wxT("Tags"), 100, false });
	columns.push_back({ 3, 3, wxT("Note"), 100, false });
	columns.push_back({ 4, 4, wxT("Date"), 100, true });
	columns.push_back({ 5, 5, wxT("Amount"), 100, false });

	columnsSettings[0] = columns;

	std::vector<ListColumnsSettings> columns2;

	columns2.push_back({ 0, 0, wxT("Account"), 100, false });	
	columns2.push_back({ 1, 1, wxT("Tags"), 100, false });
	columns2.push_back({ 2, 2, wxT("Note"), 100, false });
	columns2.push_back({ 3, 3, wxT("Date"), 100, true });
	columns2.push_back({ 4, 4, wxT("Amount"), 100, false });

	columnsSettings[1] = columns;
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

std::vector<ListColumnsSettings> Settings::GetColumns(int type) {
	auto columns = columnsSettings[type];

	std::sort(columns.begin(), columns.end(), [this](const ListColumnsSettings& v1, const ListColumnsSettings& v2) {
		return v1.order < v2.order;
	});

	return columns;
}

void Settings::SetColumns(int type, std::vector<ListColumnsSettings> columns) {
	columnsSettings[type] = columns;
}