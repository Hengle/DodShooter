#include "Player/AIController_Shooter.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/Comp/DodHealthComponent.h"
#include "Perception/AIPerceptionComponent.h"

AAIController_Shooter::AAIController_Shooter()
{
	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
}

void AAIController_Shooter::BeginPlay()
{
	Super::BeginPlay();
	RunBehaviorTree(BTAsset);
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
}

void AAIController_Shooter::OnUnPossess()
{
	OnDeathStarted(GetPawn());
	GetTeamChangedDelegateChecked().RemoveDynamic(this, &ThisClass::OnWatchedAgentChangedTeam);
	Super::OnUnPossess();
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
