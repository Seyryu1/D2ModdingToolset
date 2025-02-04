/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2020 Vladimir Makeev.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef BATTLEMSGDATA_H
#define BATTLEMSGDATA_H

#include "idlist.h"
#include "targetslist.h"
#include "unitinfolist.h"
#include <cstddef>
#include <cstdint>

namespace game {

struct IMidgardObjectMap;
struct IAttack;
struct IBatAttack;
struct LAttackSource;
struct LAttackClass;
struct LAttackReach;
struct CMidUnitGroup;

/** Unit statuses in battle. */
enum class BattleStatus : int
{
    XpCounted = 0,         /**< Unit was killed and its experience points were counted. */
    Dead,                  /**< Unit dead. */
    Paralyze,              /**< Unit paralyzed. */
    Petrify,               /**< Unit petrified. */
    DisableLong,           /**< Long disable applied (paralyze, petrify or fear). */
    BoostDamageLvl1,       /**< 25% boost */
    BoostDamageLvl2,       /**< 50% boost */
    BoostDamageLvl3,       /**< 75% boost */
    BoostDamageLvl4,       /**< 100% boost */
    BoostDamageLong,       /**< Long damage boost (until battle is over or lower damage applied). */
    LowerDamageLvl1,       /**< 50% lower damage */
    LowerDamageLvl2,       /**< 33% lower damage */
    LowerDamageLong,       /**< Long lower damage (until battle is over or removed). */
    LowerInitiative,       /**< 50% lower initiative */
    LowerInitiativeLong,   /**< Long lower initiative. */
    Poison,                /**< Poison dot. */
    PoisonLong,            /**< Long poison applied. */
    Frostbite,             /**< Frostbite dot. */
    FrostbiteLong,         /**< Long frostbite applied. */
    Blister,               /**< Blister dot. */
    BlisterLong,           /**< Long blister applied. */
    Cured,                 /**< Cure applied ? */
    Transform,             /**< Unit transformed by another unit. */
    TransformLong,         /**< Long transformation applied by another unit. */
    TransformSelf,         /**< Unit transfomed himself ? */
    TransformDoppelganger, /**< Doppelganger transformation ? */
    TransformDrainLevel,   /**< Drain level applied ? */
    Summon,                /**< Unit was summoned during battle. */
    Retreated,             /**< Unit retreated from battle. */
    Retreat,               /**< Unit is retreating. */
    Hidden,                /**< Unit is hidden. For example, while leader dueling a thief. */
    Defend,                /**< Defend was used in this round. */
    Unsummoned             /**< unsummon effect applied ? */
};

enum class BattleAction : int
{
    Attack = 0,
    Skip,
    Retreat,
    Wait,
    Defend,
    Auto,
    UseItem,
    Resolve,
};

struct ModifiedUnitInfo
{
    CMidgardID unitId;
    CMidgardID modifierId;
};

static_assert(sizeof(ModifiedUnitInfo) == 8,
              "Size of ModifiedUnitInfo structure must be exactly 8 bytes");

/** Array of 48 items (8 modifiers x 6 units). */
static const size_t ModifiedUnitCountPatched = 48;
union ModifiedUnitsPatched
{
    ModifiedUnitInfo original[8];
    ModifiedUnitInfo* patched;
};

static_assert(sizeof(ModifiedUnitsPatched) == 64,
              "Size of ModifiedUnitsPatched union must be exactly 64 bytes");

union AttackSourceImmunityStatusesPatched
{
    struct
    {
        std::uint8_t original;
        char padding[3];
    };
    std::uint32_t patched;
};

static_assert(sizeof(AttackSourceImmunityStatusesPatched) == 4,
              "Size of AttackSourceImmunityStatusesPatched union must be exactly 4 bytes");

/** Battle turn info. */
struct BattleTurn
{
    /** Id of unit performing the turn. */
    CMidgardID unitId;
    /** Number of attacks unit can make in a single turn. 2 for units with double attack. */
    char attacksCount;
    char padding[3];
};

static_assert(sizeof(BattleTurn) == 8, "Size of BattleTurn structure must be exactly 8 bytes");

/** Additional unit statuses and information packed in single byte. */
union UnitFlags
{
    struct
    {
        std::uint8_t indexInGroup : 3;
        bool attacker : 1;
        std::uint8_t flag4 : 1;
        std::uint8_t unknown : 1;
        bool revived : 1;
        /**
         * Indicates that unit waited and then started retreating.
         * Units with this flag set are skipped during turns order computation
         * at the start of the round.
         */
        bool retreatingAfterWait : 1;
    } parts;

