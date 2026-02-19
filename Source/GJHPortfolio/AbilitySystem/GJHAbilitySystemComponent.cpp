#include "GJHAbilitySystemComponent.h"

#include "GJHGameplayAbilityBase.h"
#include "Data/DataTable/GJHDataTableTypes.h"
#include "GameplayTag/GJHGameplayTag.h"
#include "Library/GJHDataStatics.h"
#include "Library/GJHGameplayStatics.h"
#include "Subsystem/GJHDataSubSystem.h"

UGJHAbilitySystemComponent::UGJHAbilitySystemComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
	SetIsReplicatedByDefault(true);
}

void UGJHAbilitySystemComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UGJHAbilitySystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UGJHAbilitySystemComponent::AbilityInputPressed(const FGameplayTag& InInputTag)
{
	if (InInputTag.IsValid() == false)
		return;

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InInputTag))
		{
			if (AbilitySpec.IsActive())
			{
				AbilitySpecInputPressed(AbilitySpec);
				
				if (GetNetMode() == NM_Client)
					Server_AbilityInputPressed(InInputTag);
			}
			else
			{
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}
}

void UGJHAbilitySystemComponent::AbilityInputPressedByAbilityTag(const FGameplayTag& InAbilityTag)
{
	if (InAbilityTag.IsValid() == false)
		return;

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.Ability->GetAssetTags().HasTagExact(InAbilityTag))
		{
			if (AbilitySpec.IsActive())
				AbilitySpecInputPressed(AbilitySpec);
			else
				TryActivateAbility(AbilitySpec.Handle);
		}
	}
}

void UGJHAbilitySystemComponent::AbilityInputReleased(const FGameplayTag& InInputTag)
{
	if (InInputTag.IsValid() == false)
		return;

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InInputTag) && AbilitySpec.IsActive())
			AbilitySpecInputReleased(AbilitySpec);
	}
}

void UGJHAbilitySystemComponent::AbilityInputHeld(const FGameplayTag& InInputTag)
{
	if (InInputTag.IsValid() == false)
		return;

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InInputTag))
		{
			AbilitySpecInputPressed(AbilitySpec);
			if (AbilitySpec.IsActive() == false)
				TryActivateAbility(AbilitySpec.Handle);
		}
	}
}

void UGJHAbilitySystemComponent::Server_AbilityInputPressed_Implementation(const FGameplayTag& InInputTag)
{
	AbilityInputPressed(InInputTag);
}

void UGJHAbilitySystemComponent::AddAbility(const TSubclassOf<UGameplayAbility> InAbility)
{
	if (IsValid(GetOwner()) == false || GetOwner()->HasAuthority() == false)
		return;
	
	FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(InAbility, 1);
	if (const UGJHGameplayAbilityBase* GJHAbility = Cast<UGJHGameplayAbilityBase>(AbilitySpec.Ability))
	{
		if (GJHAbility->GetDefaultQuickSlotInputTag().IsValid())
			AbilitySpec.GetDynamicSpecSourceTags().AddTag(GJHAbility->GetDefaultQuickSlotInputTag());

		AbilitySpec.SourceObject = GetAvatarActor();
		GiveAbility(AbilitySpec);

		OnAbilityGiven.Broadcast(AbilitySpec);
	}
}

void UGJHAbilitySystemComponent::AddAbilities(const TArray<TSubclassOf<UGameplayAbility>> InAbilities)
{
	if (IsValid(GetOwner()) == false || GetOwner()->HasAuthority() == false)
		return;
	
	for (const TSubclassOf<UGameplayAbility>& AbilityClass : InAbilities)
		AddAbility(AbilityClass);
}

void UGJHAbilitySystemComponent::ApplyGameplayEffect(const TSubclassOf<UGameplayEffect> InGameplayEffect, int32 Level)
{
	if (IsValid(GetOwner()) == false || GetOwner()->HasAuthority() == false)
		return;
	
	FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingSpec(InGameplayEffect, Level, MakeEffectContext());
	ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
}

void UGJHAbilitySystemComponent::ApplyGameplayEffects(const TArray<TSubclassOf<UGameplayEffect>> InGameplayEffects, int32 Level)
{
	if (IsValid(GetOwner()) == false || GetOwner()->HasAuthority() == false)
		return;
	
	for (const TSubclassOf<UGameplayEffect>& GameplayEffectClass : InGameplayEffects)
		ApplyGameplayEffect(GameplayEffectClass, Level);
}

void UGJHAbilitySystemComponent::AddSkillLevel(const int32 InSkillIndex, int32 InAddPoint)
{
	FGameplayAbilitySpec* Spec = GetSpecBySkillIndex(InSkillIndex);
	if (Spec == nullptr)
		return;

	FGJHSkillTableInfo SkillTableInfo = UGJHDataStatics::GetSkillInfo(this, InSkillIndex);
	if (SkillTableInfo.Index == INVALID_SKILL_INDEX)
		return;
	
	const int32 AbilityLevel = Spec->Level;
	const int32 NewSkillLevel = AbilityLevel + (InAddPoint * -1.f); 

	if (NewSkillLevel < SkillTableInfo.MinLevel || SkillTableInfo.MaxLevel < NewSkillLevel)
		return;

	Spec->Level += (InAddPoint * -1.f);
	UGJHGameplayStatics::AddSkillPoint(this, InAddPoint);
	OnAbilityLevelChanged.Broadcast(InSkillIndex, Spec->Level);
}

