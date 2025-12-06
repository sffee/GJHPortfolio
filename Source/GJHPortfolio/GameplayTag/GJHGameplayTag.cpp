#include "GJHGameplayTag.h"

void FGJHGameplayTagStaticInit::AddTags()
{
	#define GJH_GAMEPLAYTAG(Tag, TagName) FGJHGameplayTag::Tag();
	#include "GJHGameplayTag.inl"
	#undef GJH_GAMEPLAYTAG
}

static FGJHGameplayTagStaticInit GJHGameplayTagStaticInit;