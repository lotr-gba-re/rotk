#include "gfx.h"
#include "gfx/cutscenes.inc"

// clang-format off

/**
 * Cutscene pictures: 30x20 tiles over 8 palette banks, named by the first
 * CutscenePages index showing them, in ROM order.
 *
 * @romaddress 0x08e27234
 */

#define CUTSCENE_PICTURE(name)                                                                     \
    BG_ASSET_MAPPED(name, BG_ASSET_PALETTE_256 | BG_ASSET_MAP | BG_ASSET_TILES |                   \
                    BG_ASSET_CODEC_PUCRUNCH, 256, 30, 20)

// [0] "Orthanc. It was obvious the Ents had preceded us."
CUTSCENE_PICTURE(CutscenePage00BgAsset);
// [1] "Show yourself, Saruman! Receive the judgment of those you..."
CUTSCENE_PICTURE(CutscenePage01BgAsset);
// [2] "Can we not have peace, you and I?"
CUTSCENE_PICTURE(CutscenePage02BgAsset);
// [3] "We will have peace when you hang from your window for the..."
CUTSCENE_PICTURE(CutscenePage03BgAsset);
// [4] "We must find a way into Saruman's tower."
CUTSCENE_PICTURE(CutscenePage04BgAsset);
// [5] "Saruman's power is no more. His staff is broken."
CUTSCENE_PICTURE(CutscenePage05BgAsset);
// [6] "Grima! You need not follow him! You were once a man of Ro..."
CUTSCENE_PICTURE(CutscenePage06BgAsset);
// [7] "You have paid dearly for his devotion, Theoden. Did you n..."
CUTSCENE_PICTURE(CutscenePage07BgAsset);
// [8] "You made me do it!"
CUTSCENE_PICTURE(CutscenePage08BgAsset);
// [9] "And you do what I say - always - don't you worm?"
CUTSCENE_PICTURE(CutscenePage09BgAsset);
// [10] "Nooooo!"
CUTSCENE_PICTURE(CutscenePage10BgAsset);
// [11] "Now that Isengard has fallen, Edoras is safe once more."
CUTSCENE_PICTURE(CutscenePage11BgAsset);
// [12] "The same cannot be said of Gondor."
CUTSCENE_PICTURE(CutscenePage12BgAsset);
// [13] "We must ride for the stronghold of Minas Tirith. No army ..."
CUTSCENE_PICTURE(CutscenePage13BgAsset);
// [14] "Aragorn... why do you not come with us to Minas Tirith?"
CUTSCENE_PICTURE(CutscenePage14BgAsset);
// [15] "My path does not lie that way."
CUTSCENE_PICTURE(CutscenePage15BgAsset);
// [16] "And where do you think you're going?"
CUTSCENE_PICTURE(CutscenePage16BgAsset);
// [17] "Have you learned nothing of the stubbornness of Dwarves?"
CUTSCENE_PICTURE(CutscenePage18BgAsset);
// [18] "The three companions set out together down the dark road ..."
CUTSCENE_PICTURE(CutscenePage19BgAsset);
// [19] "Sauron's Corsairs were defeated, and now our ghostly army..."
CUTSCENE_PICTURE(CutscenePage20BgAsset);
// [20] "The battle is won, yet the darkness is deepening."
CUTSCENE_PICTURE(CutscenePage21BgAsset);
// [21] "If Sauron had the Ring, we would know it."
CUTSCENE_PICTURE(CutscenePage22BgAsset);
// [22] "Would we? Even if Frodo survived Cirith Ungol, thousands ..."
CUTSCENE_PICTURE(CutscenePage23BgAsset);
// [23] "Then we empty Sauron's lands... march on Mordor... and so..."
CUTSCENE_PICTURE(CutscenePage24BgAsset);
// [24] "We raced from the black tunnels... with a new ally that t..."
CUTSCENE_PICTURE(CutscenePage25BgAsset);
// [25] "The battle was won in a glorious charge, my uncle shoutin..."
CUTSCENE_PICTURE(CutscenePage26BgAsset);
// [26] "Eomer and Gandalf may have helped save Rohan, but there i..."
CUTSCENE_PICTURE(CutscenePage27BgAsset);
// [27] "We must ride to Gondor."
CUTSCENE_PICTURE(CutscenePage28BgAsset);
// [28] "Saruman will pay dearly for this night."
CUTSCENE_PICTURE(CutscenePage30BgAsset);
// [29] "Saruman is dead."
CUTSCENE_PICTURE(CutscenePage34BgAsset);
// [30] "Aragorn... you startled me. What time is it?"
CUTSCENE_PICTURE(CutscenePage35BgAsset);
// [31] "Eowyn... why did you come?"
CUTSCENE_PICTURE(CutscenePage37BgAsset);
// [32] "Do you not know?"
CUTSCENE_PICTURE(CutscenePage38BgAsset);
// [33] "It is only a shadow and a thought that you love, Eowyn. I..."
CUTSCENE_PICTURE(CutscenePage39BgAsset);
// [34] "I'm glad you've come, Gandalf! Hoom! Wood and water, stoc..."
CUTSCENE_PICTURE(CutscenePage40BgAsset);
// [35] "Too long have you escaped the vengeance of Sauron! What d..."
CUTSCENE_PICTURE(CutscenePage41BgAsset);
// [36] "I have never coveted power, Saruman."
CUTSCENE_PICTURE(CutscenePage42BgAsset);
// [37] "Shall I kill him?"
CUTSCENE_PICTURE(CutscenePage43BgAsset);
// [38] "No... I will deal with him."
CUTSCENE_PICTURE(CutscenePage44BgAsset);
// [39] "I see Him! I can see him in my head!"
CUTSCENE_PICTURE(CutscenePage45BgAsset);
// [40] "Pippin! Fool of a Took! Understand, things are now in mot..."
CUTSCENE_PICTURE(CutscenePage46BgAsset);
// [41] "There is no time to linger. There is a secret route throu..."
CUTSCENE_PICTURE(CutscenePage47BgAsset);
// [42] "We shall both go up in a great fire. Ash, ash, and smoke...."
CUTSCENE_PICTURE(CutscenePage48BgAsset);
// [43] "Stay this madness, Denethor! I shall not let you slay you..."
CUTSCENE_PICTURE(CutscenePage49BgAsset);
// [44] "Wake up! Wake up! Wake up, sleepies! We must go, yes, we ..."
CUTSCENE_PICTURE(CutscenePage50BgAsset);
// [45] "Haven't you had any sleep, Mr. Frodo? It must be getting ..."
CUTSCENE_PICTURE(CutscenePage51BgAsset);
// [46] "We should save what food we have, Sam. There's little hop..."
CUTSCENE_PICTURE(CutscenePage52BgAsset);
// [47] "I'm gonna bleed you like a stuck pig!"
CUTSCENE_PICTURE(CutscenePage54BgAsset);
// [48] "Not if I stick you first!"
CUTSCENE_PICTURE(CutscenePage55BgAsset);
// [49] "Frodo! Destroy it, now, do it! Throw it in the fire!"
CUTSCENE_PICTURE(CutscenePage56BgAsset);
// [50] "I have made my choice. I will not destroy it... the Ring ..."
CUTSCENE_PICTURE(CutscenePage57BgAsset);
// [51] "The power of the Ring is destroyed, and all that was done..."
CUTSCENE_PICTURE(CutscenePage58BgAsset);
// [52] "...and Aragorn was received with great cheers in the wall..."
CUTSCENE_PICTURE(CutscenePage59BgAsset);
// [53] "...and Eowyn was honored as a Shieldmaiden of Rohan. She ..."
CUTSCENE_PICTURE(CutscenePage61BgAsset);
// [54] "...and Frodo was reunited with his friends thirteen month..."
CUTSCENE_PICTURE(CutscenePage63BgAsset);
// [55] "...and Gandalf met the Hobbits with great hugs and smiles..."
CUTSCENE_PICTURE(CutscenePage65BgAsset);
// [56] "...and Gimli was received as a great Dwarf Lord, but more..."
CUTSCENE_PICTURE(CutscenePage67BgAsset);
// [57] "...and Legolas was celebrated as a great hero, and though..."
CUTSCENE_PICTURE(CutscenePage69BgAsset);
// clang-format on
