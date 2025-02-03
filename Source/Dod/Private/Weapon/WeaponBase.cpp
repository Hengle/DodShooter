#include "Weapon/WeaponBase.h"

#include "Character/DodCharacter.h"
#include "Components/AudioComponent.h"
#include "Equipment/DodEquipmentDefinition.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Net/UnrealNetwork.h"


FVector AWeaponBase::GetViewModeBoneLocation(FName BoneName, int32 Index) const
{
	return VM_Attachment[Index]->GetBoneLocation(BoneName);
}

FVector AWeaponBase::GetWorldModeBoneLocation(FName BoneName, int32 Index) const
{
	return WM_Attachment[Index]->GetBoneLocation(BoneName);
}

FVector AWeaponBase::GetIKBoneLocation_VM_Right()
{
	return GetViewModeBoneLocation(FName("tag_ik_loc_ri"), 0);
}

FVector AWeaponBase::GetIKBoneLocation_VM_Left()
{
	return GetViewModeBoneLocation(FName("tag_ik_loc_le"), 1);
}

FVector AWeaponBase::GetIKBoneLocation_WM_Right()
{
	return GetWorldModeBoneLocation(FName("tag_ik_loc_ri"), 0);
}

FVector AWeaponBase::GetIKBoneLocation_WM_Left()
{
	return GetWorldModeBoneLocation(FName("tag_ik_loc_le"), 1);
}

AWeaponBase::AWeaponBase()
{
	bReplicates = true;

	VM_Attachment.Init(nullptr, 10);
	WM_Attachment.Init(nullptr, 10);

	Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);

	VM_Receiver = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("VM_Receiver"));
	VM_Receiver->SetupAttachment(RootComponent);
	VM_Receiver->SetCastShadow(false);
	VM_Attachment[0] = VM_Receiver;
	VM_Receiver->SetIsReplicated(true);

	WM_Receiver = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WM_Receiver"));
	WM_Receiver->SetupAttachment(RootComponent);
	WM_Receiver->SetCastShadow(true);
	WM_Receiver->SetCastHiddenShadow(true);
	WM_Attachment[0] = WM_Receiver;
	WM_Receiver->SetIsReplicated(true);
}

void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	SetFirstPersonVis();
}

void AWeaponBase::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, VM_Receiver);
	DOREPLIFETIME(ThisClass, WM_Receiver);
	DOREPLIFETIME(ThisClass, VM_AttachmentMeshDetails);
	DOREPLIFETIME(ThisClass, WM_AttachmentMeshDetails);
	DOREPLIFETIME(ThisClass, CamoInfo);
}

void AWeaponBase::SetViewModelAttachment(const TArray<FDodAttachmentMeshDetail>& AttachmentMeshDetail)
{
	VM_AttachmentMeshDetails = AttachmentMeshDetail;
	OnRep_VM_AttachmentMeshDetails();
}

void AWeaponBase::SetWorldModelAttachment(const TArray<FDodAttachmentMeshDetail>& AttachmentMeshDetail)
{
	WM_AttachmentMeshDetails = AttachmentMeshDetail;
	OnRep_WM_AttachmentMeshDetails();
}

void AWeaponBase::SetCamo(const FCamoInfo& InCamoInfo)
{
	CamoInfo = InCamoInfo;
	OnRep_CamoInfo();
}

void AWeaponBase::SetMeshCamo(USkeletalMeshComponent* InSkeletal)
{
	if (InSkeletal && CamoInfo.CamoSlotIndex > 0 && IsValid(CamoInfo.CamoColor))
	{
		for (int32 MaterialIndex = 0; MaterialIndex < InSkeletal->GetNumMaterials(); ++MaterialIndex)
		{
			if (UMaterialInstance* MaterialInstance = Cast<UMaterialInstanceConstant>(
				InSkeletal->GetMaterial(MaterialIndex)))
			{
				if (UMaterialInstanceDynamic* MaterialInstanceDynamic = UMaterialInstanceDynamic::Create(
					MaterialInstance, InSkeletal))
				{
					MaterialInstanceDynamic->SetScalarParameterValue(TEXT("CamoSelector"), CamoInfo.CamoSlotIndex);
					MaterialInstanceDynamic->SetTextureParameterValue(TEXT("CamoColor"), CamoInfo.CamoColor);
					if (IsValid(CamoInfo.CamoNormal))
					{
						MaterialInstanceDynamic->SetTextureParameterValue(TEXT("CamoNormal"), CamoInfo.CamoNormal);
					}
					InSkeletal->SetMaterial(MaterialIndex, MaterialInstanceDynamic);
				}
			}
		}
	}
}

