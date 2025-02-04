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

#include "menubase.h"
#include "version.h"
#include <array>

namespace game::CMenuBaseApi {

// clang-format off
static std::array<Api, 3> functions = {{
    // Akella
    Api{
        (Api::Constructor)0x4db5ea,
        (Api::CreateMenu)0x4db76a,
        (Api::GetDialogInterface)0x40306a,
        (Api::ButtonCallback)0x4dcf9e,
        (Api::CreateButtonFunctor)0x4ea842,
    },
    // Russobit
    Api{
        (Api::Constructor)0x4db5ea,
        (Api::CreateMenu)0x4db76a,
        (Api::GetDialogInterface)0x40306a,
        (Api::ButtonCallback)0x4dcf9e,
        (Api::CreateButtonFunctor)0x4ea842,
    },
    // Gog
    Api{
        (Api::Constructor)0x4dac4f,
        (Api::CreateMenu)0x4dadcf,
        (Api::GetDialogInterface)0x402db0,
        (Api::ButtonCallback)0x4dbeae,
        (Api::CreateButtonFunctor)0x4e9cdd,
    },
}};
// clang-format on

Api& get()
{
    return functions[static_cast<int>(hooks::gameVersion())];
}

} // namespace game::CMenuBaseApi
