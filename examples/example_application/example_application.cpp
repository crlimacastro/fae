#include "fae/fae.hpp"
#include "fae/editor.hpp"
#include "fae/main.hpp"
#include "fae/math.hpp"

struct rotate
{
    float speed = 1.f;
    fae::vec3 axis = { 0.0f, 1.0f, 0.0f };
};

auto start(const fae::start_step& step) noexcept -> void
{
    step.global_entity.use_component<fae::ui_settings>([&](fae::ui_settings& ui_settings)
        { ui_settings.hide_ui = true; });
    fae::hide_cursor();

    auto camera_entity = step.ecs_world.create_entity();
    camera_entity
        .set_component<fae::name>(fae::name{ "camera" })
        .set_component<fae::transform>(fae::transform{
            .rotation = fae::math::angleAxis(fae::math::radians(180.f), fae::vec3(0.0f, 1.0f, 0.0f)) * fae::math::quat{ 0.f, 0.f, 0.f, 1.f },
        })
        .set_component<fae::camera>(fae::camera{});
    step.global_entity.set_component<fae::active_camera>(fae::active_camera{
        .camera_entity = camera_entity.id,
    });

    step.ecs_world.create_entity()
        .set_component<fae::name>(fae::name{ "ambient light" })
        .set_component<fae::ambient_light>(fae::ambient_light{
            .color = fae::color{ 200, 200, 200 },
        });

    step.ecs_world.create_entity()
        .set_component<fae::name>(fae::name{ "directional light" })
        .set_component<fae::directional_light>(fae::directional_light{
            .direction = fae::math::normalize(fae::vec3{ 1.f, -1.f, 1.f }),
            .color = fae::colors::white,
        });

    step.ecs_world.create_entity()
        .set_component<fae::name>(fae::name{ "floor" })
        .set_component<fae::transform>(fae::transform{
            .position = { 0.f, -2.f, 0.f },
            .rotation = fae::math::angleAxis(fae::math::radians(180.f), fae::vec3(0.0f, 0.0f, 1.0f)),
            .scale = fae::vec3(100.f, 0.5f, 100.f),
        })
        .set_component<fae::model>(fae::model{
            .mesh = fae::meshes::cube(),
            .material = fae::material{
                .diffuse = *step.assets.load<fae::texture>("rock.png"),
            },
        });

    step.ecs_world.create_entity()
        .set_component<fae::name>(fae::name{ "cube" })
        .set_component<fae::transform>(fae::transform{
            .position = { 0.f, 0.f, -10.f },
            .rotation = fae::math::angleAxis(fae::math::radians(0.f), fae::vec3(1.0f, 0.0f, 0.0f)) * fae::quat{ 0.f, 0.f, 0.f, 1.f },
            .scale = fae::vec3{ 1.f, 1.f, 1.f },
        })
        .set_component<fae::model>(fae::model{
            .mesh = *step.assets.load<fae::mesh>("cube.obj"),
            .material = fae::material{
                .diffuse = *step.assets.load<fae::texture>("wood.png"),
            },
        })
        .set_component<rotate>(rotate{ .speed = 60.f });

    // step.ecs_world.create_entity()
    //     .set_component<fae::name>(fae::name{ "model" })
    //     .set_component<fae::transform>(fae::transform{
    //         .position = { 17.f, 0.f, -23.f },
    //         .rotation = fae::math::angleAxis(fae::math::radians(-90.f), fae::vec3(1.0f, 0.0f, 0.0f)) * fae::quat{ 0.f, 0.f, 0.f, 1.f },
    //         .scale = fae::vec3{ 1.f, 1.f, 1.f } * 0.03f,
    //     })
    //     .set_component<fae::model>(fae::model{
    //         .mesh = *step.assets.load<fae::mesh>("Stanford_Bunny.stl"),
    //     });
}

