#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GJHBakeAttackAnimSequenceDataAsset.generated.h"

USTRUCT()
struct FGJHBakeAnimSocketData
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere)
	float Time = -1.f;
	
	UPROPERTY(VisibleAnywhere)
	FVector SocketStartLocation;
	
	UPROPERTY(VisibleAnywhere)
	FVector SocketEndLocation;
	
public:
	bool IsValid() const { return 0.f <= Time; }
};

UCLASS()
class GJHPORTFOLIO_API UGJHBakeAttackAnimSequenceDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
private:
	UPROPERTY(VisibleAnywhere)
	TArray<FGJHBakeAnimSocketData> AttackData;
	
public:
	void Reserve(int32 InSize);
	void AddAttackData(float InTime, const FVector& InStartLocation, const FVector& InEndLocation);
	const TArray<FGJHBakeAnimSocketData>& GetAttackData() const;
};
