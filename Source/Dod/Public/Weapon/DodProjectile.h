#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "GameFramework/Actor.h"
#include "DodProjectile.generated.h"

class UProjectileMovementComponent;
class UBoxComponent;

UCLASS()
class DOD_API ADodProjectile : public AActor
{
	GENERATED_BODY()

public:
	ADodProjectile();

	void InitializeProjectile(float InitialSpeed);

	virtual void Tick(float DeltaTime) override;

	void SweepDetection();

	UPROPERTY(BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
	FGameplayEffectSpecHandle DamageEffectSpecHandle;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStaticMeshComponent* ProjectileMesh;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UBoxComponent* ProjectileCollision;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UProjectileMovementComponent* ProjectileMovement;

protected:
	void HitItem(const FHitResult& HitResult);

private:
	FVector PreviousLocation{FVector::ZeroVector};
};
