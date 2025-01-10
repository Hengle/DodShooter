#include "Weapon/AttachmentBase.h"


AAttachmentBase::AAttachmentBase()
{
	Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);

	VM_Att = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("VM_Att"));
	VM_Att->SetupAttachment(RootComponent);
	VM_Att->SetCastShadow(false);
	WM_Att = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WM_Att"));
	WM_Att->SetupAttachment(RootComponent);
	WM_Att->SetCastShadow(true);
	WM_Att->SetCastHiddenShadow(true);
}

void AAttachmentBase::BeginPlay()
{
	Super::BeginPlay();
	
}