void AWeaponBase::SetAttachment(const FDodAttachmentMeshDetail& AttachmentMeshDetail,
                                TArray<USkeletalMeshComponent*>& AttachmentArray,
                                USkeletalMeshComponent* Receiver)
{
	USkeletalMesh* Mesh = AttachmentMeshDetail.ModelToSpawn.LoadSynchronous();
	int32 SocketIndex = AttachmentMeshDetail.SocketIndex;
	FName AttachSocket = AttachmentMeshDetail.AttachSocket;
	if (AttachSocket.IsNone())
	{
		AttachSocket = Mesh->GetSkeleton()->GetReferenceSkeleton().GetBoneName(0);
	}
	FTransform AttachTransform = AttachmentMeshDetail.AttachTransform;

	if (!Mesh || SocketIndex == 0)
	{
		return;
	}

	if (AttachmentArray.IsValidIndex(SocketIndex))
	{
		if (!AttachmentArray[SocketIndex])
		{
			FString ComponentName = FString::Printf(TEXT("%s_%i"), *Receiver->GetName(), SocketIndex);
			AttachmentArray[SocketIndex] =
				NewObject<USkeletalMeshComponent>(this, USkeletalMeshComponent::StaticClass(), *ComponentName);
			AttachmentArray[SocketIndex]->RegisterComponent();
			AttachmentArray[SocketIndex]->SetIsReplicated(false);
		}
		if (AttachmentArray[SocketIndex])
		{
			AttachmentArray[SocketIndex]->SetSkeletalMesh(Mesh);
			SetMeshCamo(AttachmentArray[SocketIndex]);

			FAttachmentTransformRules AttachmentTransformRules{
				EAttachmentRule::KeepRelative,
				EAttachmentRule::KeepRelative,
				EAttachmentRule::KeepRelative,
				false
			};

			AttachmentArray[SocketIndex]->AttachToComponent(Receiver, AttachmentTransformRules, AttachSocket);
			AttachmentArray[SocketIndex]->SetRelativeTransform(AttachTransform);
		}
	}
}

void AWeaponBase::SetFirstPersonVis()
{
	if (ADodCharacter* DodCharacter = Cast<ADodCharacter>(GetOwner()))
	{
		if (DodCharacter->IsLocallyControlled())
		{
			ChangeToFirstPerson();
		}
		else
		{
			ChangeToThirdPerson();
		}
	}
}

void AWeaponBase::ChangeToFirstPerson()
{
	for (USkeletalMeshComponent* VM_Skeletal : VM_Attachment)
	{
		if (VM_Skeletal)
		{
			VM_Skeletal->SetVisibility(true);
			VM_Skeletal->SetCastShadow(false);
			VM_Skeletal->SetCastHiddenShadow(false);
		}
	}
	for (USkeletalMeshComponent* WM_Skeletal : WM_Attachment)
	{
		if (WM_Skeletal)
		{
			WM_Skeletal->SetVisibility(false);
			WM_Skeletal->SetCastShadow(true);
			WM_Skeletal->SetCastHiddenShadow(true);
		}
	}
}

void AWeaponBase::ChangeToThirdPerson()
{
	for (USkeletalMeshComponent* VM_Skeletal : VM_Attachment)
	{
		if (VM_Skeletal)
		{
			VM_Skeletal->SetVisibility(false);
			VM_Skeletal->SetCastShadow(false);
			VM_Skeletal->SetCastHiddenShadow(false);
		}
	}
	for (USkeletalMeshComponent* WM_Skeletal : WM_Attachment)
	{
		if (WM_Skeletal)
		{
			WM_Skeletal->SetVisibility(true);
			WM_Skeletal->SetCastShadow(true);
			WM_Skeletal->SetCastHiddenShadow(true);
		}
	}
}

void AWeaponBase::TriggerFireAudio(USoundBase* InSound, AActor* InActor)
{
	if (!IsValid(AudioComp))
	{
		ADodCharacter* Character = Cast<ADodCharacter>(InActor);
		AudioComp = UGameplayStatics::SpawnSoundAttached(InSound, Character->GetMesh(),TEXT("j_gun"));
	}
	if (IsValid(AudioComp))
	{
		AudioComp->SetTriggerParameter(TEXT("Fire"));
	}
}

void AWeaponBase::OnRep_VM_AttachmentMeshDetails()
{
	for (int32 SocketIndex = 0; SocketIndex < VM_AttachmentMeshDetails.Num(); SocketIndex++)
	{
		SetAttachment(VM_AttachmentMeshDetails[SocketIndex], VM_Attachment, VM_Receiver);
	}
	SetFirstPersonVis();
}

void AWeaponBase::OnRep_WM_AttachmentMeshDetails()
{
	for (int32 SocketIndex = 0; SocketIndex < WM_AttachmentMeshDetails.Num(); SocketIndex++)
	{
		SetAttachment(WM_AttachmentMeshDetails[SocketIndex], WM_Attachment, WM_Receiver);
	}
	SetFirstPersonVis();
}

void AWeaponBase::OnRep_CamoInfo()
{
	for (USkeletalMeshComponent* VM_Skeletal : VM_Attachment)
	{
		if (VM_Skeletal)
		{
			SetMeshCamo(VM_Skeletal);
		}
	}
	for (USkeletalMeshComponent* WM_Skeletal : VM_Attachment)
	{
		if (WM_Skeletal)
		{
			SetMeshCamo(WM_Skeletal);
		}
	}
}
