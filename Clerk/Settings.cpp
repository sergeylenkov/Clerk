#include "Settings.h"

void Settings::Open(char *configName) {
	wxFileName path(wxStandardPaths::Get().GetUserDataDir(), configName);
	config = new wxFileConfig("", "", path.GetFullPath(), "", wxCONFIG_USE_LOCAL_FILE);

	selectedAccountId = config->ReadLong("SelectedAccount", -1);
	windowWidth = config->ReadLong("WindowWidth", 1000);
	windowHeight = config->ReadLong("WindowHeight", 800);
	fromPeriodDate = wxDateTime::Now(); //config->ReadObject("FromPeriodDate", wxDateTime::Now());
	toPeriodDate = wxDateTime::Now(); // config->ReadObject("ToPeriodDate", wxDateTime::Now());
}

void Settings::Save() {
	config->Write("SelectedAccount", selectedAccountId);
	config->Write("WindowWidth", windowWidth);
	config->Write("WindowHeight", windowHeight);
	config->Write("FromPeriodDate", fromPeriodDate.FormatISODate());
	config->Write("ToPeriodDate", toPeriodDate.FormatISODate());

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

void Settings::SetSelectedAccountId(int id) {
	selectedAccountId = id;
}

void Settings::SetWindowWidth(int width) {
	windowWidth = width;
}

void Settings::SetWindowHeight(int height) {
	windowHeight = height;
}