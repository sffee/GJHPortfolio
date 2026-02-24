#include "GJHAttackDataSubSystem.h"

#include "Library/GJHFileStatics.h"
#include "Data/DataAsset/GJHBakeAttackAnimSequenceDataAsset.h"

static TAutoConsoleVariable<int32> CVarCollectAttackDataTimeThreshold(
	TEXT("GJH.CollectAttackDataTimeThreshold"),
	300.f,
	TEXT(""));

UGJHAttackDataSubSystem* UGJHAttackDataSubSystem::Get(const UObject* WorldContextObject)
{
	if (GEngine)
	{
		UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);
		return UGameInstance::GetSubsystem<UGJHAttackDataSubSystem>(World->GetGameInstance());
	}

	return nullptr;
}

void UGJHAttackDataSubSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	const FString Directory = TEXT("GJH/Data/DataAsset/BakeAttackData/"); 
	UGJHFileStatics::ForeachFileInDirectory(Directory, [this](const FString& FileNameOrDirectory)
	{
		FString RelativePath = FileNameOrDirectory;
		FPaths::MakePathRelativeTo(RelativePath, *FPaths::ProjectContentDir());
        
		FString FilePath = TEXT("/Game/") + RelativePath;
		FilePath.RemoveFromEnd(TEXT(".uasset"));
		
		const FString BaseFileName = FPaths::GetBaseFilename(FilePath);
		FString AnimFileName = BaseFileName;
		AnimFileName.RemoveFromStart(TEXT("DA_"));
		AnimFileName.RemoveFromEnd(TEXT("_AttackData"));
		
		const FTopLevelAssetPath AssetPath = FTopLevelAssetPath(FName(FilePath), FName(BaseFileName));
		const FSoftObjectPath SoftObjectPath = FSoftObjectPath(AssetPath);
		TSoftObjectPtr<UGJHBakeAttackAnimSequenceDataAsset> SoftPath(SoftObjectPath);
		AttackDataMap.Add(FName(AnimFileName), SoftPath);
	});
	
	GetWorld()->GetTimerManager().SetTimer(CollectAttackDataDelegateHandle, this, &ThisClass::OnCollectAttackData, 60.f, true);
}

void UGJHAttackDataSubSystem::Deinitialize()
{
	if (CollectAttackDataDelegateHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(CollectAttackDataDelegateHandle);
		CollectAttackDataDelegateHandle.Invalidate();
	}
}

UGJHBakeAttackAnimSequenceDataAsset* UGJHAttackDataSubSystem::GetAttackData(const FString& InAnimFileName)
{
	return GetAttackData(FName(InAnimFileName));
}

UGJHBakeAttackAnimSequenceDataAsset* UGJHAttackDataSubSystem::GetAttackData(FName InAnimFileName)
{
	if (FGJHCachedAttackData* AttackData = LoadedAttackData.Find(InAnimFileName))
	{
		AttackData->Time = GetWorld()->GetTimeSeconds();
		return AttackData->AttackData;
	}
	
	if (TSoftObjectPtr<UGJHBakeAttackAnimSequenceDataAsset>* AttackDataPtr = AttackDataMap.Find(InAnimFileName))
	{
		FGJHCachedAttackData AttackData;
		AttackData.Time = GetWorld()->GetTimeSeconds();
		AttackData.AttackData = AttackDataPtr->LoadSynchronous();
		
		return LoadedAttackData.Add(InAnimFileName, MoveTemp(AttackData)).AttackData;
	}
	
	return nullptr;
}

void UGJHAttackDataSubSystem::OnCollectAttackData()
{
	const float CurrentWorldTime = GetWorld()->GetTimeSeconds();
	const float TimeThreshold = CVarCollectAttackDataTimeThreshold.GetValueOnAnyThread();
	
	for (auto Iter = LoadedAttackData.CreateIterator(); Iter; ++Iter)
	{
		if (Iter->Value.Time + TimeThreshold < CurrentWorldTime || IsValid(Iter->Value.AttackData) == false)
			Iter.RemoveCurrent();
	}
}
