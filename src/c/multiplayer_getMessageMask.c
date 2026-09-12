#include "multiplayer.h"
#include "variables.h"

/**
 * Return the bitmask of received link messages.
 *
 * @romaddress 0x08046dc4
 */
u32 multiplayer_getMessageMask(void)
{
    return g_MpMessageMask;
}