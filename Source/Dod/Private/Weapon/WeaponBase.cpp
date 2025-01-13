#include "Weapon/WeaponBase.h"

#include "Equipment/DodEquipmentDefinition.h"
#include "GameFramework/Character.h"
#include "Materials/MaterialInstanceConstant.h"


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
	WM_Receiver = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WM_Receiver"));
	WM_Receiver->SetupAttachment(RootComponent);
	WM_Receiver->SetCastShadow(true);
	WM_Receiver->SetCastHiddenShadow(true);
	WM_Attachment[0] = WM_Receiver;
}

void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	if (ACharacter* Player = Cast<ACharacter>(GetOwner()))
	{
		if (!Player->IsLocallyControlled() || Player->IsBotControlled())
		{
			VM_Receiver->SetVisibility(false);
		}
		else
		{
			WM_Receiver->SetVisibility(false);
		}
	}
}

void AWeaponBase::SetViewModelAttachment(const FDodAttachmentMeshDetail& AttachmentMeshDetail)
{
	bool bIsVis = true;
	if (ACharacter* Player = Cast<ACharacter>(GetOwner()))
	{
		if (!Player->IsLocallyControlled() || Player->IsBotControlled())
		{
			bIsVis = false;
		}
	}

	SetAttachment(AttachmentMeshDetail, VM_Attachment, VM_Receiver, bIsVis, false);
}

void AWeaponBase::SetWorldModelAttachment(const FDodAttachmentMeshDetail& AttachmentMeshDetail)
{
	bool bIsVis = false;
	if (ACharacter* Player = Cast<ACharacter>(GetOwner()))
	{
		if (!Player->IsLocallyControlled() || Player->IsBotControlled())
		{
			bIsVis = true;
		}
	}

	SetAttachment(AttachmentMeshDetail, WM_Attachment, WM_Receiver, bIsVis, true);
}

void AWeaponBase::SetCamo(const FCamoInfo& CamoInfo)
{
	auto SetCamo = [](USkeletalMeshComponent* Skeletal, const FCamoInfo& CamoInfo)
	{
		if (Skeletal)
		{
			for (int32 MaterialIndex = 0; MaterialIndex < Skeletal->GetNumMaterials(); ++MaterialIndex)
			{
				if (UMaterialInstance* MaterialInstance = Cast<UMaterialInstanceConstant>(Skeletal->GetMaterial(MaterialIndex)))
				{
					if (UMaterialInstanceDynamic* MaterialInstanceDynamic = UMaterialInstanceDynamic::Create(MaterialInstance, Skeletal))
					{
						MaterialInstanceDynamic->SetScalarParameterValue(TEXT("CamoSelector"), CamoInfo.CamoSlotIndex);
						MaterialInstanceDynamic->SetTextureParameterValue(TEXT("CamoColor"), CamoInfo.CamoColor);
						MaterialInstanceDynamic->SetTextureParameterValue(TEXT("CamoNormal"), CamoInfo.CamoNormal);
						Skeletal->SetMaterial(MaterialIndex, MaterialInstanceDynamic);
					}
				}
			}
		}
	};

	for (USkeletalMeshComponent* VM_Skeletal : VM_Attachment)
	{
		SetCamo(VM_Skeletal, CamoInfo);
	}
	for (USkeletalMeshComponent* WM_Skeletal : VM_Attachment)
	{
		SetCamo(WM_Skeletal, CamoInfo);
	}
}

void AWeaponBase::SetAttachment(const FDodAttachmentMeshDetail& AttachmentMeshDetail,
                                TArray<TObjectPtr<USkeletalMeshComponent>>& AttachmentArray,
                                USkeletalMeshComponent* Receiver,
                                bool bIsVis,
                                bool bIsCastShadow)
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
		USkeletalMeshComponent* MeshComponent = AttachmentArray[SocketIndex];
		if (!MeshComponent)
		{
			FString ComponentName = FString::Printf(TEXT("%s_%i"), *Receiver->GetName(), SocketIndex);
			AttachmentArray[SocketIndex] =
				NewObject<USkeletalMeshComponent>(this, USkeletalMeshComponent::StaticClass(), *ComponentName);
			AttachmentArray[SocketIndex]->RegisterComponent();
		}

		if (AttachmentArray[SocketIndex])
		{
			AttachmentArray[SocketIndex]->SetSkeletalMesh(Mesh);
		}

		FAttachmentTransformRules AttachmentTransformRules{
			EAttachmentRule::KeepRelative,
			EAttachmentRule::KeepRelative,
			EAttachmentRule::KeepRelative,
			false
		};

		AttachmentArray[SocketIndex]->AttachToComponent(Receiver, AttachmentTransformRules, AttachSocket);
		AttachmentArray[SocketIndex]->SetRelativeTransform(AttachTransform);

		AttachmentArray[SocketIndex]->SetVisibility(bIsVis);
		AttachmentArray[SocketIndex]->SetCastShadow(bIsCastShadow);
		AttachmentArray[SocketIndex]->SetCastHiddenShadow(bIsCastShadow);
	}
}
