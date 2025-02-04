/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2021 Stanislav Egorov.
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

#include "textids.h"
#include "log.h"
#include "scripts.h"
#include "utils.h"
#include <fmt/format.h>
#include <sol/sol.hpp>

namespace hooks {

void readOwnResourceTextIds(const sol::table& table,
                            TextIds::Events::Conditions::OwnResource& value)
{
    value.tooMany = table.get_or("tooMany", std::string());
    value.mutuallyExclusive = table.get_or("mutuallyExclusive", std::string());
}

void readGameModeTextIds(const sol::table& table, TextIds::Events::Conditions::GameMode& value)
{
    value.tooMany = table.get_or("tooMany", std::string());
    value.single = table.get_or("single", std::string());
    value.hotseat = table.get_or("hotseat", std::string());
    value.online = table.get_or("online", std::string());
}

void readPlayerTypeTextIds(const sol::table& table, TextIds::Events::Conditions::PlayerType& value)
{
    value.tooMany = table.get_or("tooMany", std::string());
    value.human = table.get_or("human", std::string());
    value.ai = table.get_or("ai", std::string());
}

void readVariableCmpTextIds(const sol::table& table,
                            TextIds::Events::Conditions::VariableCmp& value)
{
    value.equal = table.get_or("equal", std::string());
    value.notEqual = table.get_or("notEqual", std::string());
    value.greater = table.get_or("greater", std::string());
    value.greaterEqual = table.get_or("greaterEqual", std::string());
    value.less = table.get_or("less", std::string());
    value.lessEqual = table.get_or("lessEqual", std::string());
}

void readConditionsTextIds(const sol::table& table, TextIds::Events::Conditions& value)
{
    auto conditions = table.get<sol::optional<sol::table>>("conditions");
    if (!conditions.has_value())
        return;

    auto ownResource = conditions.value().get<sol::optional<sol::table>>("ownResource");
    if (ownResource.has_value()) {
        readOwnResourceTextIds(ownResource.value(), value.ownResource);
    }

    auto gameMode = conditions.value().get<sol::optional<sol::table>>("gameMode");
    if (gameMode.has_value()) {
        readGameModeTextIds(gameMode.value(), value.gameMode);
    }

    auto playerType = conditions.value().get<sol::optional<sol::table>>("playerType");
    if (playerType.has_value()) {
        readPlayerTypeTextIds(playerType.value(), value.playerType);
    }

    auto variableCmp = conditions.value().get<sol::optional<sol::table>>("variableCmp");
    if (variableCmp.has_value()) {
        readVariableCmpTextIds(variableCmp.value(), value.variableCmp);
    }
}

void readEventsTextIds(const sol::table& table, TextIds::Events& value)
{
    auto events = table.get<sol::optional<sol::table>>("events");
    if (!events.has_value())
        return;

    readConditionsTextIds(events.value(), value.conditions);
}

void readInterfTextIds(const sol::table& table, TextIds::Interf& value)
{
    auto interf = table.get<sol::optional<sol::table>>("interf");
    if (!interf.has_value())
        return;

    value.sellAllValuables = interf.value().get_or("sellAllValuables", std::string());
    value.sellAllItems = interf.value().get_or("sellAllItems", std::string());
    value.infiniteAttack = interf.value().get_or("infiniteAttack", std::string());
    value.critHitAttack = interf.value().get_or("critHitAttack", std::string());
    value.critHitDamage = interf.value().get_or("critHitDamage", std::string());
    value.ratedDamage = interf.value().get_or("ratedDamage", std::string());
    value.ratedDamageEqual = interf.value().get_or("ratedDamageEqual", std::string());
    value.ratedDamageSeparator = interf.value().get_or("ratedDamageSeparator", std::string());
    value.splitDamage = interf.value().get_or("splitDamage", std::string());
}

void initialize(TextIds& value)
{
    const auto path{hooks::scriptsFolder() / "textids.lua"};
    try {
        const auto lua{loadScriptFile(path)};
        if (!lua)
            return;

        const sol::table& table = (*lua)["textids"];
        readInterfTextIds(table, value.interf);
        readEventsTextIds(table, value.events);
    } catch (const std::exception& e) {
        showErrorMessageBox(fmt::format("Failed to read script '{:s}'.\n"
                                        "Reason: '{:s}'",
                                        path.string(), e.what()));
    }
}

const TextIds& textIds()
{
    static TextIds value;
    static bool initialized = false;

    if (!initialized) {
        initialize(value);
        initialized = true;
    }

    return value;
}

} // namespace hooks
