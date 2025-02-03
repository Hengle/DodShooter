#include "Weapon/DodProjectile.h"

#include "AbilitySystemComponent.h"
#include "Character/DodCharacter.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Physics/DodCollisionChannels.h"
#include "Player/DodPlayerState.h"
#include "Weapon/DodWeaponStateComponent.h"


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
	ProjectileMovement->InitialSpeed = 0.f;
	ProjectileMovement->bShouldBounce = false;
}

void ADodProjectile::InitializeProjectile(float InitialSpeed)
{
	FVector ShootDirection = GetActorRotation().Quaternion().GetForwardVector().GetSafeNormal();
	if (ProjectileMovement)
	{
		ProjectileMovement->Velocity = ShootDirection * InitialSpeed;
	}
	PreviousLocation = GetActorLocation();
}

void ADodProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SweepDetection();
}

void ADodProjectile::SweepDetection()
{
	FVector CurrentLocation = GetActorLocation();

	if (PreviousLocation.IsZero())
	{
		PreviousLocation = CurrentLocation;
		return;
	}

	FVector MovementDirection = CurrentLocation - PreviousLocation;
	MovementDirection.Normalize();

	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	CollisionParams.AddIgnoredActor(GetOwner());
	CollisionParams.AddIgnoredActor(GetInstigator());
	CollisionParams.bReturnPhysicalMaterial = true;

	bool bHit = GetWorld()->SweepSingleByChannel(
		HitResult,
		PreviousLocation,
		CurrentLocation,
		FQuat::Identity,
		Dod_TraceChannel_Weapon,
		FCollisionShape::MakeBox(ProjectileCollision->GetScaledBoxExtent()),
		CollisionParams
	);

	if (bHit)
	{
		HitItem(HitResult);
	}

	PreviousLocation = CurrentLocation;
}

void ADodProjectile::HitItem(const FHitResult& HitResult)
{
	if (!HasAuthority())
	{
		return;
	}
	ADodCharacter* HitActor = Cast<ADodCharacter>(HitResult.GetActor());
	if (!HitActor)
	{
		return;
	}
	UAbilitySystemComponent* ASC = HitActor->GetAbilitySystemComponent();
	if (!ASC || !DamageEffectSpecHandle.IsValid())
	{
		return;
	}

	FGameplayEffectContextHandle EffectContextHandle = DamageEffectSpecHandle.Data->GetContext();
	if (EffectContextHandle.IsValid())
	{
		FGameplayEffectContext* EffectContext = EffectContextHandle.Get();
		if (EffectContext)
		{
			EffectContext->AddHitResult(HitResult);
		}
	}

	ASC->ApplyGameplayEffectSpecToSelf(*DamageEffectSpecHandle.Data.Get());

	ADodCharacter* InstigatorCharacter = Cast<ADodCharacter>(GetInstigator());
	if (!InstigatorCharacter)
	{
		return;
	}

	AController* InstigatorController = InstigatorCharacter->GetController();
	if (!InstigatorController)
	{
		return;
	}

	if (InstigatorController->GetLocalRole() == ROLE_Authority)
	{
		if (UDodWeaponStateComponent* WeaponStateComponent =
			InstigatorController->FindComponentByClass<UDodWeaponStateComponent>())
		{
			WeaponStateComponent->Client_ConfirmTargetHit(true);
		}
	}
	Destroy();
}
