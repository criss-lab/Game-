#if WITH_DEV_AUTOMATION_TESTS
#include "Misc/AutomationTest.h"
#include "SPPlayMode.h"
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSPPlayModeValidationTest, "ShadowProtocol.PlayMode.Validation", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FSPPlayModeValidationTest::RunTest(const FString& Parameters)
{
 FSPPlaySelection Selection;
 TestTrue(TEXT("Offline story allowed"), Selection.IsValid());
 Selection.Mode = ESPPlayMode::SoloVsAI;
 TestTrue(TEXT("Offline AI allowed"), Selection.IsValid());
 Selection.Mode = ESPPlayMode::CopsAndThieves;
 TestTrue(TEXT("Offline team practice allowed"), Selection.IsValid());
 Selection.Mode = ESPPlayMode::PrivateDuel;
 TestFalse(TEXT("Offline private duel rejected"), Selection.IsValid());
 Selection.Connectivity = ESPConnectivity::Online;
 TestTrue(TEXT("Online private duel allowed"), Selection.IsValid());
 return true;
}
#endif
