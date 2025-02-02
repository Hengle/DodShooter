#include "Character/DodOperator.h"

#include "Character/Comp/DodHealthComponent.h"
#include "Character/Comp/DodOperatorComponent.h"
#include "Equipment/DodEquipmentManagerComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Weapon/DodWeaponInstance.h"

ADodOperator::ADodOperator(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	OperatorComponent = CreateDefaultSubobject<UDodOperatorComponent>(TEXT("OperatorComponent"));

	HeadMesh->SetupAttachment(GetMesh(), TEXT("j_spine4"));
}

void ADodOperator::BeginPlay()
{
	Super::BeginPlay();
}

void ADodOperator::OnDeathStarted(AActor* OwningActor)
{
	Super::OnDeathStarted(OwningActor);
	// 取消AI对该角色的感知
	Ragdoll();
	HideEquippedWeapons();
}

void ADodOperator::Ragdoll()
{
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	GetMesh()->SetAllBodiesBelowSimulatePhysics(TEXT("j_mainroot"), true);
	FVector Velocity = GetCharacterMovement()->GetLastUpdateVelocity();
	FVector Dir = Velocity.GetSafeNormal();
	GetMesh()->AddImpulse(Velocity + Dir * RagdollImpulseStrength,TEXT("j_mainroot"), true);
}

void ADodOperator::HideEquippedWeapons()
{
	if (UDodEquipmentManagerComponent* EMC = GetComponentByClass<UDodEquipmentManagerComponent>())
	{
		TArray<UDodEquipmentInstance*> Items = EMC->GetEquipmentInstancesOfType(UDodWeaponInstance::StaticClass());
		for (UDodEquipmentInstance* Item : Items)
		{
			Item->GetSpawnedActor()->SetActorHiddenInGame(true);
		}
	}
}
