#pragma once

#include "actor.h"
#include "spriteAnimation.h"
#include "types.h"

/**
 * Point an actor's sprite at an animation descriptor and restart it on frame 0. No VRAM work
 * if the actor already shows this descriptor.
 */
void sprite_setAnimation(Actor *actor, const SpriteAnimation *animation);

/**
 * Select an animation frame directly: resets the frame countdown, copies the descriptor
 * header bytes, and refreshes the trigger boxes from the frame data.
 */
void sprite_setAnimationFrame(Actor *actor, u8 frameIndex);

/** Read one animation frame's cell pixel size: sizeOut[0] = width, sizeOut[1] = height. */
void sprite_getAnimationFrameSize(const SpriteAnimation *animation, u32 *sizeOut, u32 frameIndex);

/**
 * Allocate an actor from the main pool (g_ActorPoolMain, 0x100 blocks) and link it into
 * g_ActorListMain with listId ACTOR_LIST_MAIN. The 0x148-byte block is the Actor storage
 * (see actor.h), zeroed by pool_alloc. Returns NULL on pool exhaustion.
 */
Actor *actor_allocMain(void);
/**
 * Allocate an actor from the overlay pool (g_ActorPoolOverlay, 0x28 blocks) into
 * g_ActorListOverlay with listId ACTOR_LIST_OVERLAY; used by the overlay menus (backpack,
 * skill tree, character stats, shop) so the main list stays intact underneath while
 * GAME_FLAG_OVERLAY routes rendering to the overlay list. Returns NULL on pool
 * exhaustion.
 */
Actor *actor_allocOverlay(void);
/** Set an actor sprite's position; x/y are 16.16 fixed-point screen coords. */
void sprite_setPosition(Actor *actor, u32 x, u32 y);

/**
 * Walk an actor list oldest-first (from list->tail), actor_tick'ing each object.
 *
 * @param commit true also commits the frame (gfx upload + OAM compose)
 */
u8 actor_tickAllInList(ActorList *list, bool commit);
/** Flush an actor list: process each object, hide the unused OAM tail, wait for vblank. */
void sprite_flushList(ActorList *list);
/**
 * Per-frame sprite commit dispatcher: processes + commits the main list, or the overlay
 * list when GAME_FLAG_OVERLAY is set (overlay menu open; in co-op the main list is still
 * ticked without committing).
 */
u8 actor_tickAll(void);
/**
 * Per-frame tail: hides unused OAM slots (alloc count..0x7f) and sets
 * g_EngineState->frameReady, so a scene that never calls this renders nothing new.
 */
void sprite_endFrame(void);
/** Hide every OAM shadow entry, DMA the shadow to OAM, and reset the alloc count. */
void sprite_resetOam(void);

/*
 * OBJ VRAM tile allocator: g_ObjTileBitmap has one bit per 32-byte tile (1024 tiles,
 * 32 KiB). g_ObjTileWorldHighWater counts the tiles handed to actors while the world is not
 * parked and is never lowered by a free; while GAME_FLAG_WORLD_PARKED is set, allocations
 * search above it so overlay sprites leave the parked world's tiles alone.
 */

/** Reset the allocator: all tiles free, no deferred frees, high-water mark 0. */
void sprite_resetTileAllocator(void);
/** Same as sprite_resetTileAllocator. */
void sprite_resetTileAllocator2(void);
/**
 * Reset the allocator for a display mode: bitmap modes 3-5 keep tiles 0-511 reserved for
 * the frame buffer.
 */
void sprite_resetTileAllocatorForMode(s32 displayMode);

/**
 * Allocate OBJ VRAM tiles for gfxSize bytes of character data and, with an actor, upload
 * its frame there. The first-fit bit search runs from IWRAM (0x03007690).
 *
 * @return the first tile number, 0xffff when no run is free
 */
u32 sprite_allocObjTiles(Actor *actor, u32 gfxSize);

/** Free the tiles at tileNumber covering gfxSize bytes at once. */
void sprite_freeObjTiles(u16 tileNumber, u32 gfxSize);

/**
 * Mark the tiles at tileNumber covering gfxSize bytes in g_ObjTileDeferredBitmap; they stay
 * allocated until sprite_applyDeferredObjTileFrees, so the frame still showing them is safe.
 */
void sprite_freeObjTilesDeferred(u16 tileNumber, u32 gfxSize);

