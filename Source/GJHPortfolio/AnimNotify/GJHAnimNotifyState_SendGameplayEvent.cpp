#include "GJHAnimNotifyState_SendGameplayEvent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagsManager.h"

void UGJHAnimNotifyState_SendGameplayEvent::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	
	if (MeshComp->GetWorld()->IsPreviewWorld())
		return;
	
	if (UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(MeshComp->GetOwner()))
	{
		FGameplayEventData EventData;
		EventData.EventMagnitude = TotalDuration;
		ASC->HandleGameplayEvent(StartTag, &EventData);
	}
}

void UGJHAnimNotifyState_SendGameplayEvent::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	
	if (MeshComp->GetWorld()->IsPreviewWorld())
		return;
	
	if (UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(MeshComp->GetOwner()))
	{
		FGameplayEventData EventData;
		ASC->HandleGameplayEvent(EndTag, &EventData);
	}
}

FString UGJHAnimNotifyState_SendGameplayEvent::GetNotifyName_Implementation() const
{
	FString Name(TEXT("Start : "));
	
	const auto GetTag = [this](const FGameplayTag& InTag) -> FString
	{
		FString Result;
		
		TArray<FName> Names;
		UGameplayTagsManager::Get().SplitGameplayTagFName(StartTag, Names);

		if (2 <= Names.Num())
		{
			Result = Names.Last(1).ToString();
			Result.Append(Names.Last().ToString());
		}
		else
		{
			Result = TEXT("Empty");
		}
		
		return Result;
	};
	
	Name.Append(GetTag(StartTag));
	Name.Append(TEXT(" / End : "));
	Name.Append(GetTag(EndTag));
	
	return Name;
}
