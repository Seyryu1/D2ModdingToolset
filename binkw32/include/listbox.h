/*
 * This file is part of the binkw32 proxy dll for Disciples 2.
 * (https://github.com/VladimirMakeev/D2Binkw32Proxy)
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

#ifndef LISTBOX_H
#define LISTBOX_H

#include "interface.h"
#include <cstddef>

namespace game {

struct CDialogInterf;
struct Functor;

/** Must be 148 bytes according to CListBoxInterf constructor. */
struct CListBoxInterfData
{
    char unknown[148];
};

/** List box ui element. */
struct CListBoxInterf : public CInterface
{
    CListBoxInterfData* listBoxData;

    /** Assumption: interface for list box elements rendering. */
    struct LBDisplay
    {
        const void* vftable;
    };
};

static_assert(sizeof(CListBoxInterf) == 12,
              "Size of CListBoxInterf structure must be exactly 12 bytes");

static_assert(offsetof(CListBoxInterf, listBoxData) == 8,
              "CListBoxInterf::listBoxData offset must be 8 bytes");

/** Must be 48 bytes according to CListBoxDisplayText constructor. */
struct CListBoxDisplayTextData
{
    char unknown[48];
};

/** Assumption: implements rendering for list box containing text elements. */
struct CListBoxDisplayText : public CListBoxInterf::LBDisplay
{
    CListBoxDisplayTextData* data;
};

static_assert(sizeof(CListBoxDisplayText) == 8,
              "Size of CListBoxDisplayText structure must be exactly 8 bytes");

namespace CListBoxInterfApi {

struct Api
{
    /**
     * Assigns functor with callback to a list box.
     * Searches for list box element by its and dialog names.
     * Callback will be called each time when list box selection changes.
     * @param[in] dialog used for list box search.
     * @param[in] buttonName list box to assign functor to.
     * @param[in] dialogName dialog (menu) where to search for list box.
     * @param[in] functor pointer to existing functor initialized with callback.
     * @returns list box found with callback set or nullptr if not found.
     */
    using AssignFunctor = CListBoxInterf*(__stdcall*)(CDialogInterf* dialog,
                                                      const char* listBoxName,
                                                      const char* dialogName,
                                                      Functor* functor);
    AssignFunctor assignFunctor;

    /**
     * Assigns functor with callback to a list box display text element.
     * Searches for list box element by its and dialog names.
     * Callback will be called each time when list box selection changes.
     * @param[in] dialog used for list box search.
     * @param[in] buttonName list box to assign functor to.
     * @param[in] dialogName dialog (menu) where to search for list box.
     * @param[in] functor pointer to existing functor initialized with callback.
     * @param addTextShortenedMark if true, adds '[...]' to shortened list box text elements.
     * @returns list box found with callback set or nullptr if not found.
     */
    using AssignDisplayTextFunctor = CListBoxInterf*(__stdcall*)(CDialogInterf* dialog,
                                                                 const char* listBoxName,
                                                                 const char* dialogName,
                                                                 Functor* functor,
                                                                 bool addTextShortenedMark);
    AssignDisplayTextFunctor assignDisplayTextFunctor;
};

Api& get();

} // namespace CListBoxInterfApi

} // namespace game

#endif // LISTBOX_H
