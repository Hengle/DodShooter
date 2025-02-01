#include "Weapon/DodProjectile.h"

#include "AbilitySystemComponent.h"
#include "Character/DodCharacter.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
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
	ProjectileMovement->InitialSpeed = 10000.f;
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

	bool bHit = GetWorld()->SweepSingleByChannel(
		HitResult,
		PreviousLocation,
		CurrentLocation,
		FQuat::Identity,
		ECC_Camera,
		FCollisionShape::MakeBox(ProjectileCollision->GetScaledBoxExtent()),
		CollisionParams
	);

	if (bHit)
	{
		if (HasAuthority())
		{
			ADodCharacter* HitActor = Cast<ADodCharacter>(HitResult.GetActor());
			if (HitActor)
			{
				if (UAbilitySystemComponent* ASC = HitActor->GetAbilitySystemComponent())
				{
					if (DamageEffectSpecHandle.IsValid())
					{
						ASC->ApplyGameplayEffectSpecToSelf(*DamageEffectSpecHandle.Data.Get());
						if (ADodCharacter* Character = Cast<ADodCharacter>(GetInstigator()))
						{
							if (AController* PC = Character->GetController())
							{
								UDodWeaponStateComponent* WSC = PC->FindComponentByClass<UDodWeaponStateComponent>();
								if (WSC)
								{
									int32 UniqueId = WSC->GetUnconfirmedServerSideHitMarkerCount();
									Client_ShowHitMarker(UniqueId, HitResult);
								}
							}
						}
					}
				}
			}
		}
		Destroy();
	}

	PreviousLocation = CurrentLocation;
}

void ADodProjectile::Client_ShowHitMarker_Implementation(int32 UniqueId, const FHitResult& HitResult)
{
	if (ADodCharacter* Character = Cast<ADodCharacter>(GetInstigator()))
	{
		if (AController* PC = Character->GetController())
		{
			UDodWeaponStateComponent* WSC = PC->FindComponentByClass<UDodWeaponStateComponent>();
			if (WSC)
			{
				FGameplayAbilityTargetDataHandle TargetData;
				TargetData.UniqueId = UniqueId;
				TArray<FHitResult> FoundHits;
				FoundHits.Add(HitResult);
				WSC->AddUnconfirmedServerSideHitMarkers(TargetData, FoundHits);
			}
		}
	}
}