    std::uint8_t value;
};

static_assert(sizeof(UnitFlags) == 1, "Size of UnitFlags union must be exactly 1 byte");

/** Holds unit information used in battle. */
struct UnitInfo
{
    CMidgardID unitId1;
    CMidgardID unitId2;
    /** Bitmask made of BattleStatus values used as shifts. */
    std::uint64_t unitStatuses;
    /** Additional statuses, unused in RotE v3.01. */
    std::uint64_t unitStatuses2;
    /** Id of attack that applied poison effect. */
    CMidgardID poisonAttackId;
    /** Id of attack that applied frostbite effect. */
    CMidgardID frostbiteAttackId;
    /** Id of attack that applied blister effect. */
    CMidgardID blisterAttackId;
    /** Bitmask with values for each of LAttackSource. */
    AttackSourceImmunityStatusesPatched attackSourceImmunityStatuses;
    /** Bitmask with values for each of LAttackClass. */
    std::uint32_t attackClassImmunityStatuses;
    std::int16_t unitHp;
    std::int16_t unitXp;
    UnitFlags unitFlags;
    char unknown2;
    /** Round when paralyze, petrify or fear was applied. */
    std::int8_t disableAppliedRound;
    /** Round when long poison was applied. 0 means poison was not applied. */
    std::int8_t poisonAppliedRound;
    /** Round when long frostbite was applied. */
    std::int8_t frostbiteAppliedRound;
    /** Round when long blister was applied. */
    std::int8_t blisterAppliedRound;
    /** Round when long transform was applied. */
    std::int8_t transformAppliedRound;
    char unknown6;
    CMidgardID summonOwner;
    /** Ids of units modified by this unit coupled with corresponding modifier ids. */
    ModifiedUnitsPatched modifiedUnits;
    /** Modifiers applied to this unit. */
    CMidgardID modifierIds[8];
    /** Total armor reduced by theurgist 'shatter' attacks. Negative values can increase armor. */
    int shatteredArmor;
    int fortificationArmor;
    /** Applied by game to mage units. Reduction does not shown in unit encyclopedia. */
    int attackPowerReduction;
};

static_assert(sizeof(UnitInfo) == 168, "Size of UnitInfo structure must be exactly 168 bytes");

static_assert(offsetof(UnitInfo, unitStatuses) == 8,
              "UnitInfo::unitStatuses offset must be 8 bytes");

static_assert(offsetof(UnitInfo, unitHp) == 44, "UnitInfo::unitHp offset must be 44 bytes");

static_assert(offsetof(UnitInfo, unitXp) == 46, "UnitInfo::unitXp offset must be 46 bytes");

static_assert(offsetof(UnitInfo, unitFlags) == 48, "UnitInfo::unitFlags offset must be 48 bytes");

static_assert(offsetof(UnitInfo, poisonAppliedRound) == 51,
              "UnitInfo::poisonAppliedRound offset must be 51 bytes");

static_assert(offsetof(UnitInfo, modifiedUnits) == 60,
              "UnitInfo::modifiedUnits offset must be 60 bytes");

static_assert(offsetof(UnitInfo, shatteredArmor) == 156,
              "UnitInfo::shatteredArmor offset must be 156 bytes");

/**
 * Common part of the network messages that is being sent during battle.
 */
struct BattleMsgData
{
    /**
     * 12 for alive units, another 12 is for units that can be possibly summoned over dead bodies
     * minus 2 because each group can only have 5 dead units, the 6-th one must be alive or the
     * battle is over.
     */
    UnitInfo unitsInfo[22];
    /** Order of turns in a single battle round. 12 for units and 1 reserved? */
    BattleTurn turnsOrder[13];
    CMidgardID attackerGroupId;
    /** Can be id of a stack, fortification or ruin. */
    CMidgardID defenderGroupId;
    CMidgardID attackerPlayerId;
    CMidgardID defenderPlayerId;
    CMidgardID attackerStackUnitIds[6];
    CMidgardID defenderStackUnitIds[6];
    CMidgardID ids3[2];
    /** Ids of items that were used during battle. */
    CMidgardID usedItemIds[4];
    std::int8_t currentRound; /**< Round counting starts from 1. */
    char padding[3];
    int unknown3;
    int unknown4;
    int unknown5;
    int unknown6;
    int unknown7;
    char unknown8;
    /** Holds leaders positions before duel. */
    char unknown9;
    /**
     * Indicates that battle is a duel.
     * Before duel starts, group leaders change their positions to 2, so they face each other.
     * Original positions are saved and restored when duel ends.
     * Non-leader units in both groups are marked with BattleStatus::Hidden.
     */
    bool duel;
    char padding2;
    char unknown11[4];
};

static_assert(sizeof(BattleMsgData) == 3920,
              "Size of BattleMsgData structure must be exactly 3920 bytes");

static_assert(offsetof(BattleMsgData, turnsOrder) == 3696,
              "BattleMsgData::turnsOrder offset must be 3696 bytes");

static_assert(offsetof(BattleMsgData, attackerStackUnitIds) == 3816,
              "BattleMsgData::attackerStackUnitIds offset must be 3816 bytes");

namespace BattleMsgDataApi {

struct Api
{
    using GetUnitStatus = bool(__thiscall*)(const BattleMsgData* thisptr,
                                            const CMidgardID* unitId,
                                            BattleStatus status);
    GetUnitStatus getUnitStatus;