auto hue_shift_clear_color(const fae::update_step& step) noexcept -> void
{
    auto& time = step.global_entity.get_or_set_component<fae::time>(fae::time{});
    step.global_entity.use_component<fae::renderer>(
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
    auto& imgui_io = fae::ui::GetIO();
    if (imgui_io.WantCaptureMouse)
    {
        return;
    }

    static auto had_focus = true;
    auto has_focus = false;
    step.global_entity.use_component<fae::primary_window>(
        [&](fae::primary_window& primary_window)
        {
            auto window_entity = step.ecs_world.get_entity(primary_window.window_entity);
            if (!window_entity.valid())
                return;
            auto maybe_window = window_entity.get_component<fae::window>();
            auto& window = *maybe_window;
            has_focus = window.is_focused();
        });

    if (has_focus && !had_focus)
    {
        auto input = step.global_entity.get_or_set_component<fae::input>(fae::input{});
        input.get_mouse_delta();
        had_focus = has_focus;
        return;
    }
    had_focus = has_focus;

    if (!has_focus)
    {
        return;
    }

    static bool enabled = true;
    auto input = step.global_entity.get_or_set_component<fae::input>(fae::input{});
    if (input.is_key_just_pressed(fae::key::lalt) || input.is_key_just_pressed(fae::key::grave))
    {
        enabled = !enabled;
        if (enabled)
        {
            fae::hide_cursor();
            input.get_mouse_delta();
            step.global_entity.use_component<fae::ui_settings>([&](fae::ui_settings& ui_settings)
                { ui_settings.hide_ui = true; });
        }
        else
        {
            fae::show_cursor();
            step.global_entity.use_component<fae::ui_settings>([&](fae::ui_settings& ui_settings)
                { ui_settings.hide_ui = false; });
        }
    }
    if (!enabled)
        return;

    auto& time = step.global_entity.get_or_set_component<fae::time>(fae::time{});
    step.global_entity.use_component<fae::active_camera>(
        [&](fae::active_camera& active_camera)
        {
            auto camera_entity = step.ecs_world.get_entity(active_camera.camera_entity);
            if (!camera_entity.valid())
                return;
                auto maybe_camera_transform = camera_entity.get_component<fae::transform>();
            auto& camera_transform = *maybe_camera_transform;

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
            auto mouse_delta = input.get_mouse_delta();
            auto look_input = fae::vec2(mouse_delta.x, mouse_delta.y);
            auto look_speed = 0.25f;
            auto look_delta = -look_input * look_speed * time.delta().seconds_f32();
            look_angle_radians += look_delta;
            look_angle_radians.y = std::clamp(look_angle_radians.y, fae::math::radians(-45.f), fae::math::radians(45.f));

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

auto lock_mouse(const fae::update_step& step) noexcept -> void
{
    bool is_ui_hidden = false;
    step.global_entity.use_component<fae::ui_settings>(
        [&](fae::ui_settings& ui_settings)
        {
            is_ui_hidden = ui_settings.hide_ui;
        });
    if (!is_ui_hidden)
    {
        return;
    }

    step.global_entity.use_component<fae::primary_window>(
        [&](fae::primary_window& primary_window)
        {
            auto window_entity = step.ecs_world.get_entity(primary_window.window_entity);
            auto maybe_window = window_entity.get_component<fae::window>();
            auto& window = *maybe_window;

            static auto was_focused = false;
            auto is_focused = window.is_focused();
            if (is_focused && !was_focused)
            {
                fae::hide_cursor();
            }
            else if (!is_focused && was_focused)
            {
                fae::show_cursor();
            }

            was_focused = is_focused;

            if (!is_focused)
                return;
            auto window_position = window.get_position();
            auto window_size = window.get_size();

            auto input = step.global_entity.get_or_set_component<fae::input>(fae::input{});

            auto global_mouse_position = input.get_global_mouse_position();
            if (global_mouse_position.x < window_position.x)
            {
                input.set_local_mouse_position(window_entity, window_size.width, global_mouse_position.y);
            }
            if (global_mouse_position.x > window_position.x + window_size.width)
            {
                input.set_local_mouse_position(window_entity, 0, global_mouse_position.y);
            }
            if (global_mouse_position.y < window_position.y)
            {
                input.set_local_mouse_position(window_entity, global_mouse_position.x, window_size.height);
            }
            if (global_mouse_position.y > window_position.y + window_size.height)
            {
                input.set_local_mouse_position(window_entity, global_mouse_position.x, 0);
            }
        });
}

auto rotate_system(const fae::update_step& step) noexcept -> void
{
    auto& time = step.global_entity.get_or_set_component<fae::time>(fae::time{});
    for (auto& [entity, transform, rotate] : step.ecs_world.query<fae::transform, const rotate>())
    {
        transform.rotation *= fae::math::angleAxis(fae::math::radians(rotate.speed) * time.delta(), rotate.axis);
    }
}

auto update(const fae::update_step& step) noexcept -> void
{
}

auto render(const fae::render_step& step) noexcept -> void
{
}

auto ui(const fae::ui_render_step& step) noexcept -> void
{
    static float f = 0.0f;
    static int counter = 0;
    static bool show_demo_window = false;
    static bool show_another_window = false;

    fae::ui::Begin("Hello, world!"); // Create a window called "Hello, world!" and append into it.

    fae::ui::Text("This is some useful text.");          // Display some text (you can use a format strings too)
    fae::ui::Checkbox("Demo Window", &show_demo_window); // Edit bools storing our window open/close state
    if (show_demo_window)
    {
        fae::ui::ShowDemoWindow(&show_demo_window);
    }
    fae::ui::Checkbox("Another Window", &show_another_window);
    if (show_another_window)
    {
        fae::ui::Begin("Another Window", &show_another_window);
        fae::ui::Text("Hello from another window!");
        if (fae::ui::Button("Close Me"))
            show_another_window = false;
        fae::ui::End();
    }

    if (fae::ui::Button("Button")) // Buttons return true when clicked (most widgets return true when edited/activated)
        counter++;
    fae::ui::SameLine();
    fae::ui::Text("counter = %d", counter);

    ImGuiIO& io = fae::ui::GetIO();
    fae::ui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
    fae::ui::End();
}

auto main(int argc, char* argv[]) -> int
{
    fae::application{}
        .add_plugin(fae::default_plugins{})
        .add_plugin(fae::editor_plugin{})
        .add_system<fae::start_step>(start)
        .add_system<fae::update_step>(fae::quit_on_esc)
        // .add_system<fae::update_step>(hue_shift_clear_color)
        .add_system<fae::update_step>(fps_control_active_camera)
        .add_system<fae::update_step>(lock_mouse)
        .add_system<fae::update_step>(rotate_system)
        .add_system<fae::update_step>(update)
        .add_system<fae::render_step>(render)
        .add_system<fae::ui_render_step>(ui)
        .run();
    return fae::exit_success;
}
