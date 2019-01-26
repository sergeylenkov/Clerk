#include "Settings.h"

void Settings::Open(char *configName) {
	wxFileName path(wxStandardPaths::Get().GetUserDataDir(), configName);
	config = new wxFileConfig("", "", path.GetFullPath(), "", wxCONFIG_USE_LOCAL_FILE);

	fileName = path.GetFullPath() + ".json";

	selectedAccountId = config->ReadLong("SelectedAccount", -1);
	selectedTab = config->ReadLong("SelectedTab", 0);
	windowWidth = config->ReadLong("WindowWidth", 1000);
	windowHeight = config->ReadLong("WindowHeight", 800);
	fromPeriodDate = wxDateTime::Now();
	toPeriodDate = wxDateTime::Now();

	fromPeriodDate.ParseISODate(config->ReadObject("FromPeriodDate", wxDateTime::Now().FormatISODate()));
	toPeriodDate.ParseISODate(config->ReadObject("ToPeriodDate", wxDateTime::Now().FormatISODate()));

	config->SetPath("/Tabs");

	long index;
	wxString key;

	bool entry = config->GetFirstGroup(key, index);

	while (entry)
	{
		int type = config->ReadLong(key + "/Type", -1);
		int accountId = config->ReadLong(key + "/Id", -1);

		if (type != -1) {
			AddTab(type, accountId);
		}

		entry = config->GetNextGroup(key, index);
	}

	config->SetPath("/ExpandedMenu");

	entry = config->GetFirstEntry(key, index);

	while (entry)
	{
		int type = config->ReadLong(key, -1);

		if (type != -1) {
			expandedMenu[type] = true;
		}

		entry = config->GetNextEntry(key, index);
	}
}

void Settings::Save() {
	config->SetPath("/");

	config->Write("SelectedAccount", selectedAccountId);
	config->Write("WindowWidth", windowWidth);
	config->Write("WindowHeight", windowHeight);
	config->Write("FromPeriodDate", fromPeriodDate.FormatISODate());
	config->Write("ToPeriodDate", toPeriodDate.FormatISODate());
	config->Write("SelectedTab", selectedTab);

	config->DeleteGroup("/Tabs");

	for (unsigned int i = 0; i < tabs.size(); i++) {
		wxString key = wxString::Format(wxT("/Tabs/Tab%i/"), i);

		config->Write(key + "Type", tabs[i].type);
		config->Write(key + "Id", tabs[i].id);
	}

	config->DeleteGroup("/ExpandedMenu");

	int i = 0;

	for (const auto &value : expandedMenu)
	{
		if (value.second) {
			wxString key = wxString::Format(wxT("/ExpandedMenu/Item%i"), i);
			config->Write(key, value.first);

			i++;
		}
	}

	config->Flush();

	Document json;

	json.SetObject();

	json.AddMember("WindowWidth", windowWidth, json.GetAllocator());
	json.AddMember("WindowHeight", windowHeight, json.GetAllocator());
	json.AddMember("SelectedAccount", selectedAccountId, json.GetAllocator());
	json.AddMember("SelectedTab", selectedTab, json.GetAllocator());

	FILE *fp = fopen(fileName.char_str(), "wb"); 
	char writeBuffer[65536];

	FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));
	Writer<FileWriteStream> writer(os);

	json.Accept(writer);

	fclose(fp);
}

Settings::~Settings() {
	config->Flush();
	delete config;
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

wxDateTime Settings::GetFromPeriodDate() {
	return fromPeriodDate;
}

wxDateTime Settings::GetToPeriodDate() {
	return toPeriodDate;
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

void Settings::SetFromPeriodDate(wxDateTime date) {
	fromPeriodDate = date;
}

void Settings::SetToPeriodDate(wxDateTime date) {
	toPeriodDate = date;
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

	for each (auto settings in filterSettings)
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

	for each (auto settings in filterSettings)
	{
		if (settings.type == type && settings.id == id) {
			return settings;
		}
	}

	return result;
}