    using SetUnitStatus = void(__thiscall*)(const BattleMsgData* thisptr,
                                            const CMidgardID* unitId,
                                            BattleStatus status,
                                            bool enable);
    SetUnitStatus setUnitStatus;

    using GetUnitIntValue = int(__thiscall*)(const BattleMsgData* thisptr,
                                             const CMidgardID* unitId);
    GetUnitIntValue getUnitShatteredArmor;
    GetUnitIntValue getUnitFortificationArmor;

    using SetUnitIntValue = void(__thiscall*)(BattleMsgData* thisptr,
                                              const CMidgardID* unitId,
                                              int value);
    SetUnitIntValue setUnitShatteredArmor;

    /** Sets UnitInfo::unitHp for unit with unitId to specified value. */
    using SetUnitInt16Value = void(__thiscall*)(BattleMsgData* thisptr,
                                                const CMidgardID* unitId,
                                                std::int16_t value);
    SetUnitInt16Value setUnitHp;

    /** Checks if unit is dead, updates UnitInfo::unitStatuses accordingly. */
    using CheckUnitDeath = void(__stdcall*)(const IMidgardObjectMap* objectMap,
                                            BattleMsgData* battleMsgData,
                                            const CMidgardID* unitId);
    CheckUnitDeath checkUnitDeath;

    using SetInt8Value = void(__thiscall*)(BattleMsgData* thisptr,
                                           const CMidgardID* unitId,
                                           std::int8_t value);
    SetInt8Value setDisableAppliedRound;

    using AddSummonedUnit = void(__thiscall*)(BattleMsgData* thisptr,
                                              const IMidgardObjectMap* objectMap,
                                              const CMidgardID* unitId,
                                              const CMidgardID* groupId);
    AddSummonedUnit addSummonedUnit;

    using SetSummonOwner = void(__thiscall*)(BattleMsgData* thisptr,
                                             const CMidgardID* summonUnitId,
                                             const CMidgardID* ownerUnitId);
    SetSummonOwner setSummonOwner;

    using IsUnitTransformed = bool(__stdcall*)(const CMidgardID* unitId,
                                               const BattleMsgData* battleMsgData);
    IsUnitTransformed isUnitTransformed;

    using RemoveTransformStatuses = void(__stdcall*)(const CMidgardID* unitId,
                                                     const BattleMsgData* battleMsgData);
    RemoveTransformStatuses removeTransformStatuses;

    using IsUnitAttacker = bool(__thiscall*)(const BattleMsgData* thisptr,
                                             const CMidgardID* unitId);
    IsUnitAttacker isUnitAttacker;

    GetUnitIntValue getAttackPowerReduction;
    SetUnitIntValue setAttackPowerReduction;

    using SetUnitBoolValue = void(__thiscall*)(BattleMsgData* thisptr,
                                               const CMidgardID* unitId,
                                               bool value);

    SetUnitBoolValue setUnitShown;

    using UnitHasModifier = bool(__stdcall*)(const BattleMsgData* battleMsgData,
                                             const CMidgardID* modifierId,
                                             const CMidgardID* unitId);
    UnitHasModifier unitHasModifier;

    using GetUnitInfoById = UnitInfo*(__stdcall*)(const BattleMsgData* battleMsgData,
                                                  const CMidgardID* unitId);
    GetUnitInfoById getUnitInfoById;

