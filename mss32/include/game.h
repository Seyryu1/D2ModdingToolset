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

#ifndef GAME_H
#define GAME_H

#include "idlist.h"

namespace game {

struct TRaceType;
struct CPlayerBuildings;
struct LUnitBranch;
struct CMidDataCache2;
struct CMidgardID;
struct CDBTable;
struct GlobalData;
struct IMidObject;
struct IUsSoldier;
struct IMidgardObjectMap;
struct TLordType;
struct CMidPlayer;
struct BattleMsgData;
struct CMidUnit;
struct TUsUnitImpl;
struct LAttackClass;
struct IBatAttack;
struct IAttack;
struct CMidUnitGroup;

/** Sets initial values for 'show resources' and 'minimap mode' toggle buttons. */
using RespopupInitFunc = void (*)(void);

/** Sets initial value for 'show banners' toggle button. */
using ToggleShowBannersInitFunc = void*(__thiscall*)(void* thisptr);

/**
 * Assumption: processes unit modifiers during battle.
 * Called with unit pointer being a result of dynamic_cast without a nullptr checks.
 * Main reason of crashes in battles when summoners are involved.
 */
using ProcessUnitModifiersFunc = bool(__thiscall*)(void* unit, int* a2);

/**
 * Adds tier-1 unit from specified unit branch to player's hire list.
 * @param[in] race player race.
 * @param[in] buildings player buildings.
 * @param[in] branch determines unit to add.
 * @param[inout] hireList list to store results.
 * @returns unknown.
 */
using AddUnitToHireList = void*(__stdcall*)(TRaceType* race,
                                            CPlayerBuildings* buildings,
                                            const LUnitBranch* branch,
                                            IdList* hireList);

/**
 * Adds unit from sideshow branch to player's hire list.
 * @param[in] race player race.
 * @param[in] buildings player buildings.
 * @param[inout] hireList list to store results.
 * @returns unknown.
 */
using AddSideshowUnitToHireList = void*(__stdcall*)(TRaceType* race,
                                                    CPlayerBuildings* buildings,
                                                    IdList* hireList);

/**
 * Adds player's tier-1 units to hire list.
 * @param[in] dataCache pointer to data cache object.
 * @param[in] playerId id of player to process.
 * @param[in] a3 meaning unknown.
 * @param[inout] hireList list to store results.
 * @returns true if units were added to the list.
 */
using AddPlayerUnitsToHireList = bool(__stdcall*)(CMidDataCache2* dataCache,
                                                  const CMidgardID* playerId,
                                                  const CMidgardID* a3,
                                                  IdList* hireList);

/**
 * Callback function used to process each record in GBuild.dbf.
 * Creates TBuildingType or TBuildingUnitUpgType objects from record data and stores them in a2.
 */
using CreateBuildingType = void(__stdcall*)(const CDBTable* dbTable,
                                            void* a2,
                                            const GlobalData** globalData);

/**
 * Adds object to dictionary-like structure if it's not a duplicate.
 * @returns false in case of duplicating object.
 */
using AddObjectAndCheckDuplicates = bool(__thiscall*)(void* thisptr, const IMidObject* object);

/**
 * Decides where to place unit in group.
 * @returns 0 for back lane, 1 for forward lane.
 */
using ChooseUnitLane = int(__stdcall*)(const IUsSoldier* soldier);

/** Assumption: returns current player id. */
using GetPlayerIdFromPhase = const CMidgardID*(__thiscall*)(int phase);
using GetObjectMapFromPhase = const IMidgardObjectMap*(__thiscall*)(int phase);

/** Returns lord type for specified player. */
using GetLordByPlayer = const TLordType*(__stdcall*)(const CMidPlayer* player);

/** Returns true if current turn value is in allowed range [0 : 999]. */
using IsTurnValid = bool(__stdcall*)(int turn);

/** Returns stack id of allies or enemies for specified unit id. */
using GetAllyOrEnemyStackId = CMidgardID*(__stdcall*)(CMidgardID* stackId,
                                                      const BattleMsgData* battleMsgData,
                                                      const CMidgardID* unitId,
                                                      bool allies);

/** Returns pointer to unit found by specified id. */
using FindUnitById = CMidUnit*(__stdcall*)(const IMidgardObjectMap* objectMap,
                                           const CMidgardID* unitId);

using CastUnitImplToSoldier = IUsSoldier*(__stdcall*)(TUsUnitImpl* unitImpl);

using CreateBatAttack = IBatAttack*(__stdcall*)(IMidgardObjectMap* objectMap,
                                                BattleMsgData* battleMsgData,
                                                const CMidgardID* id1,
                                                const CMidgardID* id2,
                                                int attackNumber,
                                                const LAttackClass* attackClass,
                                                bool a7);

using GetAttackByIdAndCheckTransfomed = IAttack*(__stdcall*)(IMidgardObjectMap* objectMap,
                                                             const CMidgardID* id,
                                                             int attackNumber);

using IsUnitImmuneToAttack = bool(__stdcall*)(IMidgardObjectMap* objectMap,
                                              BattleMsgData* battleMsgData,
                                              const CMidgardID* unitId,
                                              IAttack* attack,
                                              bool checkAlwaysImmuneOnly);

using AttackClassToNumber = int(__stdcall*)(const LAttackClass* attackClass);
using AttackClassToString = const char*(__stdcall*)(const LAttackClass* attackClass);

/**
 * Returns pointer to units group of stack, fortification or ruin
 * Actual object type found determined by specified id IdType.
 */
using GetStackFortRuinGroup = CMidUnitGroup*(__thiscall*)(void* thisptr,
                                                          IMidgardObjectMap* objectMap,
                                                          const CMidgardID* objectId);

/** Game and editor functions that can be hooked. */
struct Functions
{
    RespopupInitFunc respopupInit;
    ToggleShowBannersInitFunc toggleShowBannersInit;
    ProcessUnitModifiersFunc processUnitModifiers;
    AddUnitToHireList addUnitToHireList;
    AddSideshowUnitToHireList addSideshowUnitToHireList;
    AddPlayerUnitsToHireList addPlayerUnitsToHireList;
    CreateBuildingType createBuildingType;
    AddObjectAndCheckDuplicates addObjectAndCheckDuplicates;
    ChooseUnitLane chooseUnitLane;
    GetPlayerIdFromPhase getPlayerIdFromPhase;
    GetObjectMapFromPhase getObjectMapFromPhase;
    GetLordByPlayer getLordByPlayer;
    IsTurnValid isTurnValid;
    GetAllyOrEnemyStackId getAllyOrEnemyStackId;
    FindUnitById findUnitById;
    CastUnitImplToSoldier castUnitImplToSoldier;
    CreateBatAttack createBatAttack;
    GetAttackByIdAndCheckTransfomed getAttackByIdAndCheckTransfomed;
    IsUnitImmuneToAttack isUnitImmuneToAttack;
    AttackClassToNumber attackClassToNumber;
    AttackClassToString attackClassToString;
    GetStackFortRuinGroup getStackFortRuinGroup;
};

/** Global variables used in game. */
struct Variables
{
    int* minimapMode; /**< Value of 0 shows minimap, 1 shows percentage of land converted. */
    unsigned char* respopup;
};

/** Returns functions according to determined version of the game. */
Functions& gameFunctions();

/** Returns variables according to determined version of the game. */
Variables& gameVariables();

} // namespace game

#endif // GAME_H
