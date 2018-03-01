#include "Settings.h"

void Settings::Open(char *configName) {
	wxFileName path(wxStandardPaths::Get().GetUserDataDir(), configName);
	config = new wxFileConfig("", "", path.GetFullPath(), "", wxCONFIG_USE_LOCAL_FILE);

	selectedAccountId = config->ReadLong("SelectedAccount", -1);
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
}

void Settings::Save() {
	config->SetPath("/");

	config->Write("SelectedAccount", selectedAccountId);
	config->Write("WindowWidth", windowWidth);
	config->Write("WindowHeight", windowHeight);
	config->Write("FromPeriodDate", fromPeriodDate.FormatISODate());
	config->Write("ToPeriodDate", toPeriodDate.FormatISODate());

	config->DeleteEntry("/Tabs");

	for (unsigned int i = 0; i < tabs.size(); i++) {
		wxString key = wxString::Format(wxT("/Tabs/Tab%i/"), i);

		config->Write(key + "Type", tabs[i].type);
		config->Write(key + "Id", tabs[i].id);
	}

	config->Flush();
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

void Settings::AddTab(int type, int id) {
	TabSettings tab = { type, id };
	tabs.push_back(tab);
}

std::vector<TabSettings> Settings::GetTabs() {
	return tabs;
}