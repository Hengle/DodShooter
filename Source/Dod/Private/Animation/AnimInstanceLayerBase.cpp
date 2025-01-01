#include "AnimInstanceLayerBase.h"

#include "Animation/DodAnimInstance.h"

const UDodAnimInstance* UAnimInstanceLayerBase::GetDodAnimInstance() const
{
	return Cast<UDodAnimInstance>(GetOwningComponent()->GetAnimInstance());
}
