#include <memory>
#include <string>
#include <vector>

#include "avatar.h"
#include "catch/catch.hpp"
#include "game.h"
#include "item.h"
#include "iteminfo_query.h"

static void iteminfo_test( const item &i, const iteminfo_query &q, const std::string &reference )
{
    g->u.empty_traits();
    std::vector<iteminfo> info_v;
    std::string info = i.info( info_v, &q, 1 );
    CHECK( info == reference );
}

TEST_CASE( "armor_info", "[item][iteminfo]" )
{
    // Just a generic typical piece of clothing
    iteminfo_query q( { iteminfo_parts::ARMOR_BODYPARTS, iteminfo_parts::ARMOR_LAYER,
                        iteminfo_parts::ARMOR_COVERAGE, iteminfo_parts::ARMOR_WARMTH,
                        iteminfo_parts::ARMOR_ENCUMBRANCE, iteminfo_parts::ARMOR_PROTECTION
                      } );
    iteminfo_test(
        item( "longshirt" ), q,
        "--\n"
        // NOLINTNEXTLINE(cata-text-style)
        "Covers: The <color_c_cyan>torso</color>. The <color_c_cyan>arms</color>. \n"
        // NOLINTNEXTLINE(cata-text-style)
        "Layer: <color_c_light_blue>Normal</color>. \n"
        "Coverage: <color_c_yellow>90</color>%  Warmth: <color_c_yellow>5</color>\n"
        "--\n"
        "<color_c_white>Encumbrance</color>: <color_c_yellow>3</color> <color_c_red>(poor fit)</color>\n"
        "<color_c_white>Protection</color>: Bash: <color_c_yellow>1</color>  Cut: <color_c_yellow>1</color>\n"
        "  Acid: <color_c_yellow>0</color>  Fire: <color_c_yellow>0</color>  Environmental: <color_c_yellow>0</color>\n" );
}

TEST_CASE( "if_covers_nothing_omit_irreelevant_info", "[item][iteminfo]" )
{
    iteminfo_query q( { iteminfo_parts::ARMOR_BODYPARTS, iteminfo_parts::ARMOR_LAYER,
                        iteminfo_parts::ARMOR_COVERAGE, iteminfo_parts::ARMOR_WARMTH,
                        iteminfo_parts::ARMOR_ENCUMBRANCE, iteminfo_parts::ARMOR_PROTECTION
                      } );
    iteminfo_test(
        item( "ear_plugs" ), q,
        "--\n"
        "Covers: <color_c_cyan>Nothing</color>.\n" );
}

TEST_CASE( "gun_lists_default_ammo", "[item][iteminfo]" )
{
    iteminfo_query q( { iteminfo_parts::GUN_DEFAULT_AMMO } );
    iteminfo_test(
        item( "compbow" ), q,
        "--\n"
        "Gun is not loaded, so stats below assume the default ammo: <color_c_light_blue>wooden broadhead arrow</color>\n" );
}
TEST_CASE( "gun_damage_multiplier_not_integer", "[item][iteminfo]" )
{
    iteminfo_query q( { iteminfo_parts::GUN_DAMAGE, iteminfo_parts::GUN_DAMAGE_AMMOPROP,
                        iteminfo_parts::GUN_DAMAGE_TOTAL
                      } );
    iteminfo_test(
        item( "compbow" ), q,
        "--\n"
        "Damage: <color_c_yellow>18</color>*<color_c_yellow>1.25</color> = <color_c_yellow>22</color>\n" );
}