    /**
     * Calls check methods of the attack - from Method14 to Method17, then compares the results
     * against unit statuses (xp counted, dead, unsummoned, retreated and hidden) to see if the
     * attack can be performed. If all the checks are passed, calls CanPerform of the attack.
     */
    using CanPerformAttackOnUnitWithStatusCheck =
        bool(__stdcall*)(const IMidgardObjectMap* objectMap,
                         const BattleMsgData* battleMsgData,
                         const IBatAttack* batAttack,
                         const CMidgardID* unitId);
    CanPerformAttackOnUnitWithStatusCheck canPerformAttackOnUnitWithStatusCheck;

    using IsUnitAttackSourceWardRemoved = bool(__thiscall*)(BattleMsgData* thisptr,
                                                            const CMidgardID* unitId,
                                                            const LAttackSource* attackSource);
    IsUnitAttackSourceWardRemoved isUnitAttackSourceWardRemoved;

    using RemoveUnitAttackSourceWard = void(__thiscall*)(BattleMsgData* thisptr,
                                                         const CMidgardID* unitId,
                                                         const LAttackSource* attackSource);
    RemoveUnitAttackSourceWard removeUnitAttackSourceWard;

    using IsUnitAttackClassWardRemoved = bool(__thiscall*)(BattleMsgData* thisptr,
                                                           const CMidgardID* unitId,
                                                           const LAttackClass* attackClass);
    IsUnitAttackClassWardRemoved isUnitAttackClassWardRemoved;

    using RemoveUnitAttackClassWard = void(__thiscall*)(BattleMsgData* thisptr,
                                                        const CMidgardID* unitId,
                                                        const LAttackClass* attackClass);
    RemoveUnitAttackClassWard removeUnitAttackClassWard;

    using UnitCanBeHealed = bool(__stdcall*)(const IMidgardObjectMap* objectMap,
                                             const BattleMsgData* battleMsgData,
                                             const CMidgardID* unitId);
    UnitCanBeHealed unitCanBeHealed;

    using UnitCanBeCured = bool(__stdcall*)(const BattleMsgData* battleMsgData,
                                            const CMidgardID* unitId);
    UnitCanBeCured unitCanBeCured;

    using UnitCanBeRevived = bool(__stdcall*)(const BattleMsgData* battleMsgData,
                                              const CMidgardID* unitId);
    UnitCanBeRevived unitCanBeRevived;

    /** Called at the end of battle turn to cleanup temporary unit statuses. */
    using SetUnknown9Bit1AndClearBoostLowerDamage =
        void(__stdcall*)(const BattleMsgData* battleMsgData,
                         const CMidgardID* unitId,
                         CMidgardID* nextAttackUnitId);
    SetUnknown9Bit1AndClearBoostLowerDamage setUnknown9Bit1AndClearBoostLowerDamage;

    /** Called at the end of battle turn of a previous unit before a unit attacks. */
    using BeforeAttack = void(__stdcall*)(const BattleMsgData* battleMsgData,
                                          const IMidgardObjectMap* objectMap,
                                          const CMidgardID* unitId);
    BeforeAttack beforeAttack;

    /**
     * Resets info about units modified by this unit. Called in BeforeAttack after corresponding
     * modifiers are removed.
     */
    using ResetModifiedUnitsInfo = void(__thiscall*)(BattleMsgData* thisptr,
                                                     const CMidgardID* unitId);
    ResetModifiedUnitsInfo resetModifiedUnitsInfo;

    /**
     * Resets info about a modifier applied to this unit. Called in BeforeAttack after the modifier
     * is removed.
     */
    using ResetUnitModifierInfo = void(__thiscall*)(BattleMsgData* thisptr,
                                                    const CMidgardID* modifiedUnitId,
                                                    const CMidgardID* modifierId);
    ResetUnitModifierInfo resetUnitModifierInfo;

    /** Used by AI to determine attack target depending on attack class. */
    using FindAttackTarget = bool(__stdcall*)(const IMidgardObjectMap* objectMap,
                                              const CMidgardID* unitId,
                                              const IAttack* attack,
                                              const CMidUnitGroup* targetGroup,
                                              const TargetsList* targets,
                                              const BattleMsgData* battleMsgData,
                                              CMidgardID* value);
    FindAttackTarget findAttackTarget;

    /** Used by AI to determine attack target for L_ALL attack reach. */
    using FindAttackTargetWithAllReach = bool(__stdcall*)(CMidgardID* value,
                                                          const CMidUnitGroup* targetGroup,
                                                          const TargetsList* targets);
    FindAttackTargetWithAllReach findAttackTargetWithAllReach;

