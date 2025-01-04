#include "Character/Comp/DodOperatorComponent.h"

#include "DodGameplayTags.h"
#include "InputActionValue.h"
#include "Character/DodCharacter.h"
#include "Player/DodLocalPlayer.h"
#include "EnhancedInputSubsystems.h"
#include "Character/Comp/DodPawnExtensionComponent.h"
#include "Input/DodInputComponent.h"
#include "Input/Common/InputMappingContextAndPriority.h"
#include "UserSettings/EnhancedInputUserSettings.h"
#include "InputMappingContext.h"
#include "Components/GameFrameworkComponentManager.h"
#include "GameFramework/PlayerState.h"
#include "Player/DodPlayerController.h"
#include "Player/DodPlayerState.h"

const FName UDodOperatorComponent::NAME_ActorFeatureName("Operator");

UDodOperatorComponent::UDodOperatorComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

bool UDodOperatorComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState,
                                               FGameplayTag DesiredState) const
{
	check(Manager);

	APawn* Pawn = GetPawn<APawn>();

	if (!CurrentState.IsValid() && DesiredState == DodGameplayTags::InitState_Spawned)
	{
		if (Pawn)
		{
			return true;
		}
	}
	else if (CurrentState == DodGameplayTags::InitState_Spawned &&
		DesiredState == DodGameplayTags::InitState_DataAvailable)
	{
		if (!GetPlayerState<ADodPlayerState>())
		{
			return false;
		}

		if (Pawn->GetLocalRole() != ROLE_SimulatedProxy)
		{
			AController* Controller = GetController<AController>();

			const bool bHasControllerPairedWithPS =
				Controller && Controller->PlayerState && Controller->PlayerState->GetOwner() == Controller;

			if (!bHasControllerPairedWithPS)
			{
				return false;
			}
		}

		const bool bIsLocallyControlled = Pawn->IsLocallyControlled();
		const bool bIsBot = Pawn->IsBotControlled();

		if (bIsLocallyControlled && !bIsBot)
		{
			ADodPlayerController* PC = GetController<ADodPlayerController>();

			if (!Pawn->InputComponent || !PC || !PC->GetLocalPlayer())
			{
				return false;
			}
		}

		return true;
	}
	else if (CurrentState == DodGameplayTags::InitState_DataAvailable &&
		DesiredState == DodGameplayTags::InitState_DataInitialized)
	{
		ADodPlayerState* PS = GetPlayerState<ADodPlayerState>();

		return PS && Manager->HasFeatureReachedInitState(Pawn, UDodPawnExtensionComponent::NAME_ActorFeatureName,
		                                                 DodGameplayTags::InitState_DataInitialized);
	}
	else if (CurrentState == DodGameplayTags::InitState_DataInitialized && DesiredState ==
		DodGameplayTags::InitState_GameplayReady)
	{
		return true;
	}

	return false;
}

void UDodOperatorComponent::HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState,
                                                  FGameplayTag DesiredState)
{
	if (CurrentState == DodGameplayTags::InitState_DataAvailable &&
		DesiredState == DodGameplayTags::InitState_DataInitialized)
	{
		APawn* Pawn = GetPawn<APawn>();
		ADodPlayerState* PS = GetPlayerState<ADodPlayerState>();
		if (!ensure(Pawn && PS))
		{
			return;
		}

		if (UDodPawnExtensionComponent* PawnExtComp = UDodPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
		{
			PawnExtComp->InitializeAbilitySystem(PS->GetDodAbilitySystemComponent(), PS);
		}

		if (GetController<ADodPlayerController>())
		{
			if (Pawn->InputComponent != nullptr)
			{
				InitializePlayerInput(Pawn->InputComponent);
			}
		}
	}
}

void UDodOperatorComponent::OnActorInitStateChanged(const FActorInitStateChangedParams& Params)
{
	if (Params.FeatureName == UDodPawnExtensionComponent::NAME_ActorFeatureName)
	{
		if (Params.FeatureState == DodGameplayTags::InitState_DataInitialized)
		{
			CheckDefaultInitialization();
		}
	}
}

void UDodOperatorComponent::CheckDefaultInitialization()
{
	static const TArray<FGameplayTag> StateChain = {
		DodGameplayTags::InitState_Spawned,
		DodGameplayTags::InitState_DataAvailable,
		DodGameplayTags::InitState_DataInitialized,
		DodGameplayTags::InitState_GameplayReady
	};

	ContinueInitStateChain(StateChain);
}

void UDodOperatorComponent::BeginPlay()
{
	Super::BeginPlay();

	BindOnActorInitStateChanged(UDodPawnExtensionComponent::NAME_ActorFeatureName, FGameplayTag(), false);

	ensure(TryToChangeInitState(DodGameplayTags::InitState_Spawned));
	CheckDefaultInitialization();
}

void UDodOperatorComponent::InitializePlayerInput(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return;
	}

	const APlayerController* PC = GetController<APlayerController>();
	check(PC);

	const UDodLocalPlayer* LP = Cast<UDodLocalPlayer>(PC->GetLocalPlayer());
	check(LP);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);

	Subsystem->ClearAllMappings();

	if (const UDodPawnExtensionComponent* PawnExtComp = UDodPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
	{
		const FDodPawnData& PawnData = PawnExtComp->GetPawnData();
		if (const UDodInputConfig* InputConfig = PawnData.InputConfig)
		{
			for (const FInputMappingContextAndPriority& Mapping : DefaultInputMappings)
			{
				if (UInputMappingContext* IMC = Mapping.InputMapping.LoadSynchronous())
				{
					if (Mapping.bRegisterWithSettings)
					{
						if (UEnhancedInputUserSettings* Settings = Subsystem->GetUserSettings())
						{
							Settings->RegisterInputMappingContext(IMC);
						}

						FModifyContextOptions Options = {};
						Options.bIgnoreAllPressedKeysUntilRelease = false;
						Subsystem->AddMappingContext(IMC, Mapping.Priority, Options);
					}
				}
			}

			if (UDodInputComponent* IC = Cast<UDodInputComponent>(PlayerInputComponent))
			{
				TArray<uint32> BindHandles;
				IC->BindAbilityActions(InputConfig, this, &ThisClass::Input_AbilityInputTagPressed,
				                       &ThisClass::Input_AbilityInputTagReleased, /*out*/ BindHandles);

				IC->BindNativeAction(InputConfig, DodGameplayTags::InputTag_Move, ETriggerEvent::Triggered, this,
				                     &ThisClass::Input_Move);
				IC->BindNativeAction(InputConfig, DodGameplayTags::InputTag_Look_Mouse, ETriggerEvent::Triggered, this,
				                     &ThisClass::Input_LookMouse);
				IC->BindNativeAction(InputConfig, DodGameplayTags::InputTag_Look_Stick, ETriggerEvent::Triggered, this,
				                     &ThisClass::Input_LookStick);
				IC->BindNativeAction(InputConfig, DodGameplayTags::InputTag_Crouch, ETriggerEvent::Triggered, this,
				                     &ThisClass::Input_Crouch);
				IC->BindNativeAction(InputConfig, DodGameplayTags::InputTag_AutoRun, ETriggerEvent::Triggered, this,
				                     &ThisClass::Input_AutoRun);
			}
		}
	}
}

