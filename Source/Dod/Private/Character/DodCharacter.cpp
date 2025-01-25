#include "Character/DodCharacter.h"

#include "AbilitySystem/DodAbilitySystemComponent.h"
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

	CameraComponent = CreateDefaultSubobject<UDodCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);

	HeadMesh = CreateDefaultSubobject<USkeletalMeshComponent>("Head");

	PawnExtComponent = CreateDefaultSubobject<UDodPawnExtensionComponent>(TEXT("PawnExt"));
	PawnExtComponent->OnAbilitySystemInitialized_RegisterAndCall(
		FSimpleMulticastDelegate::FDelegate::CreateUObject(this, &ThisClass::OnAbilitySystemInitialized));
	PawnExtComponent->OnAbilitySystemUninitialized_Register(
		FSimpleMulticastDelegate::FDelegate::CreateUObject(this, &ThisClass::OnAbilitySystemUninitialized));

	HealthComponent = CreateDefaultSubobject<UDodHealthComponent>(TEXT("HealthComponent"));

	StaminaComponent = CreateDefaultSubobject<UDodStaminaComponent>(TEXT("StaminaComponent"));

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;
}

UDodAbilitySystemComponent* ADodCharacter::GetDodAbilitySystemComponent() const
{
	return Cast<UDodAbilitySystemComponent>(GetAbilitySystemComponent());
}

UAbilitySystemComponent* ADodCharacter::GetAbilitySystemComponent() const
{
	if (PawnExtComponent == nullptr)
	{
		return nullptr;
	}

	return PawnExtComponent->GetDodAbilitySystemComponent();
}

void ADodCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ADodCharacter::OnAbilitySystemInitialized()
{
	UDodAbilitySystemComponent* ASC = GetDodAbilitySystemComponent();
	check(ASC);

	HealthComponent->InitializeWithAbilitySystem(ASC);

	InitializeGameplayTags();
}

void ADodCharacter::OnAbilitySystemUninitialized()
{
	HealthComponent->UninitializeFromAbilitySystem();
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

void ADodCharacter::InitializeGameplayTags()
{
	if (UDodAbilitySystemComponent* ASC = GetDodAbilitySystemComponent())
	{
	}
}