    using FindSpecificAttackTarget = bool(__stdcall*)(const IMidgardObjectMap* objectMap,
                                                      const BattleMsgData* battleMsgData,
                                                      const CMidUnitGroup* targetGroup,
                                                      const TargetsList* targets,
                                                      CMidgardID* value);
    /** Used by AI to determine boost attack target. */
    FindSpecificAttackTarget findBoostAttackTarget;
    /** Used by AI to determine fear attack target. */
    FindSpecificAttackTarget findFearAttackTarget;

    using FindDoppelgangerAttackTarget = bool(__stdcall*)(const IMidgardObjectMap* objectMap,
                                                          const CMidgardID* unitId,
                                                          const BattleMsgData* battleMsgData,
                                                          const CMidUnitGroup* targetGroup,
                                                          const TargetsList* targets,
                                                          CMidgardID* value);
    /** Used by AI to determine doppelganger attack target. */
    FindDoppelgangerAttackTarget findDoppelgangerAttackTarget;

    using FindDamageAttackTargetWithNonAllReach =
        bool(__stdcall*)(const IMidgardObjectMap* objectMap,
                         const IAttack* attack,
                         int damage,
                         const CMidUnitGroup* targetGroup,
                         const TargetsList* targets,
                         const BattleMsgData* battleMsgData,
                         CMidgardID* value);
    /** Used by AI to determine damage attack target with non-all reach. */
    FindDamageAttackTargetWithNonAllReach findDamageAttackTargetWithNonAllReach;

    using FindDamageAttackTargetWithAnyReach = bool(__stdcall*)(const IMidgardObjectMap* objectMap,
                                                                const CMidUnitGroup* targetGroup,
                                                                const TargetsList* targets,
                                                                int damage,
                                                                const BattleMsgData* battleMsgData,
                                                                const LAttackClass* attackClass,
                                                                const LAttackSource* attackSource,
                                                                const BattleStatus* filterByStatus,
                                                                CMidgardID* value);
    /** Used by AI to determine damage attack target with any reach. */
    FindDamageAttackTargetWithAnyReach findDamageAttackTargetWithAnyReach;

    using FindDamageAttackTargetWithAdjacentReach =
        CMidgardID*(__stdcall*)(CMidgardID* value,
                                const IMidgardObjectMap* objectMap,
                                const CMidUnitGroup* targetGroup,
                                const TargetsList* targets,
                                const BattleMsgData* battleMsgData,
                                const LAttackSource* attackSource,
                                const LAttackClass* attackClass);
    /** Used by AI to determine damage attack target with adjacent reach. */
    FindDamageAttackTargetWithAdjacentReach findDamageAttackTargetWithAdjacentReach;

    using AddUnitToBattleMsgData = void(__stdcall*)(const IMidgardObjectMap* objectMap,
                                                    const CMidUnitGroup* group,
                                                    const CMidgardID* unitId,
                                                    char attackerFlags,
                                                    BattleMsgData* battleMsgData);
    AddUnitToBattleMsgData addUnitToBattleMsgData;

    using Constructor = BattleMsgData*(__thiscall*)(BattleMsgData* thisptr);
    Constructor constructor;

    using CopyConstructor = BattleMsgData*(__thiscall*)(BattleMsgData* thisptr,
                                                        const BattleMsgData* src);
    CopyConstructor copyConstructor;
    CopyConstructor copyConstructor2;

    using CopyAssignment = BattleMsgData*(__thiscall*)(BattleMsgData* thisptr,
                                                       const BattleMsgData* src);
    CopyAssignment copyAssignment;

    using Copy = BattleMsgData*(__thiscall*)(BattleMsgData* thisptr, const BattleMsgData* src);
    Copy copy;

    using Destructor = void(__thiscall*)(BattleMsgData* thisptr);
    Destructor destructor;

    using RemoveUnitInfo = void(__thiscall*)(BattleMsgData* thisptr, const CMidgardID* unitId);
    RemoveUnitInfo removeUnitInfo;

    using DecreaseUnitAttacks = bool(__thiscall*)(BattleMsgData* thisptr, const CMidgardID* unitId);
    DecreaseUnitAttacks decreaseUnitAttacks;

    using GetTargetsToAttack = void(__stdcall*)(IdList* value,
                                                const IMidgardObjectMap* objectMap,
                                                const IAttack* attack,
                                                const IBatAttack* batAttack,
                                                const LAttackReach* attackReach,
                                                const BattleMsgData* battleMsgData,
                                                BattleAction action,
                                                const CMidgardID* targetUnitId);
    GetTargetsToAttack getTargetsToAttack;

