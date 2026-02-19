#include "BakeAttackAnimSequenceWidget.h"

#include "AssetToolsModule.h"
#include "ContentBrowserModule.h"
#include "EditorUtilityWidgetComponents.h"
#include "IContentBrowserSingleton.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Data/DataAsset/GJHBakeAttackAnimSequenceDataAsset.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Factories/DataAssetFactory.h"
#include "UObject/SavePackage.h"

const FString UBakeAttackAnimSequenceWidget::SaveDirectoryPath = "/Game/GJH/Data/DataAsset/BakeAttackData/";

void UBakeAttackAnimSequenceWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (IsValid(EditorUtilityButton_Bake))
		EditorUtilityButton_Bake->OnClicked.AddDynamic(this, &ThisClass::OnClickedButton_Bake);
}

UGJHBakeAttackAnimSequenceDataAsset* UBakeAttackAnimSequenceWidget::CreateDataAsset(const FString& InAssetName)
{
	FAssetToolsModule& AssetToolsModule = FModuleManager::Get().LoadModuleChecked<FAssetToolsModule>("AssetTools");
	IAssetTools& AssetTools = AssetToolsModule.Get();
	UDataAssetFactory* Factory = NewObject<UDataAssetFactory>();

	return Cast<UGJHBakeAttackAnimSequenceDataAsset>(AssetTools.CreateAsset(TEXT("DA_") + InAssetName + TEXT("_AttackData"), SaveDirectoryPath, UGJHBakeAttackAnimSequenceDataAsset::StaticClass(), Factory));
}

void UBakeAttackAnimSequenceWidget::SaveDataAsset(UDataAsset* InDataAsset)
{
	UPackage* Package = InDataAsset->GetOutermost();
	if (IsValid(Package) == false)
		return;
	
	Package->SetDirtyFlag(true);
	
	FSavePackageArgs SaveArgs;
	SaveArgs.TopLevelFlags = EObjectFlags::RF_Public | EObjectFlags::RF_Standalone;
	SaveArgs.SaveFlags = SAVE_NoError;
	SaveArgs.bForceByteSwapping = false;
	SaveArgs.bWarnOfLongFilename = true;
    
	const FString PackageFileName = FPackageName::LongPackageNameToFilename(Package->GetName(), FPackageName::GetAssetPackageExtension());
	const bool bSaved = UPackage::SavePackage(Package, InDataAsset, *PackageFileName, SaveArgs);

	if (bSaved)
		FAssetRegistryModule::AssetCreated(InDataAsset);
	
	FContentBrowserModule& ContentBrowserModule = FModuleManager::Get().LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
	IContentBrowserSingleton& ContentBrowser = ContentBrowserModule.Get();

	ContentBrowser.SetSelectedPaths({ SaveDirectoryPath }, true);
}

void UBakeAttackAnimSequenceWidget::OnClickedButton_Bake()
{
	if (IsValid(AnimMontage) == false || IsValid(AnimMontage->GetSkeleton()) == false)
		return;
	
	if (SamplingInterval <= 0.f)
		return;
	
	if (StartSocketName.IsNone() || EndSocketName.IsNone())
		return;
	
	FMemMark Mark(FMemStack::Get());
	
	USkeleton* Skeleton = AnimMontage->GetSkeleton();
	
	USkeletalMeshSocket* StartSocket = Skeleton->FindSocket(StartSocketName);
	USkeletalMeshSocket* EndSocket = Skeleton->FindSocket(EndSocketName);
	
	if (IsValid(StartSocket) == false || IsValid(EndSocket) == false)
		return;
	
	const FReferenceSkeleton& RefSkeleton = AnimMontage->GetSkeleton()->GetReferenceSkeleton();
	TArray<FBoneIndexType> RequiredBoneIndices;
	RequiredBoneIndices.AddUninitialized(RefSkeleton.GetNum());
	for (int32 i = 0; i < RefSkeleton.GetNum(); ++i)
		RequiredBoneIndices[i] = i;
	
	FBoneContainer BoneContainer;
	BoneContainer.InitializeTo(RequiredBoneIndices, UE::Anim::FCurveFilterSettings(UE::Anim::ECurveFilterMode::DisallowAll), *AnimMontage->GetSkeleton());
	
	FPoseContext PoseContext(BoneContainer);
	FAnimationPoseData PoseData(PoseContext);
	
	const int32 StartBoneIndex = RefSkeleton.FindBoneIndex(StartSocket->BoneName);
	const int32 EndBoneIndex = RefSkeleton.FindBoneIndex(EndSocket->BoneName);
	
	const float AnimTotalDuration = AnimMontage->GetPlayLength();
	
	const TArray<FSlotAnimationTrack> SlotTracks = AnimMontage->SlotAnimTracks;
	if (SlotTracks.IsEmpty())
		return;
	
	UGJHBakeAttackAnimSequenceDataAsset* DataAsset = CreateDataAsset(AnimMontage->GetName());
	if (IsValid(DataAsset) == false)
		return;
	
	DataAsset->Reserve(AnimTotalDuration / SamplingInterval);
	
	int32 SegmentIndex = 0;
	const TArray<FAnimSegment>& AnimSegments = SlotTracks[0].AnimTrack.AnimSegments;
	for (float Time = 0.f; Time <= AnimTotalDuration; Time += SamplingInterval)
	{
		if (AnimSegments.Num() <= SegmentIndex)
			break;
		
		const FAnimSegment& AnimSegment = AnimSegments[SegmentIndex];
		
		const float StartTimeInMontage = AnimSegment.StartPos;
		const float EndTimeInMontage = StartTimeInMontage + AnimSegment.AnimEndTime;
		
		if (EndTimeInMontage < Time)
		{
			SegmentIndex++;
			Time -= SamplingInterval;
			continue;
		}
		
		const float TimeInMontage = Time - StartTimeInMontage; 
		FAnimExtractContext ExtractContext(TimeInMontage, false);
		AnimSegment.GetAnimReference()->GetAnimationPose(PoseData, ExtractContext);
		
		FCSPose<FCompactPose> CSPose;
		CSPose.InitPose(PoseData.GetPose());
	
		const FTransform StartBoneCSTransform = CSPose.GetComponentSpaceTransform(FCompactPoseBoneIndex(StartBoneIndex));
		const FTransform EndBoneCSTransform = CSPose.GetComponentSpaceTransform(FCompactPoseBoneIndex(EndBoneIndex));
	
		const FTransform StartBoneTransform = StartSocket->GetSocketLocalTransform() * StartBoneCSTransform;
		const FTransform EndBoneTransform = EndSocket->GetSocketLocalTransform() * EndBoneCSTransform;
	
		DataAsset->AddAttackData(Time, StartBoneTransform.GetLocation(), EndBoneTransform.GetLocation());
	}
	
	SaveDataAsset(DataAsset);
}
