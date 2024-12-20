#pragma once

#include <algorithm>
#include <array>
#include <cstdint>

#include "fae/math.hpp"

namespace fae
{
    struct color_hsva;

    struct color_rgba
    {
        std::uint8_t r = 0;
        std::uint8_t g = 0;
        std::uint8_t b = 0;
        std::uint8_t a = 255;

        [[nodiscard]] static auto from_hsva(const color_hsva& color) noexcept -> color_rgba;
        [[nodiscard]] auto to_hsva() const noexcept -> color_hsva;

        [[nodiscard]] inline constexpr auto to_array() const noexcept -> std::array<float, 4>
        {
            return { r / 255.f, g / 255.f, b / 255.f, a / 255.f };
        }

        [[nodiscard]] static inline constexpr auto from_array(std::array<float, 4> value) noexcept -> color_rgba
        {
            return {
                static_cast<std::uint8_t>(value[0] * 255.f),
                static_cast<std::uint8_t>(value[1] * 255.f),
                static_cast<std::uint8_t>(value[2] * 255.f),
                static_cast<std::uint8_t>(value[3] * 255.f),
            };
        }

        [[nodiscard]] inline constexpr auto to_vec4() const noexcept -> fae::vec4
        {
            return { r / 255.f, g / 255.f, b / 255.f, a / 255.f };
        }
    };
    using color = color_rgba;

    struct color_hsva
    {
        float h = 0.f;
        float s = 0.f;
        float v = 0.f;
        std::uint8_t a = 255;

        [[nodiscard]] static auto from_rgba(const color_rgba& color) noexcept -> color_hsva;
        [[nodiscard]] auto to_rgba() const noexcept -> color_rgba;
    };

    namespace colors
    {
        constexpr auto black = color{ 0, 0, 0 };
        constexpr auto white = color{ 255, 255, 255 };
        constexpr auto red = color{ 255, 0, 0 };
        constexpr auto green = color{ 0, 255, 0 };
        constexpr auto blue = color{ 0, 0, 255 };
        constexpr auto cornflower_blue = color{ 100, 149, 237 };
    }
}
