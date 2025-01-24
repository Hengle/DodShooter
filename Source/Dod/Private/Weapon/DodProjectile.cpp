#include "Weapon/DodProjectile.h"

#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"


ADodProjectile::ADodProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	ProjectileCollision = CreateDefaultSubobject<UBoxComponent>(FName("ProjectileCollision"));
	SetRootComponent(ProjectileCollision);

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>("ProjectileMesh");
	ProjectileMesh->SetupAttachment(ProjectileCollision);
	ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovement->InitialSpeed = 10000.f;
}

void ADodProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
