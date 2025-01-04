#include "Equipment/DodEquipmentInstance.h"

#include "Character/DodShooter.h"
#include "Equipment/DodEquipmentDefinition.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"
#include "Weapon/WeaponBase.h"

APawn* UDodEquipmentInstance::GetPawn() const
{
	return Cast<APawn>(GetOuter());
}

void UDodEquipmentInstance::OnEquipped()
{
	K2_OnEquipped();
}

void UDodEquipmentInstance::OnUnequipped()
{
	K2_OnUnequipped();
}

void UDodEquipmentInstance::SpawnEquipmentActors(const TArray<FDodEquipmentActorToSpawn>& ActorsToSpawn)
{
	if (APawn* OwningPawn = GetPawn())
	{
		USceneComponent* BodyMesh = OwningPawn->GetRootComponent();
		USkeletalMeshComponent* ArmMesh = nullptr;
		if (const ACharacter* Char = Cast<ACharacter>(OwningPawn))
		{
			BodyMesh = Char->GetMesh();
		}
		if (const ADodShooter* Char = Cast<ADodShooter>(OwningPawn))
		{
			ArmMesh = Char->ArmMesh;
		}

		for (const FDodEquipmentActorToSpawn& SpawnInfo : ActorsToSpawn)
		{
			AActor* NewActor = GetWorld()->SpawnActorDeferred<AActor>(SpawnInfo.ActorToSpawn,
			                                                          FTransform::Identity,
			                                                          OwningPawn);
			NewActor->FinishSpawning(FTransform::Identity, true);
			NewActor->SetActorRelativeTransform(SpawnInfo.AttachTransform);
			if (AWeaponBase* NewWeapon = Cast<AWeaponBase>(NewActor))
			{
				FAttachmentTransformRules AttachmentTransformRules(EAttachmentRule::KeepRelative,
				                                                   EAttachmentRule::KeepRelative,
				                                                   EAttachmentRule::KeepRelative,
				                                                   true);
				if (ArmMesh)
				{
					NewWeapon->VM_Receiver->AttachToComponent(ArmMesh,
					                                          AttachmentTransformRules,
					                                          SpawnInfo.ArmAttachSocket);
				}
				if (BodyMesh)
				{
					NewWeapon->WM_Receiver->AttachToComponent(BodyMesh,
					                                          AttachmentTransformRules,
					                                          SpawnInfo.BodyAttachSocket);
				}
			}
			else
			{
				NewActor->AttachToComponent(BodyMesh,
				                            FAttachmentTransformRules::KeepRelativeTransform,
				                            SpawnInfo.BodyAttachSocket);
			}
			SpawnedActors.Add(NewActor);
		}
	}
}

void UDodEquipmentInstance::DestroyEquipmentActors()
{
	for (AActor* Actor : SpawnedActors)
	{
		if (Actor)
		{
			Actor->Destroy();
		}
	}
}

void UDodEquipmentInstance::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, Instigator);
}

void UDodEquipmentInstance::OnRep_Instigator()
{
}
