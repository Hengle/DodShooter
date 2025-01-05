#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DodEquipmentDefinition.generated.h"

class UDodAttachmentInstance;
class UDodEquipmentInstance;
class UDodAbilitySet;

USTRUCT()
struct FDodAttachmentMeshDetail
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category=Equipment)
	TSoftObjectPtr<USkeletalMesh> ModelToSpawn;
	// 为空则自动根据根骨骼设置
	UPROPERTY(EditAnywhere, Category=Equipment)
	FName AttachSocket;
	// 一个武器一共0-9十个插槽，枪身的Idx为0
	UPROPERTY(EditAnywhere, Category=Equipment)
	int32 SocketIndex;
	UPROPERTY(EditAnywhere, Category=Equipment)
	FTransform AttachTransform;
};

USTRUCT()
struct FDodAttachmentMeshToSpawn
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category=Equipment)
	FDodAttachmentMeshDetail ViewModel;
	UPROPERTY(EditAnywhere, Category=Equipment)
	FDodAttachmentMeshDetail WorldModel;
};

USTRUCT()
struct FDodAttachmentInfo
{
	GENERATED_BODY()
	// 配件数据，添加到武器的Instance上
	UPROPERTY(EditDefaultsOnly, Category=Attachment)
	TSubclassOf<UDodAttachmentInstance> AttachmentInstance;
	// 配件网格体
	UPROPERTY(EditDefaultsOnly, Category=Attachment)
	FDodAttachmentMeshToSpawn AttachmentToSpawn;
	// 添加配件后附加的Ability
	UPROPERTY(EditDefaultsOnly, Category=Attachment)
	TArray<TObjectPtr<const UDodAbilitySet>> AbilitySetsToGrant;
};

USTRUCT()
struct FDodEquipmentActorToSpawn
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category=Equipment)
	TSubclassOf<AActor> ActorToSpawn;

	UPROPERTY(EditAnywhere, Category=Equipment)
	FName ArmAttachSocket;

	UPROPERTY(EditAnywhere, Category=Equipment)
	FName BodyAttachSocket;

	UPROPERTY(EditAnywhere, Category=Equipment)
	FTransform AttachTransform;
};

UCLASS(Blueprintable, Const, Abstract, BlueprintType)
class DOD_API UDodEquipmentDefinition : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category=Equipment)
	TSubclassOf<UDodEquipmentInstance> InstanceType;

	UPROPERTY(EditDefaultsOnly, Category=Equipment)
	TArray<TObjectPtr<const UDodAbilitySet>> AbilitySetsToGrant;

	UPROPERTY(EditDefaultsOnly, Category=Equipment)
	FDodEquipmentActorToSpawn ActorToSpawn;

	UPROPERTY(EditDefaultsOnly, Category=Weapon)
	TArray<FDodAttachmentInfo> Attachments;
};
