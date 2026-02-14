#pragma once
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameplayTagsManager.h"

struct GJHPORTFOLIO_API FGJHGameplayTag
{
	#define GJH_GAMEPLAYTAG(Tag, TagName) \
	static FGameplayTag Tag() \
	{ \
		UGameplayTagsManager& Manager = UGameplayTagsManager::Get(); \
		FGameplayTag OutTag = Manager.RequestGameplayTag(FName(TagName), false); \
		if (!OutTag.IsValid()) \
			OutTag = Manager.AddNativeGameplayTag(FName(TagName)); \
		return OutTag; \
	};
	#include "GJHGameplayTag.inl"
	#undef GJH_GAMEPLAYTAG
};

struct GJHPORTFOLIO_API FGJHGameplayTagStaticInit final : FGameplayTagNativeAdder
{
	virtual ~FGJHGameplayTagStaticInit() = default;
	virtual void AddTags() override;
};
