#include "Messages/DodVerbMessage.h"

FString FDodVerbMessage::ToString() const
{
	FString HumanReadableMessage;
	FDodVerbMessage::StaticStruct()->ExportText(/*out*/ HumanReadableMessage, this, /*Defaults=*/ nullptr,
	                                                    /*OwnerObject=*/ nullptr, PPF_None, /*ExportRootScope=*/
	                                                    nullptr);
	return HumanReadableMessage;
}