    using GetLeaderEquippedBattleItemIndex = int(__stdcall*)(const IMidgardObjectMap* objectMap,
                                                             const CMidgardID* unitId,
                                                             const BattleMsgData* battleMsgData,
                                                             const CMidgardID* itemId);
    GetLeaderEquippedBattleItemIndex getLeaderEquippedBattleItemIndex;

    using GetUnitInfos = void(__thiscall*)(BattleMsgData* thisptr, UnitInfoList* value, bool a3);
    GetUnitInfos getUnitInfos;

    /**
     * Fills targets list for a specified attack.
     * @param[in] objectMap map where to search for objects.
     * @param[in] battleMsgData battle information.
     * @param[in] batAttack battle attack for which targets list will be filled.
     * @param[in] unitId id of a unit performing the attack.
     * @param[in] attackUnitOrItemId unit or item id performing the attack.
     * @param targetAllies specifies whether the attack should target allies or not.
     * @param[inout] targetsList list to fill.
     * @param[in] checkAltAttack specifies whether alt-attack should be checked to fill targets for.
     */
    using FillTargetsList = void(__stdcall*)(const IMidgardObjectMap* objectMap,
                                             const BattleMsgData* battleMsgData,
                                             const IBatAttack* batAttack,
                                             const CMidgardID* unitId,
                                             const CMidgardID* attackUnitOrItemId,
                                             bool targetAllies,
                                             TargetsList* value,
                                             bool checkAltAttack);
    FillTargetsList fillTargetsList;

    using FillTargetsListForAllAnyAttackReach = void(__stdcall*)(const IMidgardObjectMap* objectMap,
                                                                 const BattleMsgData* battleMsgData,
                                                                 const IBatAttack* batAttack,
                                                                 const CMidgardID* targetGroupId,
                                                                 TargetsList* value);
    FillTargetsListForAllAnyAttackReach fillTargetsListForAllAttackReach;
    FillTargetsListForAllAnyAttackReach fillTargetsListForAnyAttackReach;

    using FillTargetsListForAdjacentAttackReach =
        void(__stdcall*)(const IMidgardObjectMap* objectMap,
                         const BattleMsgData* battleMsgData,
                         const IBatAttack* batAttack,
                         const CMidgardID* targetGroupId,
                         const CMidgardID* unitGroupId,
                         const CMidgardID* unitId,
                         TargetsList* value);
    FillTargetsListForAdjacentAttackReach fillTargetsListForAdjacentAttackReach;

    using FillEmptyTargetsList = void(__stdcall*)(const IMidgardObjectMap* objectMap,
                                                  const BattleMsgData* battleMsgData,
                                                  const IBatAttack* batAttack,
                                                  const CMidgardID* unitId,
                                                  const CMidgardID* attackUnitOrItemId,
                                                  bool targetAllies,
                                                  TargetsList* value);
    FillEmptyTargetsList fillEmptyTargetsList;

    using FillEmptyTargetsListForAllAnyAttackReach =
        void(__stdcall*)(const IMidgardObjectMap* objectMap,
                         const CMidgardID* targetGroupId,
                         TargetsList* value);
    FillEmptyTargetsListForAllAnyAttackReach fillEmptyTargetsListForAllAttackReach;
    FillEmptyTargetsListForAllAnyAttackReach fillEmptyTargetsListForAnyAttackReach;

    FillTargetsListForAdjacentAttackReach fillEmptyTargetsListForAdjacentAttackReach;

    using IsAutoBattle = bool(__thiscall*)(const BattleMsgData* thisptr);
    IsAutoBattle isAutoBattle;

    using AlliesNotPreventingAdjacentAttack = bool(__stdcall*)(const BattleMsgData* battleMsgData,
                                                               const CMidUnitGroup* unitGroup,
                                                               int unitPosition,
                                                               bool targetsAreAllies);
    AlliesNotPreventingAdjacentAttack alliesNotPreventingAdjacentAttack;

    using GiveAttack = void(__thiscall*)(BattleMsgData* thistr,
                                         const CMidgardID* unitId,
                                         char attacksCount,
                                         int lastIndex);
    GiveAttack giveAttack;
};

Api& get();

} // namespace BattleMsgDataApi

} // namespace game

#endif // BATTLEMSGDATA_H
