#include "GJHActorStatusBubbleIconWidget.h"

#include "Components/Image.h"

void UGJHActorStatusBubbleIconWidget::SetMaterial(UMaterialInterface* InMaterial)
{
	Image_StatusIcon->SetBrushFromMaterial(InMaterial);
}
