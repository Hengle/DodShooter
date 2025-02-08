#include "Player/AIController_Shooter.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/Comp/DodHealthComponent.h"
#include "Equipment/DodQuickBarComponent.h"
#include "GameMode/DodExperienceManagerComponent.h"
#include "Inventory/DodInventoryManagerComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Weapon/DodWeaponStateComponent.h"

AAIController_Shooter::AAIController_Shooter()
{
	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
}

void AAIController_Shooter::BeginPlay()
{
	Super::BeginPlay();

	UDodExperienceManagerComponent* ExperienceComponent =
	GetWorld()->GetGameState()->FindComponentByClass<UDodExperienceManagerComponent>();
	ExperienceComponent->CallOrRegister_OnExperienceLoaded(
		FOnDodExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::HandleExperienceLoaded));
}

void AAIController_Shooter::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (UDodHealthComponent* HealthComponent = InPawn->GetComponentByClass<UDodHealthComponent>())
	{
		HealthComponent->OnDeathStarted.AddDynamic(this, &ThisClass::OnDeathStarted);
	}
	if (BrainComponent)
	{
		BrainComponent->StartLogic();
	}
	GetTeamChangedDelegateChecked().AddDynamic(this, &ThisClass::OnWatchedAgentChangedTeam);
	if (GetGenericTeamId() != -1)
	{
		OnWatchedAgentChangedTeam(nullptr, -1, GetGenericTeamId());
	}
}

void AAIController_Shooter::OnUnPossess()
{
	OnDeathStarted(GetPawn());
	GetTeamChangedDelegateChecked().RemoveDynamic(this, &ThisClass::OnWatchedAgentChangedTeam);
	Super::OnUnPossess();
}

void AAIController_Shooter::HandleExperienceLoaded(const UDodExperienceDefinition* CurrentExperience)
{
	RunBehaviorTree(BTAsset);
}

void AAIController_Shooter::OnWatchedAgentChangedTeam(UObject* TeamAgent, int32 OldTeam, int32 NewTeam)
{
	AIPerception->RequestStimuliListenerUpdate();
}

void AAIController_Shooter::OnDeathStarted(AActor* OwningActor)
{
	GetBlackboardComponent()->ClearValue(EnemyNameKey);
	BrainComponent->StopLogic(TEXT("Dead"));
}
