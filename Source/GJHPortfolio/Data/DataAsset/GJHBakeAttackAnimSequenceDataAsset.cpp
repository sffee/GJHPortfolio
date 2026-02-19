#include "GJHBakeAttackAnimSequenceDataAsset.h"

void UGJHBakeAttackAnimSequenceDataAsset::Reserve(int32 InSize)
{
	AttackData.Reserve(InSize);
}

void UGJHBakeAttackAnimSequenceDataAsset::AddAttackData(float InTime, const FVector& InStartLocation, const FVector& InEndLocation)
{
	FGJHBakeAnimSocketData& AnimData = AttackData.AddDefaulted_GetRef();
	AnimData.Time = InTime;
	
	AnimData.SocketStartLocation = InStartLocation;
	AnimData.SocketEndLocation = InEndLocation;
}

const TArray<FGJHBakeAnimSocketData>& UGJHBakeAttackAnimSequenceDataAsset::GetAttackData() const
{
	return AttackData;
}
