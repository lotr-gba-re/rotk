#pragma once

#include "mission.h"
#include "types.h"

/** ACTOR_TYPE_NPC payload (12 bytes total). */
typedef struct ActorNpcMissionParams
{
    ActorMissionParams head;
    u8 kind; // enum NpcKind (-> Actor.variant); the companion slots are remapped per
             // active hero by actor_npc_setup
    u8 animationVariant;
    u8 linkGroup;
    u8 linkStream;
} ActorNpcMissionParams;

/**
 * Kinds of ACTOR_TYPE_NPC actors (Actor.variant): 0..7 mirror enum HeroId; the
 * companion slots are resolved to a concrete hero by actor_npc_setup. Unlisted values
 * are unmapped.
 */
enum NpcKind
{
    NPC_KIND_FRODO = 0x00,
    NPC_KIND_LEGOLAS = 0x01,
    NPC_KIND_ARAGORN = 0x02,
    NPC_KIND_GANDALF = 0x03,
    NPC_KIND_EOWYN = 0x04,
    NPC_KIND_GIMLI = 0x05,
    NPC_KIND_SAM = 0x06,
    NPC_KIND_SMEAGOL = 0x07,
    NPC_KIND_GOLLUM = 0x09,      // inferred from the Frodo companion remap
    NPC_KIND_COMPANION_A = 0x0a, // resolved per the active hero (Frodo -> Sam, ...)
    NPC_KIND_COMPANION_B = 0x0b, // second companion (Frodo -> Gollum, ...)
    NPC_KIND_SOLDIER = 0x0c,     // generic escort soldier; the only HP-damageable kind
    NPC_KIND_CIVILIAN_WOMAN = 0x0f,
    NPC_KIND_SIEGE_A = 0x16, // siege-scale attacker, 150 acc / 250 dmg (inferred)
    NPC_KIND_SIEGE_B = 0x1a, // siege-scale attacker via 0x08010380 (inferred)
};

/**
 * Per-NPC-kind voice/sound quad; NpcSfxSets @0x0806cddc (27 entries, indexed by
 * enum NpcKind = Actor.variant of an ACTOR_TYPE_NPC actor). Only hitSfx has a known
 * consumer.
 */
typedef struct NpcSfx
{
    u16 field_0x0;
    /** played when an enemy attack connects */
    u16 hitSfx;
    u16 field_0x4;
    u16 field_0x6;
} NpcSfx;

// ACTOR_TYPE_NPC: Setup resolves the companion-slot kinds to a concrete hero per the
// active heroId (Frodo -> Sam + Gollum, Aragorn -> Legolas + Gimli, ...), then picks
// the animation. Hero kinds (< 7) render from the shared hero animation banks and get the
// active tick; the rest (Gollum, civilians, siege objects) get the passive tick.
// Companion actors are despawned in co-op and tracked in g_CompanionActors.
Actor *actor_npc_createFromMission(u8 groupIndex, u8 actorIndex);
void actor_npc_setup(Actor *actor);
void actor_npc_tick(Actor *actor);
void actor_npc_tickPassive(Actor *actor);
