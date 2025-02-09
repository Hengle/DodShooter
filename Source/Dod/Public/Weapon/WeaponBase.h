#pragma once

#include "CoreMinimal.h"
#include "Equipment/DodEquipmentDefinition.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

struct FCamoInfo;
struct FDodAttachmentMeshDetail;

UCLASS()
class DOD_API AWeaponBase : public AActor
{
	GENERATED_BODY()

public:
	
	FVector GetViewModeBoneLocation(FName BoneName, int32 Index) const;
	FVector GetWorldModeBoneLocation(FName BoneName, int32 Index) const;

	FVector GetIKBoneLocation_VM_Right();
	FVector GetIKBoneLocation_VM_Left();
	FVector GetIKBoneLocation_WM_Right();
	FVector GetIKBoneLocation_WM_Left();
	
	AWeaponBase();
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	void SetViewModelAttachment(const TArray<FDodAttachmentMeshDetail>& AttachmentMeshDetail);

	void SetWorldModelAttachment(const TArray<FDodAttachmentMeshDetail>& AttachmentMeshDetail);

	void SetCamo(const FCamoInfo& InCamoInfo);
	void SetMeshCamo(USkeletalMeshComponent* InSkeletal);
	

	void SetAttachment(const FDodAttachmentMeshDetail& AttachmentMeshDetail,
	                   TArray<USkeletalMeshComponent*>& AttachmentArray,
	                   USkeletalMeshComponent* Receiver);

	void SetFirstPersonVis();
	void ChangeToFirstPerson();
	void ChangeToThirdPerson();

	void TriggerFireAudio(USoundBase* InSound, AActor* InActor);

	UFUNCTION()
	void OnRep_VM_AttachmentMeshDetails();
	UFUNCTION()
	void OnRep_WM_AttachmentMeshDetails();
	UFUNCTION()
	void OnRep_CamoInfo();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dod|Weapon")
	USceneComponent* Root;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dod|Weapon", Replicated)
	TObjectPtr<USkeletalMeshComponent> VM_Receiver;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dod|Weapon", Replicated)
	TObjectPtr<USkeletalMeshComponent> WM_Receiver;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dod|Weapon", Replicated)
	TArray<USkeletalMeshComponent*> VM_Attachment;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dod|Weapon", Replicated)
	TArray<USkeletalMeshComponent*> WM_Attachment;

	UPROPERTY(ReplicatedUsing=OnRep_VM_AttachmentMeshDetails)
	TArray<FDodAttachmentMeshDetail> VM_AttachmentMeshDetails;
	UPROPERTY(ReplicatedUsing=OnRep_WM_AttachmentMeshDetails)
	TArray<FDodAttachmentMeshDetail> WM_AttachmentMeshDetails;

	UPROPERTY(ReplicatedUsing=OnRep_CamoInfo)
	FCamoInfo CamoInfo;
private:
	UPROPERTY()
	UAudioComponent* AudioComp{nullptr};
};
