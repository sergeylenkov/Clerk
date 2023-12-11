#include "AlertModel.h"

using namespace Clerk::Data;

AlertModel::AlertModel()
{
	id = -1;
	name = std::wstring();
	message = std::wstring();
	type = AlertType::Balance;
	period = AlertPeriod::Month;
	condition = AlertCondition::Equal;
	importance = AlertImportance::Low;
	amount = 0;	
	accountIds = std::wstring();
	created = std::wstring();
}