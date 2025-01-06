#include "Equipment/DodEquipmentInstance.h"

#include "Character/DodShooter.h"
#include "Equipment/DodEquipmentDefinition.h"
#include "GameFramework/Character.h"
#include "Iris/ReplicationSystem/ReplicationFragmentUtil.h"
#include "Net/UnrealNetwork.h"
#include "Weapon/WeaponBase.h"

UWorld* UDodEquipmentInstance::GetWorld() const
{
	if (APawn* OwningPawn = GetPawn())
	{
		return OwningPawn->GetWorld();
	}
	return nullptr;
}

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

void UDodEquipmentInstance::SpawnEquipmentActors(const FDodEquipmentActorToSpawn& ActorToSpawn)
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


		AActor* NewActor = GetWorld()->SpawnActorDeferred<AActor>(ActorToSpawn.ActorToSpawn,
		                                                          FTransform::Identity,
		                                                          OwningPawn);
		NewActor->FinishSpawning(FTransform::Identity, true);
		NewActor->SetActorRelativeTransform(ActorToSpawn.AttachTransform);
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
				                                          ActorToSpawn.ArmAttachSocket);
			}
			if (BodyMesh)
			{
				NewWeapon->WM_Receiver->AttachToComponent(BodyMesh,
				                                          AttachmentTransformRules,
				                                          ActorToSpawn.BodyAttachSocket);
			}
		}
		else
		{
			NewActor->AttachToComponent(BodyMesh,
			                            FAttachmentTransformRules::KeepRelativeTransform,
			                            ActorToSpawn.BodyAttachSocket);
		}
		SpawnedActor = NewActor;
	}
}

void UDodEquipmentInstance::DestroyEquipmentActors()
{
	if (SpawnedActor)
	{
		SpawnedActor->Destroy();
	}
}

void UDodEquipmentInstance::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, Instigator);
	DOREPLIFETIME(ThisClass, SpawnedActor);
}
#if UE_WITH_IRIS
void UDodEquipmentInstance::RegisterReplicationFragments(UE::Net::FFragmentRegistrationContext& Context,
                                                         UE::Net::EFragmentRegistrationFlags RegistrationFlags)
{
	using namespace UE::Net;
	FReplicationFragmentUtil::CreateAndRegisterFragmentsForObject(this, Context, RegistrationFlags);
}
#endif // UE_WITH_IRIS
void UDodEquipmentInstance::OnRep_Instigator()
{
}
