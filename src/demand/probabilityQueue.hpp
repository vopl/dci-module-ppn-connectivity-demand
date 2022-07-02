/* This file is part of the the dci project. Copyright (C) 2013-2022 vopl, shtoba.
   This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public
   License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.
   You should have received a copy of the GNU Affero General Public License along with this program. If not, see <https://www.gnu.org/licenses/>. */

#pragma once

#include "pch.hpp"
#include "element.hpp"

namespace dci::module::ppn::connectivity::demand
{
    class ProbabilityQueue
    {
    public:
        ProbabilityQueue();
        ~ProbabilityQueue();

        void insert(const Element* e);
        void update(const std::set<const Element *>& es);
        void delete_(const Element* e);
        void clear();

        const Element* sample();

    private:
        void update();

    private:
        //TODO пока просто заглушка, сюда надо нормальную "очередь с вероятностными приоритетами"
        std::deque<const Element *> _elements;
        real64                      _totalPriority{};
    };
}
