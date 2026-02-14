#include "GJHAnimNotify_SendGameplayEvent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagsManager.h"

void UGJHAnimNotify_SendGameplayEvent::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp->GetWorld()->IsPreviewWorld())
		return;
	
	if (UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(MeshComp->GetOwner()))
	{
		ASC->HandleGameplayEvent(GameplayTag, &EventData);
	}
}

FString UGJHAnimNotify_SendGameplayEvent::GetNotifyName_Implementation() const
{
	TArray<FName> Names;
	UGameplayTagsManager::Get().SplitGameplayTagFName(GameplayTag, Names);

	FString Name(TEXT("Event : "));

	if (2 <= Names.Num())
	{
		Name.Append(Names.Last(1).ToString());
		Name.Append(Names.Last().ToString());
	}
	else
	{
		Name.Append(TEXT("Empty"));
	}
	
	return Name;
}
