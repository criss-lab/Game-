#include "SPIdentityComponent.h"
USPIdentityComponent::USPIdentityComponent() { PrimaryComponentTick.bCanEverTick = false; }
FString USPIdentityComponent::GenerateRandomAlias()
{
 static const TCHAR* Prefixes[] = { TEXT("Shadow"), TEXT("Ghost"), TEXT("Raven"), TEXT("Cipher"), TEXT("Nova"), TEXT("Viper"), TEXT("Echo"), TEXT("Onyx"), TEXT("Falcon"), TEXT("Specter"), TEXT("Atlas"), TEXT("Phoenix") };
 const int32 PrefixIndex = FMath::RandRange(0, UE_ARRAY_COUNT(Prefixes)-1);
 return FString::Printf(TEXT("%s-%04d"), Prefixes[PrefixIndex], FMath::RandRange(0,9999));
}
bool USPIdentityComponent::IsValidAlias(const FString& Alias)
{
 const FString Trimmed = Alias.TrimStartAndEnd();
 if (Trimmed.Len() < 3 || Trimmed.Len() > 20) return false;
 for (TCHAR C : Trimmed)
 {
  if (!FChar::IsAlnum(C) && C != TEXT('-') && C != TEXT('_')) return false;
 }
 return true;
}
