
module;
#include <functional>
#include <typeindex>
#include <unordered_map>
#include <any>
#include <optional>

export module fae:resource_manager;

import :core;

export namespace fae
{
    struct resource_manager
    {
        template <typename t_resource>
        [[nodiscard]] inline constexpr auto insert_and_get(t_resource &&resource = {}) noexcept -> t_resource &
        {
            const auto key = std::type_index(typeid(t_resource));
            m_resources.insert(std::make_pair(key, std::forward<t_resource &&>(resource)));
            return std::any_cast<t_resource &>(m_resources.at(key));
        }

        template <typename t_resource>
        [[maybe_unused]] inline constexpr auto insert(t_resource &&resource = {}) noexcept -> resource_manager &
        {
            [[maybe_unused]] const auto &res = insert_and_get(std::forward<t_resource &&>(resource));
            return *this;
        }

        template <typename t_resource, typename... t_args>
        [[nodiscard]] inline constexpr auto emplace_and_get(t_args &&...args) noexcept -> t_resource &
        {
            const auto key = std::type_index(typeid(t_resource));
            if (!m_resources.contains(key))
            {
                m_resources.emplace(key, std::forward<t_args &&>(args)...);
            }
            else
            {
                m_resources.insert(std::make_pair(key, std::move(t_resource{std::forward<t_args &&>(args)...})));
            }
            return std::any_cast<t_resource &>(m_resources[key]);
        }

        template <typename t_resource, typename... t_args>
        [[maybe_unused]] inline constexpr auto emplace(t_args &&...args) noexcept -> resource_manager &
        {
            [[maybe_unused]] const auto &res = emplace_and_get<const t_resource>(std::forward<t_args &&>(args)...);
            return *this;
        }

        template <typename t_resource>
        [[nodiscard]] inline constexpr auto get() noexcept -> optional_reference<t_resource>
        {
            const auto key = std::type_index(typeid(t_resource));
            if (!m_resources.contains(key))
            {
                return std::nullopt;
            }
            return optional_reference<t_resource>(std::any_cast<t_resource &>(m_resources.at(key)));
        }

        template <typename t_resource>
        [[nodiscard]] inline constexpr auto get() const noexcept -> optional_reference<const t_resource>
        {
            const auto key = std::type_index(typeid(t_resource));
            if (!m_resources.contains(key))
            {
                return std::nullopt;
            }
            return optional_reference<const t_resource>(std::any_cast<const t_resource &>(m_resources.at(key)));
        }

        template <typename t_resource>
        [[nodiscard]] inline constexpr auto get_or_default(t_resource &default_resource = {}) noexcept -> t_resource &
        {
            auto maybe_resource = get<t_resource>();
            if (!maybe_resource)
            {
                return default_resource;
            }
            return *maybe_resource;
        }

        template <typename t_resource>
        [[nodiscard]] inline constexpr auto get_or_default(const t_resource &default_resource = {}) const noexcept -> const t_resource &
        {
            const auto maybe_resource = get<const t_resource>();
            if (!maybe_resource)
            {
                return default_resource;
            }
            return *maybe_resource;
        }

        template <typename t_resource>
        [[nodiscard]] inline constexpr auto get_or_insert(t_resource &&default_resource = {}) noexcept -> t_resource &
        {
            auto maybe_resource = get<t_resource>();
            if (!maybe_resource)
            {
                return insert_and_get(std::forward<t_resource &&>(default_resource));
            }
            return *maybe_resource;
        }

        template <typename t_resource, typename... t_args>
        [[nodiscard]] inline constexpr auto get_or_emplace(t_args &&...args) noexcept -> t_resource &
        {
            auto maybe_resource = get<t_resource>();
            if (!maybe_resource)
            {
                return emplace_and_get<t_resource>(std::forward<t_args &&>(args)...);
            }
            return *maybe_resource;
        }

        template <typename t_resource>
        [[maybe_unused]] inline constexpr auto erase() noexcept -> resource_manager &
        {
            const auto key = std::type_index(typeid(t_resource));
            if (m_resources.contains(key))
            {
                m_resources.erase(key);
            }
            return *this;
        }

        [[maybe_unused]] inline auto clear() noexcept -> resource_manager &
        {
            m_resources.clear();
            return *this;
        }

        template <typename t_resource>
        [[maybe_unused]] inline constexpr auto use_resource(std::function<void(t_resource &)> callback) noexcept -> resource_manager &
        {
            const auto key = std::type_index(typeid(t_resource));
            if (m_resources.contains(key))
            {
                callback(std::any_cast<t_resource &>(m_resources.at(key)));
            }
            return *this;
        }

        template <typename t_resource>
        [[maybe_unused]] inline constexpr auto use_resource(std::function<void(const t_resource &)> callback) const noexcept -> const resource_manager &
        {
            const auto key = std::type_index(typeid(t_resource));
            if (m_resources.contains(key))
            {
                callback(std::any_cast<const t_resource &>(m_resources.at(key)));
            }
            return *this;
        }

    private:
        std::unordered_map<std::type_index, std::any> m_resources{};
    };
}
