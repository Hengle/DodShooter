#include "Bot/Services/BTS_SetFocus.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

void UBTS_SetFocus::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	if (bSetFocus)
	{
		UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
		if (Blackboard)
		{
			AActor* TargetActor = Cast<AActor>(Blackboard->GetValueAsObject(TargetEnemy.SelectedKeyName));
			AIOwner->SetFocus(TargetActor);
		}
	}
	else
	{
		AIOwner->ClearFocus(EAIFocusPriority::Gameplay);
	}
}
