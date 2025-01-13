#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

struct FCamoInfo;
struct FDodAttachmentMeshDetail;

UCLASS()
class DOD_API AWeaponBase : public AActor
{
	GENERATED_BODY()

public:
	AWeaponBase();
	virtual void BeginPlay() override;

	void SetViewModelAttachment(const FDodAttachmentMeshDetail& AttachmentMeshDetail);
	void SetWorldModelAttachment(const FDodAttachmentMeshDetail& AttachmentMeshDetail);

	void SetCamo(const FCamoInfo& CamoInfo);

	void SetAttachment(const FDodAttachmentMeshDetail& AttachmentMeshDetail,
	                   TArray<TObjectPtr<USkeletalMeshComponent>>& AttachmentArray,
	                   USkeletalMeshComponent* Receiver,
	                   bool bIsVis,
	                   bool bIsCastShadow);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dod|Weapon")
	USceneComponent* Root;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dod|Weapon")
	TObjectPtr<USkeletalMeshComponent> VM_Receiver;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dod|Weapon")
	TObjectPtr<USkeletalMeshComponent> WM_Receiver;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dod|Weapon")
	TArray<TObjectPtr<USkeletalMeshComponent>> VM_Attachment;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dod|Weapon")
	TArray<TObjectPtr<USkeletalMeshComponent>> WM_Attachment;
};
