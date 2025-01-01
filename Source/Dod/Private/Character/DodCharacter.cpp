#include "Character/DodCharacter.h"

#include "Camera/CameraComponent.h"
#include "Camera/DodCameraComponent.h"
#include "Character/DodCharacterMovementComp.h"
#include "Character/Comp/DodHealthComponent.h"
#include "Character/Comp/DodPawnExtensionComponent.h"
#include "Character/Comp/DodStaminaComponent.h"
#include "GameFramework/SpringArmComponent.h"

ADodCharacter::ADodCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UDodCharacterMovementComp>(CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	USkeletalMeshComponent* MeshComp = GetMesh();
	check(MeshComp);
	MeshComp->SetRelativeLocation(FVector(0.f, 0.f, -90.f));

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->TargetArmLength = 0.f;
	SpringArmComponent->SetRelativeLocation(FVector(0.f, 0.f, 60.f));
	SpringArmComponent->bUsePawnControlRotation = true;

	ArmMesh = CreateDefaultSubobject<USkeletalMeshComponent>("ArmMesh");
	ArmMesh->SetupAttachment(SpringArmComponent);

	CameraComponent = CreateDefaultSubobject<UDodCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(ArmMesh, TEXT("tag_camera"));

	HeadMesh = CreateDefaultSubobject<USkeletalMeshComponent>("Head");
	HeadMesh->SetupAttachment(MeshComp, TEXT("j_spine4"));

	PawnExtComponent = CreateDefaultSubobject<UDodPawnExtensionComponent>(TEXT("PawnExt"));

	HealthComponent = CreateDefaultSubobject<UDodHealthComponent>(TEXT("HealthComponent"));

	StaminaComponent = CreateDefaultSubobject<UDodStaminaComponent>(TEXT("StaminaComponent"));

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;
}

void ADodCharacter::BeginPlay()
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

void ADodCharacter::ChangeToFirstPerson()
{
	if (!IsLocallyControlled())
	{
		return;
	}
	HeadMesh->SetVisibility(false);
	GetMesh()->SetVisibility(false);
	ArmMesh->SetVisibility(true);
}

void ADodCharacter::ChangeToThirdPerson()
{
	HeadMesh->SetVisibility(true);
	GetMesh()->SetVisibility(true);
	ArmMesh->SetVisibility(false);
}

void ADodCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	PawnExtComponent->HandleControllerChanged();
}

void ADodCharacter::UnPossessed()
{
	Super::UnPossessed();
	
	PawnExtComponent->HandleControllerChanged();
}

void ADodCharacter::OnRep_Controller()
{
	Super::OnRep_Controller();

	PawnExtComponent->HandleControllerChanged();
}

void ADodCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	PawnExtComponent->HandlePlayerStateReplicated();
}

void ADodCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PawnExtComponent->SetupPlayerInputComponent();
}
