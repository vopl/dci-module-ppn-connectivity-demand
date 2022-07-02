/* This file is part of the the dci project. Copyright (C) 2013-2022 vopl, shtoba.
   This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public
   License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.
   You should have received a copy of the GNU Affero General Public License along with this program. If not, see <https://www.gnu.org/licenses/>. */

#include "pch.hpp"
#include "demand.hpp"

namespace dci::module::ppn::connectivity
{
    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Demand::Demand()
        : idl::ppn::connectivity::Demand<>::Opposite(idl::interface::Initializer())
    {
        {
            node::Feature<>::Opposite op = *this;

            op->setup() += sol() * [this](node::feature::Service<> srv)
            {
                srv->start() += sol() * [this, srv]() mutable
                {
                    _started = true;
                    _registry.start();
                };

                srv->stop() += sol() * [this]
                {
                    _started = false;
                    _registry.stop();
                };

                srv->registerAgentProvider(api::Registry<>::lid(), *this);
            };
        }

        {
            idl::Configurable<>::Opposite op = *this;

            op->configure() += sol() * [this](dci::idl::Config&& config)
            {
                auto c = config::cnvt(std::move(config));

                _registry.setIntensity(std::atof(c.get("intensity", "10").data()));

                return cmt::readyFuture<>();
            };
        }

        methods()->getAgent() += sol() * [this](idl::ILid ilid)
        {
            if(api::Registry<>::lid() == ilid)
            {
                return cmt::readyFuture<idl::Interface>(idl::Interface{_registry});
            }

            dbgWarn("crazy node?");
            return cmt::readyFuture<idl::Interface>(exception::buildInstance<api::Error>("bad agent ilid requested"));
        };
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Demand::~Demand()
    {
        _started = false;

        sol().flush();
    }
}
