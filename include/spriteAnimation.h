#pragma once

#include "types.h"

// Sprite animation assets: the descriptor sprite_setAnimation takes and the frame set,
// OAM templates and trigger boxes it points at.

/**
 * One sprite-relative trigger box (Actor.triggerBoxes slot): the collision
 * pass pairwise-tests enabled boxes of colliding actors and fires
 * their triggerCallbacks. xMin/xMax are negated when the sprite is h-flipped.
 */
struct ActorTriggerBox
{
    s8 xMin;
    s8 xMax;
    s8 yMin;
    s8 yMax;
    bool enable;
} __attribute__((packed, aligned(2)));

/** Conventional trigger-box slots for combat actors. */
typedef enum CombatTriggerBoxSlot
{
    COMBAT_TRIGGER_HURTBOX = 0, // vulnerable body area and touch callback
    COMBAT_TRIGGER_HITBOX = 1,  // active attack area and attack callback
} CombatTriggerBoxSlot;

/** SpriteFrame.flags bits above the OAM template count. */
enum SpriteFrameFlag
{
    SPRITE_FRAME_RL = 1 << 5,   // tile data is BIOS RL compressed (RLUnCompVram)
    SPRITE_FRAME_LZ77 = 1 << 6, // tile data is lz77 compressed (decompress_lz77)
};

union SpriteFrameFlags {
    u8 p;

    struct
    {
        u8 oamCount : 5; // SpriteOamTemplate count following the frame
        u8 rl : 1;       // SPRITE_FRAME_RL
        u8 lz77 : 1;     // SPRITE_FRAME_LZ77
    } __attribute__((packed)) d;
} __attribute__((packed, aligned(2)));

/**
 * One OBJ of a frame, relative to the sprite position. sprite_composeOam turns each into
 * an OAM entry at the actor's tile number plus tileOffset.
 */
union SpriteOamTemplate {
    u32 p;

    struct
    {
        s32 x : 9; // pixel offset from the sprite position
        s32 y : 9;
        u32 objSize : 2;     // OAM attr1 bits 14-15
        u32 objShape : 2;    // OAM attr0 bits 14-15 (0 square, 1 horizontal, 2 vertical)
        u32 tileOffset : 10; // OBJ tile number offset within the frame's character data
    } __attribute__((packed)) d;
} __attribute__((packed));

/** Physical sprite bounds relative to its position, used for movement and actor overlap. */
typedef struct SpriteBounds
{
    s8 xMin;
    s8 xMax;
    s8 yMin;
    s8 yMax;
} __attribute__((packed)) SpriteBounds;

/** A signed sprite-relative point used to place projectiles and effects. */
typedef struct SpriteHotspot
{
    s8 x;
    s8 y;
} __attribute__((packed)) SpriteHotspot;

/**
 * One frame of a frame set, located by its frame offset. Followed in
 * memory by SpriteFrameSetHeader.hotspotCount SpriteHotspot records, triggerBoxCount
 * ActorTriggerBox records and flags.d.oamCount SpriteOamTemplate records.
 */
typedef struct SpriteFrame
{
    union SpriteFrameFlags flags;
    u8 width; // cell size in pixels
    u8 height;
    u16 tileOffset; // byte offset of the frame's tile data in SpriteAnimation.tiles
    u16 tileBytes;  // decompressed 4bpp tile data size; the sprite's gfxSize
} __attribute__((packed, aligned(2))) SpriteFrame;

/**
 * Frame set header; u16 frameOffsets[frameCount] follow, each a SpriteFrame's byte offset
 * from the end of this header.
 */
typedef struct SpriteFrameSetHeader
{
    u8 width; // cell size in pixels
    u8 height;

    /** Physical footprint used for terrain movement collision and actor overlap. */
    SpriteBounds movementCollisionBox;

    u16 frameCount;

    /** Largest decompressed SpriteFrame.tileBytes value among the frames. */
    u16 maxFrameTileBytes;

    /** Number of SpriteHotspot records following each SpriteFrame. */
    u8 hotspotCount;

    /** Number of ActorTriggerBox records per frame. */
    u8 triggerBoxCount;
} __attribute__((packed, aligned(2))) SpriteFrameSetHeader;

/** A variable-length frame set, with frame offsets relative to the start of data. */
typedef struct SpriteFrameSet
{
    SpriteFrameSetHeader header;
#ifdef AGBCC
    u8 data[0]; // u16 frame offsets followed by variable-length SpriteFrame records
#else
    u8 data[]; // u16 frame offsets followed by variable-length SpriteFrame records
#endif
} __attribute__((aligned(4))) SpriteFrameSet;

/**
 * Sprite-engine animation descriptor; one per variant in the animation tables
 * sprite_setAnimation takes (e.g. ShopCursorAnimations).
 */
typedef struct SpriteAnimation
{
    const void *tiles; // 4bpp OBJ character data, per frame at SpriteFrame.tileOffset
    const SpriteFrameSetHeader *frames;
    const u16 *palette; // optional 16-color OBJ palette, uploaded on set
    u8 frameDuration;   // frame countdown reset value
} SpriteAnimation;

/** A still SpriteAnimation: a single frame, and frameDuration 0 so it never steps. */
#define SPRITE_STILL_ANIMATION(tiles_, frames_, palette_)                                          \
    {.tiles = tiles_, .frames = frames_, .palette = palette_, .frameDuration = 0}