void UDodOperatorComponent::Input_AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (const APawn* Pawn = GetPawn<APawn>())
	{
		if (const UDodPawnExtensionComponent* PawnExtComp =
			UDodPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
		{
			/*if (UDodAbilitySystemComponent* LyraASC = PawnExtComp->GetDodAbilitySystemComponent())
			{
				LyraASC->AbilityInputTagPressed(InputTag);
			}*/
		}
	}
}

void UDodOperatorComponent::Input_AbilityInputTagReleased(FGameplayTag InputTag)
{
	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return;
	}

	if (const UDodPawnExtensionComponent* PawnExtComp = UDodPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
	{
		/*if (UDodAbilitySystemComponent* ASC = PawnExtComp->GetDodAbilitySystemComponent())
		{
			ASC->AbilityInputTagReleased(InputTag);
		}*/
	}
}

void UDodOperatorComponent::Input_Move(const FInputActionValue& InputActionValue)
{
	APawn* Pawn = GetPawn<APawn>();
	AController* Controller = Pawn ? Pawn->GetController() : nullptr;

	/*if (ALyraPlayerController* LyraController = Cast<ALyraPlayerController>(Controller))
	{
		LyraController->SetIsAutoRunning(false);
	}*/

	if (Controller)
	{
		const FVector2D Value = InputActionValue.Get<FVector2D>();
		const FRotator MovementRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);

		if (Value.X != 0.0f)
		{
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::RightVector);
			Pawn->AddMovementInput(MovementDirection, Value.X);
		}

		if (Value.Y != 0.0f)
		{
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::ForwardVector);
			Pawn->AddMovementInput(MovementDirection, Value.Y);
		}
	}
}

void UDodOperatorComponent::Input_LookMouse(const FInputActionValue& InputActionValue)
{
	APawn* Pawn = GetPawn<APawn>();

	if (!Pawn)
	{
		return;
	}

	const FVector2D Value = InputActionValue.Get<FVector2D>();

	if (Value.X != 0.0f)
	{
		Pawn->AddControllerYawInput(Value.X);
	}

	if (Value.Y != 0.0f)
	{
		Pawn->AddControllerPitchInput(Value.Y);
	}
}

void UDodOperatorComponent::Input_LookStick(const FInputActionValue& InputActionValue)
{
	APawn* Pawn = GetPawn<APawn>();

	if (!Pawn)
	{
		return;
	}

	const FVector2D Value = InputActionValue.Get<FVector2D>();

	const UWorld* World = GetWorld();
	check(World);

	if (Value.X != 0.0f)
	{
		Pawn->AddControllerYawInput(Value.X * 300.f * World->GetDeltaSeconds());
	}

	if (Value.Y != 0.0f)
	{
		Pawn->AddControllerPitchInput(Value.Y * 165.f * World->GetDeltaSeconds());
	}
}

void UDodOperatorComponent::Input_Crouch(const FInputActionValue& InputActionValue)
{
	if (ADodCharacter* Character = GetPawn<ADodCharacter>())
	{
		// Character->ToggleCrouch();
	}
}

void UDodOperatorComponent::Input_AutoRun(const FInputActionValue& InputActionValue)
{
	if (APawn* Pawn = GetPawn<APawn>())
	{
		/*if (ALyraPlayerController* Controller = Cast<ALyraPlayerController>(Pawn->GetController()))
		{
			Controller->SetIsAutoRunning(!Controller->GetIsAutoRunning());
		}*/
	}
}
