#include "GJHDamageGameplayAbilityBase.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameplayTag/GJHGameplayTag.h"
#include "GJHSettings/GJHDataDeveloperSettings.h"
#include "Library/GJHAbilitySystemStatics.h"
#include "Library/GJHDataStatics.h"

float UGJHDamageGameplayAbilityBase::GetDamage(int32 InComboIndex)
{
	if (DamageKindData.Contains(InComboIndex) == false)
		return 0.f;

	return DamageKindData[InComboIndex].Damage.GetValueAtLevel(GetAbilityLevel());
}

void UGJHDamageGameplayAbilityBase::ApplyDamage(float InDamage, AActor* InTarget, const int32 InComboIndex, const FGameplayTag& InDamageType) const
{
	if (DamageEffect == nullptr)
		return;

	if (K2_HasAuthority() == false)
		return;

	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	if (IsValid(ASC) == false)
		return;

	FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(DamageEffect, 1, MakeDamageEffectContext(InTarget, InComboIndex));
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, FGJHGameplayTag::Ability_SetByCaller_Damage(), InDamage);
	UAbilitySystemBlueprintLibrary::AddAssetTag(EffectSpecHandle, InDamageType);
	
	ASC->ApplyGameplayEffectSpecToTarget(*EffectSpecHandle.Data.Get(), UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InTarget));
}

void UGJHDamageGameplayAbilityBase::ApplyStatusDamage(float InDuration, AActor* InTarget, const FGameplayTag& InStatusTag) const
{
	if (StatusTag.IsValid() == false)
		return;

	if (K2_HasAuthority() == false)
		return;
	
	FGJHStatusData StatusData = UGJHDataStatics::GetStatusData(InStatusTag);
	if (StatusData.GameplayEffect.IsNull())
		return;

	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	if (IsValid(ASC) == false)
		return;

	FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(StatusData.GameplayEffect.LoadSynchronous(), 1, MakeEffectContext(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo()));
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, FGJHGameplayTag::Ability_SetByCaller_Damage(), 2.f);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, FGJHGameplayTag::Status_SetByCaller_Duration(), InDuration);
	UAbilitySystemBlueprintLibrary::AddAssetTag(EffectSpecHandle, InStatusTag);
	
	ASC->ApplyGameplayEffectSpecToTarget(*EffectSpecHandle.Data.Get(), UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InTarget));
}

FGameplayEffectContextHandle UGJHDamageGameplayAbilityBase::MakeDamageEffectContext(AActor* InTarget, const int32 InComboIndex) const
{
	FGameplayEffectContextHandle EffectContextHandle = MakeEffectContext(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo());
	
	const FGJHDamageKindData& Data = DamageKindData[InComboIndex];
	
	AActor* Attacker = GetCurrentActorInfo()->AvatarActor.Get();
	const FVector PushDirection = (InTarget->GetActorLocation() - Attacker->GetActorLocation()).GetSafeNormal(); 

	const FVector KnockBackVector = PushDirection * FVector(Data.KnockBackImpulseX, 0.f, Data.KnockBackImpulseZ);
	UGJHAbilitySystemStatics::SetKnockBackImpulse(EffectContextHandle, KnockBackVector);
	
	const FVector DeathVector = PushDirection * FVector(Data.DeathImpulseX, 0.f, Data.DeathImpulseZ);
	UGJHAbilitySystemStatics::SetDeathImpulse(EffectContextHandle, DeathVector);	
	
	return EffectContextHandle;
}
