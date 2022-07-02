/* This file is part of the the dci project. Copyright (C) 2013-2022 vopl, shtoba.
   This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public
   License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.
   You should have received a copy of the GNU Affero General Public License along with this program. If not, see <https://www.gnu.org/licenses/>. */

#pragma once

#include "pch.hpp"

namespace dci::module::ppn::connectivity::demand
{
    class Registry;

    class Element
        : public dci::mm::heap::Allocable<Element>
    {
    public:
        using Clock = std::chrono::steady_clock;
        using Moment = Clock::time_point;

    public:
        Element(const node::link::Id& id, Registry* r);
        ~Element();

        const node::link::Id& id() const;

        void updatePriority() const;
        real64 priority() const;
        real64 totalPriority() const;
        void totalPriority(real64 v) const;

        bool tryUnban(Moment now) const;

    public:
        api::NeedHolder<> need(real64 weight) const;
        api::SatisfactionHolder<> satisfy() const;

    private:
        node::link::Id                                                              _id;
        mutable Registry *                                                          _r {};

        struct NeedState
        {
            sbs::Owner  _sol;
            real64      _weight{};
        };
        struct SatisfactionState
        {
            sbs::Owner  _sol;
        };

        mutable std::map<api::NeedHolder<>::Opposite, NeedState>                    _needs;
        mutable std::map<api::SatisfactionHolder<>::Opposite, SatisfactionState>    _satisfies;

        mutable std::multiset<Moment>                                               _bans;

    private:
        mutable real64 _priority {};
        mutable real64 _totalPriority {};
    };
}
