#include "GJHDamageTextWidget.h"

#include "Animation/WidgetAnimation.h"
#include "Components/TextBlock.h"

void UGJHDamageTextWidget::SetDamage(float InDamage)
{
	FVector2D RandomLoc = GetRenderTransform().Translation;
	RandomLoc.X += FMath::RandRange(-30.f, 30.f);
	RandomLoc.Y += FMath::RandRange(-30.f, 30.f);
	SetRenderTranslation(RandomLoc);

	TextBlock_Damage->SetText(FText::AsNumber(InDamage));

	if (IsValid(Animation_DamageText))
		PlayAnimation(Animation_DamageText);
}
