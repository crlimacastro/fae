#pragma once

#include <cstdint>
#include <array>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/euler_angles.hpp>

namespace fae
{
    using vec2 = glm::vec2;
    using vec3 = glm::vec3;
    using vec4 = glm::vec4;
    using quat = glm::quat;
    using mat2 = glm::mat2;
    using mat3 = glm::mat3;
    using mat4 = glm::mat4;
    using mat2x2 = glm::mat2x2;
    using mat2x3 = glm::mat2x3;
    using mat2x4 = glm::mat2x4;
    using mat3x2 = glm::mat3x2;
    using mat3x3 = glm::mat3x3;
    using mat3x4 = glm::mat3x4;
    using mat4x2 = glm::mat4x2;
    using mat4x3 = glm::mat4x3;
    using mat4x4 = glm::mat4x4;

    namespace math
    {
        using namespace glm;
    }

    inline auto to_bytes(mat4 value) -> std::array<uint8_t, sizeof(mat4)>
    {
        std::array<uint8_t, sizeof(mat4)> data{};
        std::memcpy(data.data(), &value, sizeof(mat4));
        return data;
    }

    struct transform
    {
        static constexpr std::size_t bytes_in_transform = sizeof(vec3) + sizeof(quat) + sizeof(vec3);

        vec3 position = { 0.f, 0.f, 0.f };
        quat rotation = { 0.f, 0.f, 0.f, 1.f };
        vec3 scale = { 1.f, 1.f, 1.f };

        [[nodiscard]] inline constexpr auto right() const noexcept -> vec3
        {
            return rotation * vec3{ 1.f, 0.f, 0.f };
        }

        [[nodiscard]] inline constexpr auto forward() const noexcept -> vec3
        {
            return rotation * vec3{ 0.f, 0.f, 1.f };
        }

        [[nodiscard]] inline constexpr auto up() const noexcept -> vec3
        {
            return rotation * vec3{ 0.f, 1.f, 0.f };
        }

        [[nodiscard]] inline constexpr auto to_mat4() const noexcept -> mat4
        {
            return math::translate(mat4{ 1.f }, position) *
                   math::toMat4(rotation) *
                   math::scale(mat4{ 1.f }, scale);
        }

        auto to_bytes() const -> std::array<std::uint8_t, bytes_in_transform>
        {
            std::array<std::uint8_t, bytes_in_transform> data{};
            std::memcpy(data.data(), &position, sizeof(vec3));
            std::memcpy(data.data() + sizeof(vec3), &rotation, sizeof(quat));
            std::memcpy(data.data() + sizeof(vec3) + sizeof(quat), &scale, sizeof(vec3));
            return data;
        }
    };
}
