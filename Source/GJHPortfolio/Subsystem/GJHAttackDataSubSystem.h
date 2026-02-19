#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GJHAttackDataSubSystem.generated.h"

class UGJHBakeAttackAnimSequenceDataAsset;

USTRUCT()
struct FGJHCachedAttackData
{
	GENERATED_BODY()
	
	float Time = 0.f;
	
	UPROPERTY()
	TObjectPtr<UGJHBakeAttackAnimSequenceDataAsset> AttackData;
};

UCLASS()
class GJHPORTFOLIO_API UGJHAttackDataSubSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
private:
	TMap<uint32, TSoftObjectPtr<UGJHBakeAttackAnimSequenceDataAsset>> AttackDataMap;
	
	UPROPERTY()
	TMap<uint32, FGJHCachedAttackData> LoadedAttackData;
	
	FTimerHandle CollectAttackDataDelegateHandle;
	
public:
	static UGJHAttackDataSubSystem* Get(const UObject* WorldContextObject);
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
public:
	UGJHBakeAttackAnimSequenceDataAsset* GetAttackData(const FString& InAnimFileName);
	UGJHBakeAttackAnimSequenceDataAsset* GetAttackData(uint32 InAnimFileHash);
	
private:
	void OnCollectAttackData();
};
