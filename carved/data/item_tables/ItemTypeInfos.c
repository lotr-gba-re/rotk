#include "item.h"
#include "variables.h"

// clang-format off

/** @romaddress 0x08068f50 */
const ItemTypeInfo ItemTypeInfos[15] = {
    // ITEM_TYPE_KNIFE
    {
      .baseItems          = KnifeItems,
      .prefixRegionRanges = WeaponPrefixRegionRanges,
      .prefixes           = WeaponPrefixes,
      .prefixFlags        = WeaponPrefixFlags,
      .suffixTierRanges   = SuffixTierRanges,
      .suffixes           = ItemSuffixes,
      .suffixFlags        = ItemSuffixFlags,
      .baseGfx            = KnifeGfx,
      .baseMetadata       = KnifeItemFlags,
      .baseItemCount      = 18, },

    // ITEM_TYPE_SWORD_1H
    {
      .baseItems          = Sword1HItems,
      .prefixRegionRanges = WeaponPrefixRegionRanges,
      .prefixes           = WeaponPrefixes,
      .prefixFlags        = WeaponPrefixFlags,
      .suffixTierRanges   = SuffixTierRanges,
      .suffixes           = ItemSuffixes,
      .suffixFlags        = ItemSuffixFlags,
      .baseGfx            = Sword1HGfx,
      .baseMetadata       = Sword1HItemFlags,
      .baseItemCount      = 11, },

    // ITEM_TYPE_SWORD_2H
    {
      .baseItems          = Sword2HItems,
      .prefixRegionRanges = WeaponPrefixRegionRanges,
      .prefixes           = WeaponPrefixes,
      .prefixFlags        = WeaponPrefixFlags,
      .suffixTierRanges   = SuffixTierRanges,
      .suffixes           = ItemSuffixes,
      .suffixFlags        = ItemSuffixFlags,
      .baseGfx            = Sword2HGfx,
      .baseMetadata       = Sword2HItemFlags,
      .baseItemCount      = 9, },

    // ITEM_TYPE_AXE
    {
      .baseItems          = AxeItems,
      .prefixRegionRanges = WeaponPrefixRegionRanges,
      .prefixes           = WeaponPrefixes,
      .prefixFlags        = WeaponPrefixFlags,
      .suffixTierRanges   = SuffixTierRanges,
      .suffixes           = ItemSuffixes,
      .suffixFlags        = ItemSuffixFlags,
      .baseGfx            = AxeGfx,
      .baseMetadata       = AxeItemFlags,
      .baseItemCount      = 11, },

    // ITEM_TYPE_MACE
    {
      .baseItems          = MaceItems,
      .prefixRegionRanges = WeaponPrefixRegionRanges,
      .prefixes           = WeaponPrefixes,
      .prefixFlags        = WeaponPrefixFlags,
      .suffixTierRanges   = SuffixTierRanges,
      .suffixes           = ItemSuffixes,
      .suffixFlags        = ItemSuffixFlags,
      .baseGfx            = MaceGfx,
      .baseMetadata       = MaceItemFlags,
      .baseItemCount      = 10, },

    // ITEM_TYPE_STAFF
    {
      .baseItems          = StaffItems,
      .prefixRegionRanges = StaffBowPrefixRegionRanges,
      .prefixes           = StaffBowPrefixes,
      .prefixFlags        = StaffBowPrefixFlags,
      .suffixTierRanges   = SuffixTierRanges,
      .suffixes           = ItemSuffixes,
      .suffixFlags        = ItemSuffixFlags,
      .baseGfx            = StaffGfx,
      .baseMetadata       = StaffItemFlags,
      .baseItemCount      = 4, },

    // ITEM_TYPE_BOW
    {
      .baseItems          = BowItems,
      .prefixRegionRanges = StaffBowPrefixRegionRanges,
      .prefixes           = StaffBowPrefixes,
      .prefixFlags        = StaffBowPrefixFlags,
      .suffixTierRanges   = SuffixTierRanges,
      .suffixes           = ItemSuffixes,
      .suffixFlags        = ItemSuffixFlags,
      .baseGfx            = BowGfx,
      .baseMetadata       = BowItemFlags,
      .baseItemCount      = 13, },

    // ITEM_TYPE_ARROW
    {
      .baseItems          = ArrowItems,
      .prefixRegionRanges = NULL,
      .prefixes           = NULL,
      .prefixFlags        = NULL,
      .suffixTierRanges   = NULL,
      .suffixes           = NULL,
      .suffixFlags        = NULL,
      .baseGfx            = ArrowGfx,
      .baseMetadata       = ArrowItemFlags,
      .baseItemCount      = 10, },

    // ITEM_TYPE_ARMOR
    {
      .baseItems          = ArmorItems,
      .prefixRegionRanges = ArmorPrefixRegionRanges,
      .prefixes           = ArmorPrefixes,
      .prefixFlags        = ArmorPrefixFlags,
      .suffixTierRanges   = SuffixTierRanges,
      .suffixes           = ItemSuffixes,
      .suffixFlags        = ItemSuffixFlags,
      .baseGfx            = ArmorGfx,
      .baseMetadata       = ArmorItemFlags,
      .baseItemCount      = 24, },

    // ITEM_TYPE_SHIELD
    {
      .baseItems          = ShieldItems,
      .prefixRegionRanges = ArmorPrefixRegionRanges,
      .prefixes           = ArmorPrefixes,
      .prefixFlags        = ArmorPrefixFlags,
      .suffixTierRanges   = SuffixTierRanges,
      .suffixes           = ItemSuffixes,
      .suffixFlags        = ItemSuffixFlags,
      .baseGfx            = ShieldGfx,
      .baseMetadata       = ShieldItemFlags,
      .baseItemCount      = 8, },

    // ITEM_TYPE_ACCESSORY
    {
      .baseItems          = AccessoryItems,
      .prefixRegionRanges = AccessoryPrefixRegionRanges,
      .prefixes           = AccessoryPrefixes,
      .prefixFlags        = AccessoryPrefixFlags,
      .suffixTierRanges   = SuffixTierRanges,
      .suffixes           = ItemSuffixes,
      .suffixFlags        = ItemSuffixFlags,
      .baseGfx            = AccessoryGfx,
      .baseMetadata       = AccessoryItemFlags,
      .baseItemCount      = 12, },

    // ITEM_TYPE_CLOAK
    {
      .baseItems          = CloakItems,
      .prefixRegionRanges = CloakPrefixRegionRanges,
      .prefixes           = CloakPrefixes,
      .prefixFlags        = CloakPrefixFlags,
      .suffixTierRanges   = SuffixTierRanges,
      .suffixes           = ItemSuffixes,
      .suffixFlags        = ItemSuffixFlags,
      .baseGfx            = CloakGfx,
      .baseMetadata       = CloakItemFlags,
      .baseItemCount      = 9, },

    // ITEM_TYPE_GLOVES
    {
      .baseItems          = GloveItems,
      .prefixRegionRanges = GloveShoePrefixRegionRanges,
      .prefixes           = GloveShoePrefixes,
      .prefixFlags        = GloveShoePrefixFlags,
      .suffixTierRanges   = SuffixTierRanges,
      .suffixes           = ItemSuffixes,
      .suffixFlags        = ItemSuffixFlags,
      .baseGfx            = GloveGfx,
      .baseMetadata       = GloveItemFlags,
      .baseItemCount      = 8, },

    // ITEM_TYPE_SHOES
    {
      .baseItems          = ShoeItems,
      .prefixRegionRanges = GloveShoePrefixRegionRanges,
      .prefixes           = GloveShoePrefixes,
      .prefixFlags        = GloveShoePrefixFlags,
      .suffixTierRanges   = SuffixTierRanges,
      .suffixes           = ItemSuffixes,
      .suffixFlags        = ItemSuffixFlags,
      .baseGfx            = ShoeGfx,
      .baseMetadata       = ShoeItemFlags,
      .baseItemCount      = 7, },

    // ITEM_TYPE_UNIQUE
    {
      .baseItems          = UniqueItems,
      .prefixRegionRanges = NULL,
      .prefixes           = NULL,
      .prefixFlags        = (const AffixFlags *)0x08065574,  // uncarved
      .suffixTierRanges   = NULL,
      .suffixes           = NULL,
      .suffixFlags        = NULL,
      .baseGfx            = UniqueGfx,
      .baseMetadata       = UniqueItemFlags,
      .baseItemCount      = 53, }
};
// clang-format on
