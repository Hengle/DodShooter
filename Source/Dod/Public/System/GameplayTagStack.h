#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "UObject/Object.h"
#include "GameplayTagStack.generated.h"

struct FGameplayTagStackContainer;

/* FGameplayTagStack
 * 标签和标签存储的数量
 */
USTRUCT(BlueprintType)
struct FGameplayTagStack : public FFastArraySerializerItem
{
	GENERATED_BODY()

	FGameplayTagStack()
	{
	}

	FGameplayTagStack(FGameplayTag InTag, int32 InStackCount)
		: Tag(InTag)
		  , StackCount(InStackCount)
	{
	}

private:
	friend FGameplayTagStackContainer;

	UPROPERTY()
	FGameplayTag Tag;

	UPROPERTY()
	int32 StackCount = 0;
};

/* FGameplayTagStackContainer
 * 存储一系列标签，每个标签也可以存储一个数值
 */
USTRUCT(BlueprintType)
struct FGameplayTagStackContainer : public FFastArraySerializer
{
	GENERATED_BODY()

	// 添加一个Tag和它的数值
	void AddStack(FGameplayTag Tag, int32 StackCount);

	// 移除一个Tag和它的一些数值
	void RemoveStack(FGameplayTag Tag, int32 StackCount);

	// 返回Tag的数值
	int32 GetStackCount(FGameplayTag Tag) const
	{
		return TagToCountMap.FindRef(Tag);
	}

	// 返回是否保存了这个Tag
	bool ContainsTag(FGameplayTag Tag) const
	{
		return TagToCountMap.Contains(Tag);
	}

	//~FFastArraySerializer contract
	void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize);
	void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
	void PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize);
	//~End of FFastArraySerializer contract

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FGameplayTagStack, FGameplayTagStackContainer>(
			Stacks, DeltaParms, *this);
	}

private:
	UPROPERTY()
	TArray<FGameplayTagStack> Stacks;

	TMap<FGameplayTag, int32> TagToCountMap;
};

template <>
struct TStructOpsTypeTraits<FGameplayTagStackContainer> : public TStructOpsTypeTraitsBase2<FGameplayTagStackContainer>
{
	enum
	{
		WithNetDeltaSerializer = true,
	};
};
