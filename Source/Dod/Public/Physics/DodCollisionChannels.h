#pragma once

// Trace against Actors/Components which provide interactions.
#define Dod_TraceChannel_Interaction				ECC_GameTraceChannel1

// Trace used by weapons, will hit physics assets instead of capsules
#define Dod_TraceChannel_Weapon						ECC_GameTraceChannel2

// Trace used by by weapons, will hit pawn capsules instead of physics assets
#define Dod_TraceChannel_Weapon_Capsule				ECC_GameTraceChannel3

// Trace used by by weapons, will trace through multiple pawns rather than stopping on the first hit
#define Dod_TraceChannel_Weapon_Multi				ECC_GameTraceChannel4