/** Release the deferred frees into g_ObjTileBitmap; system_endFrame runs it every frame. */
void sprite_applyDeferredObjTileFrees(void);

/**
 * Select an actor's animation from a bank (see SpriteRender.animationBank): pose picks the
 * column, facingVariant the SpriteAnimation within it; applies it and uploads its palette.
 */
void sprite_selectAnimation(Actor *actor, void *animationBank, u8 pose, u8 facingVariant);

/**
 * Enable multi-sprite mode and init the actor's SpriteRender slots: animationBank cleared,
 * all slots freed, slot palette numbers seeded from the actor's own palette number.
 */
void sprite_initSlots(Actor *actor);
/** Attach an animation-bank part to a sub-sprite slot with its own OAM palette number. */
void sprite_attachSlot(Actor *actor, u8 slot, u8 partId, u8 paletteNumber);
/** Free a sub-sprite slot (slotPartIds[slot] = 0xff). */
void sprite_detachSlot(Actor *actor, u8 slot);
/** Set a slot's draw order (0..7); see SpriteRender.slotDrawOrders. */
void sprite_setSlotDrawOrder(Actor *actor, u8 slot, u8 drawOrder);
// Set/clear per-slot flag bits (see SpriteRender.slotFlags); only partly decoded (bits
// 2/3 gate the slot's frame advance).
void sprite_setSlotFlag0(Actor *actor, u8 slot);
void sprite_setSlotFlag1(Actor *actor, u8 slot);
void sprite_clearSlotFlag1(Actor *actor, u8 slot);
void sprite_setSlotFlag2(Actor *actor, u8 slot);

/**
 * Slot equivalent of actor_getAnimationFrame: the slot's current-frame packed gfx id;
 * two frame bytes go to out.
 */
u16 sprite_getSlotFrameInfo(Actor *actor, u32 *out, u8 slot);
/** The slot's current-frame tile data pointer. */
void *sprite_getSlotFrameGfx(Actor *actor, u8 slot);

/**
 * Per-frame update of one sprite: advance the animation, integrate movement, tick the
 * scale run, submit to the render lists.
 */
void actor_tick(Actor *actor, bool commit);
/**
 * Release a sprite's OBJ VRAM tiles (main + sub-sprite slots) and unlink it from its
 * actor list. Returns 0, or -1 when the actor's recordType is 0xffff.
 */
s32 sprite_free(Actor *actor);
/**
 * Compose the sprite's OAM entries: screen position, the multi-sprite slots in draw
 * order (7..4 before the main sprite, 3..0 after), then the main sprite. Returns 0 if not
 * submitted; 2 also queues the sprite for post-commit processing.
 */
u8 sprite_composeOam(Actor *actor);
/**
 * Commit a submitted sprite's graphics: allocate OAM slots and upload the current
 * frame's tiles for the main sprite and each attached slot, releasing stale slots.
 */
void sprite_commitGfx(Actor *actor, bool commit);

/**
 * Set a sprite's OAM scale immediately: signed 8.8 fixed-point texture-step divisors
 * (the ObjAffineSet source format: 0x100 = 1:1, larger = smaller on screen, so 0x200 =
 * half size and 0x80 = double size; valid 1..0x7fff, 0 divides by zero in the BIOS);
 * angle 0..0xffff = 0..360 deg. Enables the sprite's OAM rotation/scaling +
 * double-size bits.
 */
void sprite_setScale(Actor *actor, s16 scaleX, s16 scaleY, u16 angle);
/**
 * Start a linear x/y scale-over-time run: from (startX, startY) to (targetX, targetY)
 * over the given frame count (16.16 fixed-point texture-step divisors, 0x10000 = 1:1 -
 * the visual magnification is the reciprocal; valid 0x100..0x7fff00 - the tick stores
 * them as 8.8 OAM scale params, and only scaleX is guarded against a div-by-zero at 0).
 * Enables the sprite's OAM rotation/scaling + double-size bits. Unused by the shipping
 * ROM.
 */
void sprite_startScale(Actor *actor, s32 startX, s32 startY, s32 targetX, s32 targetY, s32 frames);
/** Advance the scale run one frame; actor_tick calls it while scaleFramesLeft != 0. */
void sprite_tickScale(Actor *actor);
/** Clear the sprite's OAM rotation/scaling + double-size bits (oam.affineMode).
 */
void sprite_disableScale(Actor *actor);
