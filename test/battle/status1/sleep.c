#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Sleep prevents the battler from using a move")
{
    u32 turns, j;
    PARAMETRIZE { turns = 1; }
    PARAMETRIZE { turns = 2; }
    PARAMETRIZE { turns = 3; }
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Status1(STATUS1_SLEEP_TURN(turns)); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        for (j = 0; j < turns; j++)
            TURN { MOVE(player, MOVE_CELEBRATE); }
    } SCENE {
        for (j = 0; j < turns - 1; j++)
            MESSAGE("Wobbuffet is fast asleep.");
        MESSAGE("Wobbuffet woke up!");
        STATUS_ICON(player, none: TRUE);
        MESSAGE("Wobbuffet used Celebrate!");
    }
}

SINGLE_BATTLE_TEST("Sleep: Spore affects grass types (Gen1-5)")
{
    GIVEN {
        WITH_CONFIG(GEN_CONFIG_POWDER_GRASS, GEN_5);
        ASSUME(IsPowderMove(MOVE_SPORE));
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_CHIKORITA);
    } WHEN {
        TURN { MOVE(player, MOVE_SPORE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SPORE, player);
    }
}

SINGLE_BATTLE_TEST("Sleep: Spore doesn't affect grass types (Gen6+)")
{
    GIVEN {
        WITH_CONFIG(GEN_CONFIG_POWDER_GRASS, GEN_6);
        ASSUME(IsPowderMove(MOVE_SPORE));
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_CHIKORITA);
    } WHEN {
        TURN { MOVE(player, MOVE_SPORE); }
    } SCENE {
        NOT ANIMATION(ANIM_TYPE_MOVE, MOVE_SPORE, player);
    }
}

AI_SINGLE_BATTLE_TEST("AI avoids hypnosis when it can not put target to sleep")
{
    u32 species;
    enum Ability ability;

    PARAMETRIZE { species = SPECIES_HOOTHOOT; ability = ABILITY_INSOMNIA; }
    PARAMETRIZE { species = SPECIES_MANKEY; ability = ABILITY_VITAL_SPIRIT; }
    PARAMETRIZE { species = SPECIES_KOMALA; ability = ABILITY_COMATOSE; }
    PARAMETRIZE { species = SPECIES_NACLI; ability = ABILITY_PURIFYING_SALT; }

    GIVEN {
        AI_FLAGS(AI_FLAG_CHECK_BAD_MOVE | AI_FLAG_CHECK_VIABILITY | AI_FLAG_TRY_TO_FAINT | AI_FLAG_OMNISCIENT);
        PLAYER(species) { Ability(ability); }
        OPPONENT(SPECIES_WOBBUFFET) { Moves(MOVE_CELEBRATE, MOVE_HYPNOSIS); }
    } WHEN {
        TURN { SCORE_EQ(opponent, MOVE_CELEBRATE, MOVE_HYPNOSIS); } // Both get -10
    }
}

SINGLE_BATTLE_TEST("newsleep", s16 damage)
{   bool32 asleep;
    PARAMETRIZE { asleep = FALSE; }
    PARAMETRIZE { asleep = TRUE; }

    GIVEN {
        ASSUME(GetMoveCategory(MOVE_SCRATCH) == DAMAGE_CATEGORY_PHYSICAL);
        PLAYER(SPECIES_WOBBUFFET) {
            Moves(MOVE_SCRATCH);
        }
        OPPONENT(SPECIES_WOBBUFFET) {
            if (asleep)
                Status1(STATUS1_SLEEP);
        }
    } WHEN {
        TURN {
            MOVE(player, MOVE_SCRATCH);
        }
    } SCENE {
        HP_BAR(opponent, captureDamage: &results[asleep].damage);
    } FINALLY {
        EXPECT_MUL_EQ(
            results[FALSE].damage,
            Q_4_12(1.2),
            results[TRUE].damage
        );
    }
}

SINGLE_BATTLE_TEST("Sleep heals 1/8 HP at end of turn")
{
    s16 hpBefore;
    s16 hpAfter;

    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) {
            HP(50);
            MaxHP(100);
        }
        OPPONENT(SPECIES_WOBBUFFET) {
            Status1(STATUS1_SLEEP);
        }
    } WHEN {
        TURN {
            MOVE(player, MOVE_SPLASH);
        }
    } SCENE {
        NONE_OF {
            ANIMATION(ANIM_TYPE_MOVE, MOVE_SPLASH, player);
        }
    } FINALLY {
        hpBefore = 50;
        hpAfter = GetBattlerHp(player);

        EXPECT_EQ(
            hpBefore + 100 / 8,
            hpAfter
        );
    }
}

SINGLE_BATTLE_TEST("Sleeping target has 20 percent less evasion")
{
    bool32 asleep;

    PARAMETRIZE { asleep = FALSE; }
    PARAMETRIZE { asleep = TRUE; }

    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) {
            Moves(MOVE_TACKLE);
        }
        OPPONENT(SPECIES_WOBBUFFET) {
            if (asleep)
                Status1(STATUS1_SLEEP);
        }
    } WHEN {
        TURN {
            MOVE(player, MOVE_TACKLE);
        }
    } SCENE {
        if (asleep)
            MESSAGE("The attack hit!");
        else
            MESSAGE("The attack hit!");
    } FINALLY {
        EXPECT_MUL_EQ(
            results[FALSE].hits,
            Q_4_12(1.25),
            results[TRUE].hits
        );
    }
}
