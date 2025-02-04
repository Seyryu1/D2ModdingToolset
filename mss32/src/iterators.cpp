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

#include "iterators.h"
#include "version.h"
#include <array>

namespace game::Iterators {

// clang-format off
static std::array<Api, 4> functions = {{
    // Akella
    Api{
        (Api::CreateIterator)0,
        (Api::CreateIterator)0,
        (Api::CreateIterator)0x419d24,
        (Api::CreateIterator)0x419dca,
        (Api::CreateIterator)0,
        (Api::CreateIterator)0,
        (Api::CreateIterator)0,
        (Api::CreateIterator)0,
    },
    // Russobit
    Api{
        (Api::CreateIterator)0,
        (Api::CreateIterator)0,
        (Api::CreateIterator)0x419d24,
        (Api::CreateIterator)0x419dca,
        (Api::CreateIterator)0,
        (Api::CreateIterator)0,
        (Api::CreateIterator)0,
        (Api::CreateIterator)0,
    },
    // Gog
    Api{
        (Api::CreateIterator)0,
        (Api::CreateIterator)0,
        (Api::CreateIterator)0x41983f,
        (Api::CreateIterator)0x4198e5,
        (Api::CreateIterator)0,
        (Api::CreateIterator)0,
        (Api::CreateIterator)0,
        (Api::CreateIterator)0,
    },
    // Scenario Editor
    Api{
        (Api::CreateIterator)0x41a62a,
        (Api::CreateIterator)0x41a6d0,
        (Api::CreateIterator)0x41de25,
        (Api::CreateIterator)0x41decb,
        (Api::CreateIterator)0x4051fc,
        (Api::CreateIterator)0x4052a2,
        (Api::CreateIterator)0x41ff4a,
        (Api::CreateIterator)0x41fff0,
        (Api::CreateIterator)0x41ef16,
        (Api::CreateIterator)0x41efbc,
        (Api::CreateIterator)0x405341,
        (Api::CreateIterator)0x4053e7,
    },
}};
// clang-format on

Api& get()
{
    return functions[static_cast<int>(hooks::gameVersion())];
}

} // namespace game::Iterators
