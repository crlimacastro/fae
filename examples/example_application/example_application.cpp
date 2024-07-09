#include <format>
#include <type_traits>

#include "fae/fae.hpp"
#include "fae/main.hpp"
#include "fae/math.hpp"

struct rotate
{
    float speed = 60.f;
};

auto start(const fae::start_step& step) noexcept -> void
{
    auto camera_entity = step.ecs_world.create_entity();
    camera_entity
        .set_component<fae::transform>(fae::transform{
            .rotation = fae::math::angleAxis(fae::math::radians(180.f), fae::vec3(0.0f, 1.0f, 0.0f)) * fae::math::quat{ 0.f, 0.f, 0.f, 1.f },
        })
        .set_component<fae::camera>(fae::camera{});
    step.resources.insert<fae::active_camera>(fae::active_camera{
        .camera_entity = camera_entity,
    });

    step.ecs_world.create_entity()
        .set_component<fae::transform>(fae::transform{
            .position = { 0.f, -2.f, 0.f },
            .rotation = fae::math::angleAxis(fae::math::radians(180.f), fae::vec3(0.0f, 0.0f, 1.0f)),
            .scale = fae::vec3(100.f, 0.5f, 100.f),
        })
        .set_component<fae::model>(fae::model{
            .mesh = fae::meshes::cube(),
        });

    step.ecs_world.create_entity()
        .set_component<fae::transform>(fae::transform{
            .position = { 0.f, -2.f, -10.f },
            .rotation = fae::quat{ 0.f, 0.f, 0.f, 1.f } * fae::math::angleAxis(fae::math::radians(90.f), fae::vec3(1.0f, 0.0f, 0.0f)),
            .scale = fae::vec3{ 1.f, 1.f, 1.f } * .03f,
        })
        .set_component<fae::model>(fae::model{
            .mesh = *step.assets.load<fae::mesh>("Stanford_Bunny.stl"),
        })
        .set_component<rotate>(rotate{});
}

auto hue_shift_clear_color(const fae::update_step& step) noexcept -> void
{
    auto& time = step.resources.get_or_emplace<fae::time>(fae::time{});
    step.resources.use_resource<fae::renderer>(
        [&](fae::renderer& renderer)
        {
            const auto speed = 200.f;
            const auto delta_h = speed * time.delta().seconds_f32();
            auto clear_color_hsva = renderer.get_clear_color().to_hsva();
            clear_color_hsva.h = static_cast<int>((clear_color_hsva.h + delta_h)) % 360;
            auto new_clear_color = clear_color_hsva.to_rgba();
            renderer.set_clear_color(new_clear_color);
        });
}

auto fps_control_active_camera(const fae::update_step& step) noexcept -> void
{
    auto& time = step.resources.get_or_emplace<fae::time>(fae::time{});
    step.resources.use_resource<fae::active_camera>(
        [&](fae::active_camera& active_camera)
        {
            auto& camera_transform = active_camera.transform();
            auto input = step.resources.get_or_emplace<fae::input>(fae::input{});

            auto move_input = fae::vec3(0.0f, 0.0f, 0.0f);

            if (input.is_key_pressed(fae::key::w))
            {
                move_input.z += 1;
            }
            if (input.is_key_pressed(fae::key::s))
            {
                move_input.z += -1;
            }
            if (input.is_key_pressed(fae::key::a))
            {
                move_input.x += -1;
            }
            if (input.is_key_pressed(fae::key::d))
            {
                move_input.x += 1;
            }
            if (input.is_key_pressed(fae::key::lshift))
            {
                move_input.y += -1;
            }
            if (input.is_key_pressed(fae::key::lctrl))
            {
                move_input.y += 1;
            }
            if (fae::math::length(move_input) > 0.0f)
            {
                move_input = fae::math::normalize(move_input);
            }

            auto move_speed = 20.0f;
            auto move_delta = move_speed * time.delta().seconds_f32();
            camera_transform.position += camera_transform.rotation * move_input * move_delta;

            static auto look_angle_radians = fae::vec2(0.f);
            auto look_input = input.get_mouse_delta();
            auto look_speed = 0.25f;
            auto look_delta = -look_input * look_speed * time.delta().seconds_f32();
            look_angle_radians += look_delta;
            look_angle_radians.y = std::clamp(look_angle_radians.y, fae::math::radians(-60.f), fae::math::radians(60.f));

            camera_transform.rotation = fae::math::angleAxis(look_angle_radians.y, camera_transform.rotation * fae::vec3(1.f, 0.f, 0.f)) *
                                        fae::math::angleAxis(fae::math::radians(180.f) + look_angle_radians.x, fae::vec3(0.f, 1.f, 0.f)) *
                                        fae::quat(0.f, 0.f, 0.f, 1.f);

            if (input.is_key_just_pressed(fae::key::f))
            {
                camera_transform.position = { 0.f, 0.f, 0.f };
                look_angle_radians = { 0.f, 0.f };
            }
        });
}

auto update(const fae::update_step& step) noexcept -> void
{
    auto& time = step.resources.get_or_emplace<fae::time>(fae::time{});
    for (auto& [entity, transform, rotate] : step.ecs_world.query<fae::transform, const rotate>())
    {
        transform.rotation *= fae::math::angleAxis(fae::math::radians(rotate.speed) * time.delta(), fae::vec3(0.0f, 0.0f, 1.0f));
    }
}

auto render(const fae::render_step& step) noexcept -> void
{
}

auto fae_main(int argc, char* argv[]) -> int
{
    fae::application{}
        .add_plugin(fae::default_plugins{})
        .add_system<fae::start_step>(start)
        .add_system<fae::update_step>(fae::quit_on_esc)
        // .add_system<fae::update_step>(hue_shift_clear_color)
        .add_system<fae::update_step>(fps_control_active_camera)
        .add_system<fae::update_step>(update)
        .add_system<fae::render_step>(render)
        .run();
    return fae::exit_success;
}
