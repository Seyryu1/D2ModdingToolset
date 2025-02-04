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

#include "scenarioview.h"
#include "dynamiccast.h"
#include "locationview.h"
#include "midgardmapblock.h"
#include "midgardobjectmap.h"
#include "midscenvariables.h"
#include "scenarioinfo.h"
#include "scenvariablesview.h"
#include "tileview.h"
#include "utils.h"
#include <sol/sol.hpp>

namespace bindings {

ScenarioView::ScenarioView(const game::IMidgardObjectMap* objectMap)
    : objectMap(objectMap)
{ }

void ScenarioView::bind(sol::state& lua)
{
    auto scenario = lua.new_usertype<ScenarioView>("Scenario");
    scenario["getLocation"] = sol::overload<>(&ScenarioView::getLocation,
                                              &ScenarioView::getLocationById);
    scenario["variables"] = sol::property(&ScenarioView::getScenVariables);
    scenario["getTile"] = sol::overload<>(&ScenarioView::getTile, &ScenarioView::getTileByPoint);
    scenario["day"] = sol::property(&ScenarioView::getCurrentDay);
    scenario["size"] = sol::property(&ScenarioView::getSize);
}

std::optional<LocationView> ScenarioView::getLocation(const std::string& id) const
{
    return getLocationById(IdView{id});
}

std::optional<LocationView> ScenarioView::getLocationById(const IdView& id) const
{
    using namespace game;

    auto obj = objectMap->vftable->findScenarioObjectById(objectMap, &id.id);
    if (!obj) {
        return std::nullopt;
    }

    const auto dynamicCast = RttiApi::get().dynamicCast;
    const auto& rtti = RttiApi::rtti();

    auto* location = (const game::CMidLocation*)dynamicCast(obj, 0, rtti.IMidScenarioObjectType,
                                                            rtti.CMidLocationType, 0);
    if (!location) {
        return std::nullopt;
    }

    return LocationView{location};
}

std::optional<ScenVariablesView> ScenarioView::getScenVariables() const
{
    const auto variablesId{hooks::createScenarioVariablesId(objectMap)};
    auto variablesObj = objectMap->vftable->findScenarioObjectById(objectMap, &variablesId);

    auto variables = static_cast<const game::CMidScenVariables*>(variablesObj);
    if (!variables) {
        return std::nullopt;
    }

    return ScenVariablesView{variables};
}

std::optional<TileView> ScenarioView::getTile(int x, int y) const
{
    auto info = getScenarioInfo();
    if (!info) {
        return std::nullopt;
    }

    if (x < 0 || x >= info->mapSize || y < 0 || y >= info->mapSize) {
        // Outside of map
        return std::nullopt;
    }

    using namespace game;

    const auto& id = CMidgardIDApi::get();
    CMidgardID blockId{};
    const std::uint32_t blockX = x / 8 * 8;
    const std::uint32_t blockY = y / 4 * 4;
    id.fromParts(&blockId, IdCategory::Scenario, id.getCategoryIndex(&info->infoId),
                 IdType::MapBlock, blockX | (blockY << 8));

    auto blockObj = objectMap->vftable->findScenarioObjectById(objectMap, &blockId);
    if (!blockObj) {
        return std::nullopt;
    }

    auto block = static_cast<const CMidgardMapBlock*>(blockObj);
    const auto& blockPos = block->position;
    if (x < blockPos.x || x >= blockPos.x + 8 || y < blockPos.y || y >= blockPos.y + 4) {
        // Outside of map block
        return std::nullopt;
    }

    const auto index = x + 8 * (y - blockPos.y) - blockPos.x;
    if (index < 0 || index >= 32) {
        return std::nullopt;
    }

    return TileView{block->tiles[index]};
}

std::optional<TileView> ScenarioView::getTileByPoint(const Point& p) const
{
    return getTile(p.x, p.y);
}

int ScenarioView::getCurrentDay() const
{
    auto info = getScenarioInfo();
    return info ? info->currentTurn : 0;
}

int ScenarioView::getSize() const
{
    auto info = getScenarioInfo();
    return info ? info->mapSize : 0;
}

const game::CScenarioInfo* ScenarioView::getScenarioInfo() const
{
    using namespace game;

    const auto& id = CMidgardIDApi::get();
    auto scenarioId = objectMap->vftable->getId(objectMap);

    CMidgardID infoId{};
    id.fromParts(&infoId, id.getCategory(scenarioId), id.getCategoryIndex(scenarioId),
                 IdType::ScenarioInfo, 0);

    auto infoObj = objectMap->vftable->findScenarioObjectById(objectMap, &infoId);
    if (!infoObj) {
        return nullptr;
    }

    return static_cast<const CScenarioInfo*>(infoObj);
}

} // namespace bindings
