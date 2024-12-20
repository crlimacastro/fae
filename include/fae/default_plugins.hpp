#pragma once

#include "fae/time.hpp"
#include "fae/input.hpp"
#include "fae/windowing.hpp"
#include "fae/rendering/rendering.hpp"
#include "fae/lighting.hpp"
#include "fae/ui.hpp"

namespace fae
{
    struct application;

    struct default_plugins
    {
        time_plugin time_plugin{};
        input_plugin input_plugin{};
        windowing_plugin windowing_plugin{};
        rendering_plugin rendering_plugin{};
        lighting_plugin lighting_plugin{};
        ui_plugin ui_plugin{};

        auto init(application& app) const noexcept -> void;
    };
}
