#include "fae/rendering/webgpu_renderer.hpp"

#include <cstdint>

#include "fae/rendering/renderer.hpp"
#include "fae/resource_manager.hpp"
#include "fae/math.hpp"
#include "fae/time.hpp"
#include "fae/webgpu.hpp"
#include "fae/color.hpp"
#include "fae/rendering/renderer.hpp"
#include "fae/rendering/mesh.hpp"

namespace fae
{
    [[nodiscard]] auto
    make_webgpu_renderer(resource_manager& resources) noexcept -> renderer
    {
        return renderer{
            .get_clear_color =
                [&]()
            {
                auto clear_color = colors::black;
                resources.use_resource<fae::webgpu>(
                    [&](webgpu& webgpu)
                    {
                        clear_color = color{
                            .r = static_cast<std::uint8_t>(
                                webgpu.clear_color.r * 255.0f),
                            .g = static_cast<std::uint8_t>(
                                webgpu.clear_color.g * 255.0f),
                            .b = static_cast<std::uint8_t>(
                                webgpu.clear_color.b * 255.0f),
                            .a = static_cast<std::uint8_t>(
                                webgpu.clear_color.a * 255.0f),
                        };
                    });
                return clear_color;
            },
            .set_clear_color =
                [&](color value)
            {
                resources.use_resource<fae::webgpu>(
                    [&](webgpu& webgpu)
                    {
                        webgpu.clear_color = wgpu::Color{
                            .r = value.r / 255.0f,
                            .g = value.g / 255.0f,
                            .b = value.b / 255.0f,
                            .a = value.a / 255.0f,
                        };
                    });
            },
            .clear =
                [&]()
            {
                // TODO
            },
            .begin =
                [&]()
            {
                resources.use_resource<fae::webgpu>(
                    [&](webgpu& webgpu)
                    {
                        webgpu.current_render.render_commands.clear();

                        wgpu::SurfaceTexture surface_texture;
                        webgpu.surface.GetCurrentTexture(&surface_texture);
                        auto surface_texture_view = surface_texture.texture.CreateView();

                        auto command_encoder = webgpu.device.CreateCommandEncoder();
                        auto color_attachment = wgpu::RenderPassColorAttachment{
                            .view = surface_texture_view,
                            .depthSlice = WGPU_DEPTH_SLICE_UNDEFINED,
                            .resolveTarget = nullptr,
                            .loadOp = wgpu::LoadOp::Clear,
                            .storeOp = wgpu::StoreOp::Store,
                            .clearValue = webgpu.clear_color,
                        };
                        auto depth_attachment = wgpu::RenderPassDepthStencilAttachment{
                            .view = webgpu.render_pipeline.depth_texture.CreateView(),
                            .depthLoadOp = wgpu::LoadOp::Clear,
                            .depthStoreOp = wgpu::StoreOp::Store,
                            .depthClearValue = 1.0,
                            .stencilReadOnly = true,
                        };
                        auto render_pass_desc = wgpu::RenderPassDescriptor{
                            .colorAttachmentCount = 1,
                            .colorAttachments = &color_attachment,
                            .depthStencilAttachment = &depth_attachment,
                        };
                        auto render_pass = command_encoder.BeginRenderPass(&render_pass_desc);
                        render_pass.SetPipeline(webgpu.render_pipeline.pipeline);
                        webgpu.current_render.command_encoder = command_encoder;
                        webgpu.current_render.render_pass = render_pass;
                    });
            },
            .end =
                [&]()
            {
                resources.use_resource<fae::webgpu>(
                    [&](webgpu& webgpu)
                    {
                        std::vector<t_uniforms> uniforms;
                        for (auto& render_command : webgpu.current_render.render_commands)
                        {
                            uniforms.push_back(render_command.uniform_data);
                        }
                        auto sizeof_uniforms = uniforms.size() * webgpu.render_pipeline.uniform_stride;
                        auto uniform_buffer = create_buffer(webgpu.device, "fae_uniform_buffer", sizeof_uniforms, wgpu::BufferUsage::Uniform);
                        auto bind_entries = std::vector<wgpu::BindGroupEntry>{
                            wgpu::BindGroupEntry{
                                .binding = 0,
                                .buffer = uniform_buffer,
                                .size = sizeof(t_uniforms),
                            },
                        };

                        std::uint32_t uniform_offset = 0;
                        for (auto & uniform : uniforms)
                        {
                            webgpu.device.GetQueue().WriteBuffer(uniform_buffer, uniform_offset, &uniform, sizeof(t_uniforms));
                            uniform_offset += webgpu.render_pipeline.uniform_stride;
                        }

                        auto bind_group_descriptor = wgpu::BindGroupDescriptor{
                            .layout = webgpu.render_pipeline.pipeline.GetBindGroupLayout(0),
                            .entryCount = static_cast<std::size_t>(bind_entries.size()),
                            .entries = bind_entries.data(),
                        };
                        auto uniform_bind_group = webgpu.device.CreateBindGroup(&bind_group_descriptor);

                        uniform_offset = 0;
                        for (auto& render_command : webgpu.current_render.render_commands)
                        {
                            webgpu.current_render.render_pass.SetBindGroup(0, uniform_bind_group, 1, &uniform_offset);
                            uniform_offset += webgpu.render_pipeline.uniform_stride;

                            const auto vertex_buffer = create_buffer_with_data(
                                webgpu.device, "indexed_render_data_vertex_buffer", render_command.vertex_data.data(), sizeof_data(render_command.vertex_data),
                                wgpu::BufferUsage::Vertex);
                            webgpu.current_render.render_pass.SetVertexBuffer(0, vertex_buffer);
                            if (!render_command.index_data.empty())
                            {
                                const auto index_buffer = create_buffer_with_data(
                                    webgpu.device, "indexed_render_data_index_buffer", render_command.index_data.data(), sizeof_data(render_command.index_data),
                                    wgpu::BufferUsage::Index);
                                webgpu.current_render.render_pass.SetIndexBuffer(index_buffer, wgpu::IndexFormat::Uint32);
                                webgpu.current_render.render_pass.DrawIndexed(render_command.index_data.size());
                            } else {
                                webgpu.current_render.render_pass.Draw(render_command.vertex_data.size());
                            }
                        }

                        webgpu.current_render.render_pass.End();
                        auto command_buffer = webgpu.current_render.command_encoder.Finish();
                        auto queue = webgpu.device.GetQueue();
                        queue.Submit(1, &command_buffer);
#ifndef FAE_PLATFORM_WEB
                        webgpu.surface.Present();
                        webgpu.instance.ProcessEvents();
#endif
                    });
            },
            .render_cube =
                [&](const renderer::draw_cube_args& args)
            {
                resources.use_resource<fae::webgpu>(
                    [&](webgpu& webgpu)
                    {
                        auto mesh = meshes::cube();
                        t_uniforms uniforms;
                        auto transform = fae::transform{};
                        transform.position = args.position;
                        transform.rotation = args.rotation;
                        transform.scale = args.scale;
                        uniforms.model = transform.to_mat4();
                        auto fov = 45.f;
                        auto aspect = 1920.f / 1080.f;
                        auto near_plane = 0.1f;
                        auto far_plane = 1000.f;
                        uniforms.projection = math::perspective(math::radians(fov), aspect, near_plane, far_plane);
                        uniforms.tint = args.tint.to_vec4();
                        auto time = resources.get_or_emplace<fae::time>(fae::time{});
                        auto t = time.elapsed().seconds_f32();
                        uniforms.time = t;
                        webgpu.current_render.render_commands.push_back(fae::webgpu::current_render::render_command{
                            .vertex_data = mesh.vertices,
                            .index_data = mesh.indices,
                            .uniform_data = uniforms,
                        });
                    });
            },
            .render_model = [&](const fae::model& model, const fae::transform& transform)
            {
                // TODO
            },
        };
    }
}
