#include "fae/editor.hpp"

#include <string>

#include "fae/application/application.hpp"
#include "fae/ui.hpp"
#include "fae/logging.hpp"
#include "fae/rendering/rendering.hpp"
#include "fae/lighting.hpp"

namespace fae
{
    auto editor_plugin::init(application& app) const noexcept -> void
    {
        app
            .set_global_component(editor{
                .selected_entity = entt::null,
            })
            .add_system<ui_render_step>(ui_render_editor)
            .add_system<editor_render_step>(ui_render_entity_scene)
            .add_system<editor_render_step>(ui_render_entity_inspector)
            .add_system<editor_render_step>(ui_render_resources);
    }

    auto ui_render_editor(const ui_render_step& step) noexcept -> void
    {
        fae::ui::DockSpaceOverViewport(0U, nullptr, ImGuiDockNodeFlags_PassthruCentralNode);
        step.scheduler.invoke(editor_render_step{
            .global_entity = step.global_entity,
            .assets = step.assets,
            .scheduler = step.scheduler,
            .ecs_world = step.ecs_world,
        });
    }

    auto ui_render_entity_scene(const editor_render_step& step) noexcept -> void
    {
        fae::ui::Begin("Scene");

        if (ImGui::IsKeyPressed(ImGuiKey_Delete))
        {
            step.global_entity.use_component<editor>([&](editor& editor)
                { 
                    auto entity = fae::entity_commands
                    {
                        .id = editor.selected_entity,
                        .registry = step.ecs_world.registry,
                    };
                    if (!entity.valid())
                        return;
                        entity.destroy(); });
        }

        for (auto& entity : step.ecs_world.entities())
        {
            auto name = std::to_string(static_cast<std::uint32_t>(entity.id));
            entity.use_component<fae::name>([&](fae::name& name_component)
                { name = name_component.value; });
            name += " (" + std::to_string(static_cast<std::uint32_t>(entity.id)) + ")";
            if (fae::ui::TreeNodeEx(name.c_str(), ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen))
            {
                if (fae::ui::IsItemClicked())
                {
                    step.global_entity.use_component<editor>([&](editor& editor)
                        { editor.selected_entity = entity.id; });
                }
            }
        }
        fae::ui::End();
    }

    auto ui_render_entity_inspector(const editor_render_step& step) noexcept -> void
    {
        step.global_entity.use_component<editor>([&](editor& editor)
            {
                    fae::ui::Begin("Inspector");
                    static entt::entity prev_selected_entity = entt::null;
                if (editor.selected_entity != entt::null)
                {
                    auto entity = fae::entity_commands{
                        .id = editor.selected_entity,
                        .registry = step.ecs_world.registry,
                    };
                    auto final_name = std::to_string(static_cast<std::uint32_t>(entity.id));
                    entity.use_component<fae::name>([&](fae::name& name)
                        {
                            final_name = name.value;
                        });
                    if (fae::ui::InputText("##", &final_name))
                    {
                        if (prev_selected_entity == editor.selected_entity)
                        {
                        entity.set_component<fae::name>(fae::name{ .value = final_name });
                        }
                    }
                    prev_selected_entity = editor.selected_entity;

                    entity.use_component<transform>([&](transform& transform)
                        {
                            if (fae::ui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
                            {
                                fae::ui::DragFloat3("Position", fae::math::value_ptr(transform.position), 0.01f);
                                auto euler = fae::math::eulerAngles(transform.rotation);
                                fae::ui::DragFloat3("Rotation", fae::math::value_ptr(euler), 0.01f);
                                transform.rotation = fae::math::quat(euler);
                                fae::ui::DragFloat3("Scale", fae::math::value_ptr(transform.scale), 0.01f);
                            }
                        });

                        entity.use_component<ambient_light>([&](ambient_light& light)
                        {
                            if (fae::ui::CollapsingHeader("Ambient Light"), ImGuiTreeNodeFlags_DefaultOpen)
                            {
                            auto color = light.color.to_array();
                            fae::ui::ColorEdit4("Color", color.data());
                            light.color = fae::color::from_array(color);
                            }
                        });
                    
                    entity.use_component<directional_light>([&](directional_light& light)
                        {
                            if (fae::ui::CollapsingHeader("Directional Light"), ImGuiTreeNodeFlags_DefaultOpen)
                            {
                            fae::ui::DragFloat3("Direction", fae::math::value_ptr(light.direction), 1.f, -1.f, 1.f);
                            light.direction = fae::math::normalize(light.direction);
                            auto color = light.color.to_array();
                            fae::ui::ColorEdit4("Color", color.data());
                            light.color = fae::color::from_array(color);
                            }
                        });
                }
                fae::ui::End(); });
    }

    auto ui_render_resources(const editor_render_step& step) noexcept -> void
    {
        fae::ui::Begin("Resources");
        step.global_entity.use_component<fae::renderer>(
            [&](fae::renderer& renderer)
            {
                if (fae::ui::CollapsingHeader("Renderer", ImGuiTreeNodeFlags_DefaultOpen))
                {
                    auto clear_color = renderer.get_clear_color().to_array();
                    fae::ui::ColorEdit3("Clear Color", clear_color.data());
                    renderer.set_clear_color(fae::color::from_array(clear_color));
                }
            });
        fae::ui::End();
    }
}
