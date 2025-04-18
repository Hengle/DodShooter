﻿#include "Performance/DodPerformanceStatSubsystem.h"

#include "GameFramework/PlayerState.h"
#include "GameMode/GameState/DodGameState.h"
#include "Performance/DodPerformanceStatTypes.h"

void FDodPerformanceStatCache::StartCharting()
{
}

void FDodPerformanceStatCache::ProcessFrame(const FFrameData& FrameData)
{
	CachedData = FrameData;
	CachedServerFPS = 0.0f;
	CachedPingMS = 0.0f;
	CachedPacketLossIncomingPercent = 0.0f;
	CachedPacketLossOutgoingPercent = 0.0f;
	CachedPacketRateIncoming = 0.0f;
	CachedPacketRateOutgoing = 0.0f;
	CachedPacketSizeIncoming = 0.0f;
	CachedPacketSizeOutgoing = 0.0f;

	if (UWorld* World = MySubsystem->GetGameInstance()->GetWorld())
	{
		if (const ADodGameState* GameState = World->GetGameState<ADodGameState>())
		{
			CachedServerFPS = GameState->GetServerFPS();
		}

		if (APlayerController* LocalPC = GEngine->GetFirstLocalPlayerController(World))
		{
			if (APlayerState* PS = LocalPC->GetPlayerState<APlayerState>())
			{
				CachedPingMS = PS->GetPingInMilliseconds();
			}

			if (UNetConnection* NetConnection = LocalPC->GetNetConnection())
			{
				const UNetConnection::FNetConnectionPacketLoss& InLoss = NetConnection->GetInLossPercentage();
				CachedPacketLossIncomingPercent = InLoss.GetAvgLossPercentage();
				const UNetConnection::FNetConnectionPacketLoss& OutLoss = NetConnection->GetOutLossPercentage();
				CachedPacketLossOutgoingPercent = OutLoss.GetAvgLossPercentage();

				CachedPacketRateIncoming = NetConnection->InPacketsPerSecond;
				CachedPacketRateOutgoing = NetConnection->OutPacketsPerSecond;

				CachedPacketSizeIncoming = (NetConnection->InPacketsPerSecond != 0)
					                           ? (NetConnection->InBytesPerSecond / (float)NetConnection->
						                           InPacketsPerSecond)
					                           : 0.0f;
				CachedPacketSizeOutgoing = (NetConnection->OutPacketsPerSecond != 0)
					                           ? (NetConnection->OutBytesPerSecond / (float)NetConnection->
						                           OutPacketsPerSecond)
					                           : 0.0f;
			}
		}
	}
}

void FDodPerformanceStatCache::StopCharting()
{
}

double FDodPerformanceStatCache::GetCachedStat(EDodDisplayablePerformanceStat Stat) const
{
	static_assert((int32)EDodDisplayablePerformanceStat::Count == 15,
	              "Need to update this function to deal with new performance stats");
	switch (Stat)
	{
	case EDodDisplayablePerformanceStat::ClientFPS:
		return (CachedData.TrueDeltaSeconds != 0.0) ? (1.0 / CachedData.TrueDeltaSeconds) : 0.0;
	case EDodDisplayablePerformanceStat::ServerFPS:
		return CachedServerFPS;
	case EDodDisplayablePerformanceStat::IdleTime:
		return CachedData.IdleSeconds;
	case EDodDisplayablePerformanceStat::FrameTime:
		return CachedData.TrueDeltaSeconds;
	case EDodDisplayablePerformanceStat::FrameTime_GameThread:
		return CachedData.GameThreadTimeSeconds;
	case EDodDisplayablePerformanceStat::FrameTime_RenderThread:
		return CachedData.RenderThreadTimeSeconds;
	case EDodDisplayablePerformanceStat::FrameTime_RHIThread:
		return CachedData.RHIThreadTimeSeconds;
	case EDodDisplayablePerformanceStat::FrameTime_GPU:
		return CachedData.GPUTimeSeconds;
	case EDodDisplayablePerformanceStat::Ping:
		return CachedPingMS;
	case EDodDisplayablePerformanceStat::PacketLoss_Incoming:
		return CachedPacketLossIncomingPercent;
	case EDodDisplayablePerformanceStat::PacketLoss_Outgoing:
		return CachedPacketLossOutgoingPercent;
	case EDodDisplayablePerformanceStat::PacketRate_Incoming:
		return CachedPacketRateIncoming;
	case EDodDisplayablePerformanceStat::PacketRate_Outgoing:
		return CachedPacketRateOutgoing;
	case EDodDisplayablePerformanceStat::PacketSize_Incoming:
		return CachedPacketSizeIncoming;
	case EDodDisplayablePerformanceStat::PacketSize_Outgoing:
		return CachedPacketSizeOutgoing;
	default:
		break;
	}

	return 0.0f;
}

double UDodPerformanceStatSubsystem::GetCachedStat(EDodDisplayablePerformanceStat Stat) const
{
	return Tracker->GetCachedStat(Stat);
}

void UDodPerformanceStatSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UDodPerformanceStatSubsystem::Deinitialize()
{
	Super::Deinitialize();
}