/** SPRITE_STILL_ANIMATION from a symbol prefix: name##Tiles / name##Frames / name##Palette. */
#define SPRITE_STILL_ANIMATION_NAMED(name)                                                         \
    SPRITE_STILL_ANIMATION(name##Tiles, &name##Frames, name##Palette)

/**
 * Frame set of an item icon: one 32x24 frame drawn as a 32x8 over a 32x16 OBJ. Frame sets
 * are padded to 4 bytes in ROM.
 */
typedef struct SpriteIconFrameSet
{
    SpriteFrameSetHeader header;
    u16 frameOffsets[1];
    SpriteFrame frame;
    union SpriteOamTemplate oam[2];
} __attribute__((aligned(4))) SpriteIconFrameSet;

/** SpriteIconFrameSet with one trigger box slot (no padding needed). */
typedef struct SpriteIconBoxedFrameSet
{
    SpriteFrameSetHeader header;
    u16 frameOffsets[1];
    SpriteFrame frame;
    struct ActorTriggerBox triggerBoxes[1];
    union SpriteOamTemplate oam[2];
} __attribute__((aligned(4))) SpriteIconBoxedFrameSet;

// OBJ shape/size attribute pairs by OBJ pixel size (GBATEK OBJ attributes 0 and 1).
#define OBJ_SHAPE_8x8 0
#define OBJ_SIZE_8x8 0
#define OBJ_SHAPE_16x16 0
#define OBJ_SIZE_16x16 1
#define OBJ_SHAPE_32x32 0
#define OBJ_SIZE_32x32 2
#define OBJ_SHAPE_64x64 0
#define OBJ_SIZE_64x64 3
#define OBJ_SHAPE_16x8 1
#define OBJ_SIZE_16x8 0
#define OBJ_SHAPE_32x8 1
#define OBJ_SIZE_32x8 1
#define OBJ_SHAPE_32x16 1
#define OBJ_SIZE_32x16 2
#define OBJ_SHAPE_64x32 1
#define OBJ_SIZE_64x32 3
#define OBJ_SHAPE_8x16 2
#define OBJ_SIZE_8x16 0
#define OBJ_SHAPE_8x32 2
#define OBJ_SIZE_8x32 1
#define OBJ_SHAPE_16x32 2
#define OBJ_SIZE_16x32 2
#define OBJ_SHAPE_32x64 2
#define OBJ_SIZE_32x64 3

/** 4bpp character data bytes of a w x h cell. */
#define SPRITE_TILE_BYTES(w, h) ((w) * (h) / 2)

// Typed arrays following a frame header. Counts may be zero.
#define SPRITE_FRAME_RECORD(hotspotCount, boxCount, oamCount)                                      \
    struct __attribute__((packed, aligned(2)))                                                     \
    {                                                                                              \
        SpriteFrame frame;                                                                         \
        SpriteHotspot hotspots[hotspotCount];                                                      \
        struct ActorTriggerBox triggerBoxes[boxCount];                                             \
        union SpriteOamTemplate oam[oamCount];                                                     \
    }

// The trailing declarations describe the frames in ROM order, with per-frame array sizes.
#define SPRITE_FRAME_SET_LAYOUT(frameCount, ...)                                                   \
    struct __attribute__((aligned(4)))                                                             \
    {                                                                                              \
        SpriteFrameSetHeader header;                                                               \
        u16 frameOffsets[frameCount];                                                              \
        __VA_ARGS__                                                                                \
    }

// A uniform frame set, with the same record shape for every frame.
#define SPRITE_FRAME_SET_STRUCT(frameCount, boxCount, oamCount)                                    \
    SPRITE_FRAME_SET_LAYOUT(frameCount,                                                            \
                            SPRITE_FRAME_RECORD(0, boxCount, oamCount) frames[frameCount];)
#define SPRITE_FRAME_SET_HEADER(w, h, frameCount_, boxCount)                                       \
    {.width = w,                                                                                   \
     .height = h,                                                                                  \
     .frameCount = frameCount_,                                                                    \
     .maxFrameTileBytes = SPRITE_TILE_BYTES(w, h),                                                 \
     .triggerBoxCount = boxCount}
#define SPRITE_FRAME(oamCount_, lz77_, w, h, tileOffset_)                                          \
    {.flags.d = {.oamCount = oamCount_, .lz77 = lz77_},                                            \
     .width = w,                                                                                   \
     .height = h,                                                                                  \
     .tileOffset = tileOffset_,                                                                    \
     .tileBytes = SPRITE_TILE_BYTES(w, h)}
#define TRIGGER_BOX(xMin_, xMax_, yMin_, yMax_)                                                    \
    {.xMin = xMin_, .xMax = xMax_, .yMin = yMin_, .yMax = yMax_, .enable = TRUE}
/** One w x h OBJ at (x, y) of the frame, drawn from OBJ tile number tile. */
#define SPRITE_OAM(x_, y_, w, h, tile)                                                             \
    {                                                                                              \
        .d = {                                                                                     \
            .x = x_,                                                                               \
            .y = y_,                                                                               \
            .objSize = OBJ_SIZE_##w##x##h,                                                         \
            .objShape = OBJ_SHAPE_##w##x##h,                                                       \
            .tileOffset = tile                                                                     \
        }                                                                                          \
    }
