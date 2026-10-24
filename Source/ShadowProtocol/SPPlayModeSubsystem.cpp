#include "SPPlayModeSubsystem.h"
bool USPPlayModeSubsystem::SelectMode(FSPPlaySelection Requested)
{
 if (!Requested.IsValid()) return false;
 Selection = Requested;
 return true;
}