int32 UGJHAbilitySystemComponent::GetAbilityLevelBySkillIndex(const int32 InSkillIndex)
{
	FGameplayAbilitySpec* AbilitySpec = GetSpecBySkillIndex(InSkillIndex);
	if (AbilitySpec)
		return AbilitySpec->Level;

	return 0;
}

FString UGJHAbilitySystemComponent::GetDescriptionBySkillIndex(const int32 InSkillIndex)
{
	FGameplayAbilitySpec* AbilitySpec = GetSpecBySkillIndex(InSkillIndex);
	if (AbilitySpec)
	{
		if (UGJHGameplayAbilityBase* Ability = Cast<UGJHGameplayAbilityBase>(AbilitySpec->NonReplicatedInstances[0]))
			return Ability->GetDescription();
	}
	else
	{
		int32 CharacterLevel = UGJHGameplayStatics::GetPlayerCharacterLevel(this);
		int32 AbilityLevelRequire = UGJHDataStatics::GetSkillLevelRequireBySkillIndex(this, InSkillIndex);

		if (CharacterLevel < AbilityLevelRequire)
			return FString::Printf(TEXT("<Red>스킬 습득에 필요한 레벨 : %d</>"), AbilityLevelRequire);
	}

	return FString(TEXT("UnKnown"));
}

void UGJHAbilitySystemComponent::EquipAbility(const int32 InSkillIndex, const FGameplayTag& InQuickSlotInputTag)
{
	FGameplayAbilitySpec* AbilitySpec = GetSpecBySkillIndex(InSkillIndex);
	if (AbilitySpec == nullptr)
		return;

	const FGameplayTag PrevQuickSlotInputTag = GetQuickSlotInputTagFromSpec(AbilitySpec);

	ClearQuickSlot(AbilitySpec);
	AbilitySpec->GetDynamicSpecSourceTags().AddTag(InQuickSlotInputTag);

	OnAbilityEquipped.Broadcast(InSkillIndex, InQuickSlotInputTag, PrevQuickSlotInputTag);
}

FGameplayAbilitySpec* UGJHAbilitySystemComponent::GetSpecByAbilityTag(const FGameplayTag& InAbilityTag)
{
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		for (const FGameplayTag& GameplayTag : AbilitySpec.Ability->GetAssetTags())
		{
			if (GameplayTag.MatchesTag(InAbilityTag))
				return &AbilitySpec;
		}
	}

	return nullptr;
}

FGameplayAbilitySpec* UGJHAbilitySystemComponent::GetSpecByQuickSlotInputTag(const FGameplayTag& InQuickSlotInputTag)
{
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InQuickSlotInputTag))
			return &AbilitySpec;
	}

	return nullptr;
}

FGameplayAbilitySpec* UGJHAbilitySystemComponent::GetSpecBySkillIndex(const int32 InSkillIndex)
{
	const FGJHSkillTableInfo SkillInfo = UGJHDataStatics::GetSkillInfo(this, InSkillIndex);
	if (SkillInfo.Ability == nullptr)
	{
		ensureMsgf(false, TEXT("Ability Not Set!!"));
		return nullptr;
	}
	
	const FGameplayTagContainer AbilityTags = SkillInfo.Ability.GetDefaultObject()->GetAssetTags();
	if (AbilityTags.IsEmpty())
		return nullptr;
	
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		for (const FGameplayTag& GameplayTag : AbilitySpec.Ability->GetAssetTags())
		{
			if (GameplayTag.MatchesTag(AbilityTags.First()))
				return &AbilitySpec;
		}
	}

	return nullptr;
}

FGameplayTag UGJHAbilitySystemComponent::GetQuickSlotInputTagFromSpec(const FGameplayAbilitySpec* InAbilitySpec)
{
	if (InAbilitySpec)
	{
		for (FGameplayTag InputTag : InAbilitySpec->GetDynamicSpecSourceTags())
		{
			if (InputTag.MatchesTag(FGJHGameplayTag::Input()))
				return InputTag;
		}
	}

	return FGameplayTag();
}

void UGJHAbilitySystemComponent::CancelAbilityByAbilityTag(const FGameplayTag& InAbilityTag)
{
	if (InAbilityTag.IsValid() == false)
		return;

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.Ability->GetAssetTags().HasTagExact(InAbilityTag))
			CancelAbilityHandle(AbilitySpec.Handle);
	}
}

void UGJHAbilitySystemComponent::CancelAbilityByInputTag(const FGameplayTag& InInputTag)
{
	if (InInputTag.IsValid() == false)
		return;

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InInputTag))
			CancelAbilityHandle(AbilitySpec.Handle);
	}
}

bool UGJHAbilitySystemComponent::AbilityHasAnyQuickSlot(FGameplayAbilitySpec* InAbilitySpec) const
{
	return InAbilitySpec->GetDynamicSpecSourceTags().HasTag(FGJHGameplayTag::Input());
}

void UGJHAbilitySystemComponent::ClearQuickSlot(FGameplayAbilitySpec* InAbilitySpec)
{
	const FGameplayTag SlotTag = GetQuickSlotInputTagFromSpec(InAbilitySpec);
	if (SlotTag.IsValid())
		InAbilitySpec->GetDynamicSpecSourceTags().RemoveTag(SlotTag);
}
