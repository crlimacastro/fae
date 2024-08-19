#pragma once

#include <array>
#include <any>
#include <memory>

#include <webgpu/webgpu_cpp.h>

#include "fae/core/enum.hpp"

#include "fae/logging.hpp"
#include "fae/math.hpp"
#include "fae/windowing.hpp"

#include "fae/webgpu/sdl_impl.hpp"
#include "fae/webgpu/string_utils.hpp"
#include "fae/webgpu/utils.hpp"

#include "fae/rendering/mesh.hpp"
#include "fae/rendering/texture.hpp"
#include "fae/rendering/render_pipeline.hpp"

namespace fae
{
    struct application;

    struct webgpu
    {
        wgpu::Instance instance;
        wgpu::Adapter adapter;
        wgpu::Device device;
        wgpu::Surface surface;

        wgpu::Color clear_color = { 0, 0, 0, 1 };

        wgpu::TextureFormat depth_texture_format = wgpu::TextureFormat::Depth24Plus;

        struct render_pipeline
        {
            wgpu::ShaderModule shader_module;
            wgpu::RenderPipeline render_pipeline;
            wgpu::Texture depth_texture;
            std::uint32_t uniform_stride;
        };
        std::vector<render_pipeline> render_pipelines;

        struct render_pass
        {
            std::size_t render_pipeline_id;
            wgpu::CommandEncoder command_encoder;
            wgpu::RenderPassEncoder render_pass_encoder;

            struct render_command
            {
                std::vector<vertex> vertex_data;
                std::vector<std::uint32_t> index_data;
                std::vector<std::uint8_t> uniform_data;
                wgpu::TextureView texture_view;
                wgpu::Sampler sampler;
            };
            std::vector<render_command> render_commands;
        };
        std::vector<render_pass> render_passes;
    };

    struct webgpu_plugin
    {
        wgpu::RequestAdapterOptions adapter_options{};
        wgpu::DeviceDescriptor device_descriptor{
#ifndef FAE_PLATFORM_WEB
            .deviceLostCallbackInfo = wgpu::DeviceLostCallbackInfo{
                .mode = wgpu::CallbackMode::AllowSpontaneous,
                .callback = [](WGPUDevice const* device, WGPUDeviceLostReason cReason, char const* message, void* userdata)
                {
                    auto reason = static_cast<wgpu::DeviceLostReason>(cReason);
                    switch (reason)
                    {
                    case wgpu::DeviceLostReason::Destroyed:
                        // don't log this reason to not scare the user, happens naturally at the end of the application lifecycle
                        break;
                    case wgpu::DeviceLostReason::Unknown:
                    case wgpu::DeviceLostReason::InstanceDropped:
                    case wgpu::DeviceLostReason::FailedCreation:
                        fae::log_info(std::format("[wgpu] Device lost. [reason] {} [message] {}", to_string(reason), message));
                        break;
                    }
                },
            }
#endif
        };
#ifndef FAE_PLATFORM_WEB
        wgpu::LoggingCallback logging_callback = [](WGPULoggingType cType, char const* message, void* userdata)
        {
            auto type = static_cast<wgpu::LoggingType>(cType);
            fae::log_info(std::format("[wgpu] Info [type] {} [message] {}", to_string(type), message));
        };
#endif
        wgpu::ErrorCallback error_callback = [](WGPUErrorType cType, const char* message, void* userdata)
        {
            auto type = static_cast<wgpu::ErrorType>(cType);
            fae::log_error(std::format("[wgpu] Error [type] {} [message] {}", to_string(type), message));
        };

        auto init(application& app) const noexcept -> void;
    };

    auto reconfigure_on_window_resized(const fae::window_resized& e) noexcept -> void;
}
