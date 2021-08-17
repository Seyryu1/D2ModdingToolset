/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2021 Vladimir Makeev.
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

#ifndef MIDEVCONDITIONHOOKS_H
#define MIDEVCONDITIONHOOKS_H

namespace game {
struct CMidEvCondition;
struct LEventCondCategory;
struct String;
struct IMidgardObjectMap;
} // namespace game

namespace hooks {

game::CMidEvCondition* __stdcall createEventConditionFromCategoryHooked(
    const game::LEventCondCategory* category);

void __stdcall eventConditionGetInfoStringHooked(game::String* info,
                                                 const game::IMidgardObjectMap* objectMap,
                                                 const game::CMidEvCondition* eventCondition);

void __stdcall eventConditionGetDescriptionHooked(game::String* description,
                                                  const game::LEventCondCategory* category);

void __stdcall eventConditionGetBriefHooked(game::String* brief,
                                            const game::LEventCondCategory* category);

} // namespace hooks

#endif // MIDEVCONDITIONHOOKS_H
