#pragma once

#include <cstdint>
#include <format>
#include <functional>
#include <numbers>
#include <optional>
#include <string_view>
#include <variant>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>

#include "fae/application.hpp"
#include "fae/color.hpp"
#include "fae/core.hpp"
#include "fae/logging.hpp"
#include "fae/math.hpp"
#include "fae/resource_manager.hpp"
#include "fae/sdl.hpp"
#include "fae/time.hpp"
#include "fae/webgpu.hpp"
#include "fae/windowing.hpp"

namespace fae
{
	struct renderer
	{
		std::function<color()> get_clear_color;
		std::function<void(color)> set_clear_color;
		std::function<void()> clear;
		std::function<void()> begin;
		std::function<void()> end;
	};

	struct render_step
	{
		resource_manager &resources;
		scheduler &scheduler;
		ecs_world &ecs_world;
	};

	auto update_rendering(const update_step &step) noexcept -> void
	{
		static bool first_render_happened = false;
		step.resources.use_resource<fae::renderer>(
			[&](fae::renderer &renderer)
			{
				renderer.begin();
				renderer.clear();
				step.scheduler.invoke<render_step>(render_step{
					.resources = step.resources,
					.scheduler = step.scheduler,
					.ecs_world = step.ecs_world,
				});
				renderer.end();
				if (!first_render_happened)
				{
					step.scheduler.invoke(fae::first_render_end{
						.resources = step.resources,
						.scheduler = step.scheduler,
						.ecs_world = step.ecs_world,
					});
					first_render_happened = true;
				}
			});
	}

	/* incomplete/deprecated for now */
	[[nodiscard]] auto make_sdl_renderer(resource_manager &resources) noexcept
		-> renderer
	{
		return renderer{
			.get_clear_color = [&]() -> color
			{
				auto clear_color = colors::black;
				resources.use_resource<sdl_renderer>(
					[&](sdl_renderer &renderer)
					{
						std::uint8_t r{}, g{}, b{}, a{};
						SDL_GetRenderDrawColor(renderer.raw.get(), &r, &g, &b, &a);
						clear_color = color{r, g, b, a};
					});
				return clear_color;
			},
			.set_clear_color =
				[&](color value)
			{
				resources.use_resource<sdl_renderer>(
					[&](sdl_renderer &renderer)
					{
						SDL_SetRenderDrawColor(renderer.raw.get(), value.r, value.g,
							value.b, value.a);
					});
			},
			.clear =
				[&]()
			{
				resources.use_resource<sdl_renderer>(
					[&](sdl_renderer &renderer)
					{ SDL_RenderClear(renderer.raw.get()); });
			},
			.begin =
				[&]()
			{
				// do nothing
			},
			.end =
				[&]()
			{
				resources.use_resource<sdl_renderer>(
					[&](sdl_renderer &renderer)
					{ SDL_RenderPresent(renderer.raw.get()); });
			},
		};
	}

