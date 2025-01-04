#include "Character/DodShooter.h"

#include "Camera/DodCameraComponent.h"
#include "GameFramework/SpringArmComponent.h"


ADodShooter::ADodShooter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ArmMesh = CreateDefaultSubobject<USkeletalMeshComponent>("ArmMesh");
	ArmMesh->SetupAttachment(SpringArmComponent);

	CameraComponent->SetupAttachment(ArmMesh, TEXT("tag_camera"));
}

void ADodShooter::ChangeToFirstPerson()
{
	if (!IsLocallyControlled())
	{
		return;
	}
	HeadMesh->SetVisibility(false);
	GetMesh()->SetVisibility(false);
	ArmMesh->SetVisibility(true);
}

void ADodShooter::ChangeToThirdPerson()
{
	HeadMesh->SetVisibility(true);
	GetMesh()->SetVisibility(true);
	ArmMesh->SetVisibility(false);
}

void ADodShooter::BeginPlay()
{
	Super::BeginPlay();

	HeadMesh->SetCastShadow(true);
	GetMesh()->SetCastShadow(true);
	ArmMesh->SetCastShadow(false);

	if (IsLocallyControlled())
	{
		ChangeToFirstPerson();
	}
	else
	{
		ChangeToThirdPerson();
	}
}

void ADodShooter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (!HasAuthority())
	{
		return;
	}
	AddInitialInventory();
}

void ADodShooter::AddInitialInventory()
{
	if (!HasAuthority())
	{
		return;
	}
}
