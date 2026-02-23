#include "GJHDamageTextWidget.h"

#include "Animation/WidgetAnimation.h"
#include "Components/TextBlock.h"
#include "GameplayTag/GJHGameplayTag.h"

void UGJHDamageTextWidget::SetDamage(float InDamage, const FGameplayTag& InStatusTag)
{
	FVector2D RandomLoc = GetRenderTransform().Translation;
	RandomLoc.X += FMath::RandRange(-30.f, 30.f);
	RandomLoc.Y += FMath::RandRange(-30.f, 30.f);
	SetRenderTranslation(RandomLoc);
	
	FNumberFormattingOptions Format;
	Format.MinimumFractionalDigits = 0;
	Format.MaximumFractionalDigits = 0;

	TextBlock_Damage->SetText(FText::AsNumber(InDamage, &Format));
	
	FLinearColor TextColor = FLinearColor::White;
	if (InStatusTag.MatchesTag(FGJHGameplayTag::Status_Type_Poison_Deadly()))
		TextColor = FLinearColor(0.2f, 0.4f, 0.6f);
	else if (InStatusTag.MatchesTag(FGJHGameplayTag::Status_Type_Poison()))
		TextColor = FLinearColor(0.f, 1.f, 0.f);
	
	TextBlock_Damage->SetColorAndOpacity(TextColor);

	if (IsValid(Animation_DamageText))
		PlayAnimation(Animation_DamageText);
}