	[[nodiscard]] auto
	make_webgpu_renderer(resource_manager &resources) noexcept -> renderer
	{
		return renderer{
			.get_clear_color =
				[&]()
			{
				auto clear_color = colors::black;
				resources.use_resource<fae::webgpu>(
					[&](webgpu &webgpu)
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
					[&](webgpu &webgpu)
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
					[&](webgpu &webgpu)
					{
						webgpu.current_render.vertex_data.clear();
						webgpu.current_render.index_data.clear();
						webgpu.current_render.uniform_data.clear();
						webgpu.current_render.uniform_data.resize((3 * 4 * 16) + (1 * 4 * 4));

						wgpu::SurfaceTexture surface_texture;
						webgpu.surface.GetCurrentTexture(&surface_texture);
						auto surface_texture_view = surface_texture.texture.CreateView();

						auto fov = 45.f;
						auto aspect = 1920.f / 1080.f;
						auto near_plane = 0.1f;
						auto far_plane = 1000.f;

						static auto transform = fae::transform{};

						auto time = resources.get_or_emplace<fae::time>(fae::time{});
						auto dt = time.delta().seconds_f32();
						auto t = time.elapsed().seconds_f32();

						transform.position = {1.2f * std::cos(t), 1.2f * std::sinf(t * 2), -5.f};
						transform.rotation *= math::angleAxis(math::radians(60.f) * dt, vec3(0.0f, 1.0f, 0.0f));
						auto model = transform.to_mat4();
						auto view = mat4(1.f);
						auto projection = math::perspective(math::radians(fov), aspect, near_plane, far_plane);
						static auto hsva = color_hsva::from_rgba(colors::red);
						hsva.h = static_cast<float>(static_cast<int>(hsva.h + dt * 120) % 360);
						auto tint = hsva.to_rgba().to_vec4f();
						std::memcpy(webgpu.current_render.uniform_data.data(), &model, sizeof(model));
						std::memcpy(webgpu.current_render.uniform_data.data() + sizeof(model), &view, sizeof(view));
						std::memcpy(webgpu.current_render.uniform_data.data() + sizeof(model) + sizeof(view), &projection, sizeof(projection));
						std::memcpy(webgpu.current_render.uniform_data.data() + sizeof(model) + sizeof(view) + sizeof(projection), &tint, sizeof(tint));

						webgpu.device.GetQueue().WriteBuffer(webgpu.uniform_buffer.buffer, 0, webgpu.current_render.uniform_data.data(), webgpu.uniform_buffer.size);
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
							.view = webgpu.depth_texture.CreateView(),
							.depthLoadOp = wgpu::LoadOp::Clear,
							.depthStoreOp = wgpu::StoreOp::Store,
							.depthClearValue = 1.0,
						};
						auto render_pass_desc = wgpu::RenderPassDescriptor{
							.colorAttachmentCount = 1,
							.colorAttachments = &color_attachment,
							.depthStencilAttachment = &depth_attachment,
						};
						auto render_pass = command_encoder.BeginRenderPass(&render_pass_desc);
						render_pass.SetPipeline(webgpu.render_pipeline);
						render_pass.SetBindGroup(0, webgpu.uniform_buffer.bind_group);
						webgpu.current_render.render_pass = render_pass;
						webgpu.current_render.command_encoder = command_encoder;
					});
			},
			.end =
				[&]()
			{
				resources.use_resource<fae::webgpu>(
					[&](webgpu &webgpu)
					{
						const auto vertex_buffer = create_buffer_with_data(
							webgpu.device, "vertex_buffer", webgpu.current_render.vertex_data.data(), sizeof_data(webgpu.current_render.vertex_data),
							wgpu::BufferUsage::Vertex);
						webgpu.current_render.render_pass.SetVertexBuffer(0, vertex_buffer);
						const auto index_buffer = create_buffer_with_data(
							webgpu.device, "index_buffer", webgpu.current_render.index_data.data(), sizeof_data(webgpu.current_render.index_data),
							wgpu::BufferUsage::Index);
						webgpu.current_render.render_pass.SetIndexBuffer(index_buffer, wgpu::IndexFormat::Uint32);
						webgpu.current_render.render_pass.DrawIndexed(webgpu.current_render.index_data.size());
						webgpu.current_render.render_pass.End();
						auto command_buffer = webgpu.current_render.command_encoder.Finish();
						auto queue = webgpu.device.GetQueue();
						queue.Submit(1, &command_buffer);
						webgpu.surface.Present();
						webgpu.instance.ProcessEvents();
					});
			},
		};
	}

	struct rendering_plugin
	{
		struct webgpu_renderer_options
		{
		};

		std::variant<sdl_renderer::options, webgpu_renderer_options>
			renderer_options = webgpu_renderer_options{};

		auto init(application &app) const noexcept -> void
		{
			app.add_plugin(windowing_plugin{});

			fae::match(
				renderer_options,
				[&](sdl_renderer::options options)
				{
					app.add_plugin(sdl_plugin{});
					auto maybe_primary = app.resources.get<primary_window>();
					if (!maybe_primary)
					{
						fae::log_error("no primary window found");
						return;
					}
					auto &primary = *maybe_primary;
					auto maybe_sdl_window =
						primary.window_entity.get_component<fae::sdl_window>();
					if (!maybe_sdl_window)
					{
						fae::log_error("primary window is not an sdl_window");
						return;
					}
					auto &sdl_window = *maybe_sdl_window;

					auto maybe_sdl_renderer = sdl_renderer::create(sdl_window, options);
					if (!maybe_sdl_renderer)
					{
						const auto &error = maybe_sdl_renderer.error();
						fae::log_error(std::format("could not create renderer: {}", error.message));
						return;
					}
					auto &sdl_renderer = *maybe_sdl_renderer;
					app.emplace_resource<fae::sdl_renderer>(std::move(sdl_renderer));
					app.insert_resource<renderer>(std::move(make_sdl_renderer(app.resources)));
				},
				[&](webgpu_renderer_options options)
				{
					app.add_plugin(webgpu_plugin{});
					const auto maybe_webgpu_renderer =
						app.resources.get<webgpu>();
					if (!maybe_webgpu_renderer)
					{
						fae::log_error("webgpu renderer not found");
						return;
					}
					auto webgpu_renderer = *maybe_webgpu_renderer;
					app.emplace_resource<renderer>(
						make_webgpu_renderer(app.resources));
				},
				[&]([[maybe_unused]] auto other)
				{ fae::log_error("unknown renderer type"); });

			app.add_system<update_step>(update_rendering);
		}
	};
} // namespace fae