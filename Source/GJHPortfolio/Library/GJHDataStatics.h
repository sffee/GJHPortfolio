#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Data/DataTable/GJHDataTableTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GJHDataStatics.generated.h"

class UGameplayAbility;

UCLASS()
class GJHPORTFOLIO_API UGJHDataStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static UDataTable* GetDataTable(const FGameplayTag& InDataTableTypeTag);

	template<typename T>
	static void ForeachDataTableInfo(const FGameplayTag& InDataTableTypeTag, TFunctionRef<void(const T&)> InFunc);

	static FGJHMonsterTableInfo GetMonsterInfo(const UObject* InWorldContextObject, int32 InMonsterIndex);
	static FGameplayTag GetMonsterGrade(const UObject* InWorldContextObject, int32 InMonsterIndex);
	static float GetMonsterXP(const UObject* InWorldContextObject, int32 InMonsterIndex, int32 InMonsterLevel);

	static FGJHSkillTableInfo GetSkillInfo(const UObject* InWorldContextObject, int32 InSkillIndex);
	static FGJHSkillTableInfo GetSkillInfo(const UObject* InWorldContextObject, const FGameplayTag& InCharacterTypeTag, int32 InSkillIndex);
	static int32 GetSkillIndex(const UObject* InWorldContextObject, const UGameplayAbility* InAbility);
	static int32 GetSkillIndex(const UObject* InWorldContextObject, const FGameplayTag& InCharacterTypeTag, const UGameplayAbility* InAbility);
	static UTexture2D* GetSkillIcon(const UObject* InWorldContextObject, int32 InSkillIndex);
	static UTexture2D* GetSkillIcon(const UObject* InWorldContextObject, const FGameplayTag& InCharacterTypeTag, int32 InSkillIndex);

	static FGameplayTag GetSkillCooldownTag(const UObject* InWorldContextObject, const int32 InSkillIndex);
	static FGameplayTag GetSkillCooldownTag(const UObject* InWorldContextObject, const FGameplayTag& InCharacterTypeTag, const int32 InSkillIndex);

	static int32 GetSkillLevelRequireBySkillIndex(const UObject* InWorldContextObject, const int32 InSkillIndex);
	static int32 GetSkillLevelRequireBySkillIndex(const UObject* InWorldContextObject, const FGameplayTag& InCharacterTypeTag, const int32 InSkillIndex);

	static void ForeachSkillInfo(const UObject* InWorldContextObject, TFunctionRef<void(const FGJHSkillTableInfo&)> InFunc);
	static void ForeachSkillInfo(const UObject* InWorldContextObject, const FGameplayTag& InCharacterTypeTag, TFunctionRef<void(const FGJHSkillTableInfo&)> InFunc);

	static TSubclassOf<UGJHItemDefinition> GetItemDefinition(const UObject* InWorldContextObject, const int32 InItemIndex);
};

template <typename T>
void UGJHDataStatics::ForeachDataTableInfo(const FGameplayTag& InDataTableTypeTag, TFunctionRef<void(const T&)> InFunc)
{
	UDataTable* DataTable = GetDataTable(InDataTableTypeTag);
	if (IsValid(DataTable) == false)
		return;

	DataTable->ForeachRow<T>("", [InFunc](const FName& Key, const T& Value)
	{
		InFunc(Value);
	});
}
