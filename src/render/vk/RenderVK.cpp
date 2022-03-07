#include "common/Common.h"
#include "platform/Platform.h"

#include <vku/vku_framework.hpp>

#include <vector>
#include <stdexcept>

#include "platform/Window.h"
#include "render/Render.h"

#include "../CommandBuffer.h"

// TEMPORARY
#include "assets/models/importers/OBJ.h"

namespace engine::render
{
    // TEMPORARY
    using Vertex = Vertex_XYZ_RGB_UV;
    
    class ShaderVK final : public Shader
    {
    public:
        vku::ShaderModule vert;
        vku::ShaderModule frag;
        vk::UniquePipeline pipeline;
    };

    class RenderVK final : public Render
    {
        struct RenderState {
            uint width, height;
        } state;

        vku::Framework  fw;
        vk::Device      device;
        vku::Window     window;

        bool clearEnabled = true;
        bool clearDepthEnabled = true;
        Color clearColor = Colors.Black;
        float clearDepth = 1.0f;
        uint clearStencil = 0;

        vk::UniquePipelineLayout        pipelineLayout;
        vk::UniqueDescriptorSetLayout   descriptorSetLayout;
        std::vector<vk::DescriptorSet>  descriptorSets;

        vk::UniqueSemaphore         dynamicSemaphore;
        vk::UniqueCommandBuffer     cmd;
        vk::UniqueFence             cmdFence;

        uint32 framebufferIndex = 0;

        std::vector<ShaderVK> shaders;

        Matrix4x4 viewMatrix, projMatrix;

        // Temp stuff
        Shader*               shader;
        ModelOBJ              model;
        vku::HostVertexBuffer vbuffer;
        vku::HostIndexBuffer  ibuffer;

        vku::UniformBuffer ubuffer;
        struct Uniform {
            Matrix4x4 modelToPerspective;
            Matrix4x4 modelToWorld;
            Matrix4x4 normalToWorld;
            Matrix4x4 modelToLight;
            Vector4 cameraPos;
            Vector4 lightPos;
        };

/*
        const std::vector<Vertex> vertices = {
            {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
            {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
            {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
            {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
        };

        const std::vector<uint32> indices = {
            0, 1, 2, 2, 3, 0
        };
*/

    protected:
        void Init(Window* win)
        {
            fw = vku::Framework(win->GetTitle());
            if (!fw.ok())
                throw std::runtime_error("[VKU] Failed to create framework!");

            device = fw.device();

            // Attach to window surface
            {
                auto display = (Display*)win->GetNativeDisplay();
                auto x11win = (::Window)win->GetNativeWindow();
                auto info = vk::XlibSurfaceCreateInfoKHR{{}, display, x11win};
                auto surface = fw.instance().createXlibSurfaceKHR(info);
                window = vku::Window {fw.instance(), device, fw.physicalDevice(), fw.graphicsQueueFamilyIndex(), surface};
                if (!window.ok())
                    throw std::runtime_error("[VKU] Failed to attach to window.");
            }

            //window.dumpCaps(std::cout, fw.physicalDevice());

            dynamicSemaphore = device.createSemaphoreUnique(vk::SemaphoreCreateInfo {});

            vk::CommandBufferAllocateInfo cbInfo = {window.commandPool(), vk::CommandBufferLevel::ePrimary, 1};
            cmd = std::move(device.allocateCommandBuffersUnique(cbInfo).front());

            vk::FenceCreateInfo fenceInfo;
            fenceInfo.flags = vk::FenceCreateFlagBits::eSignaled;
            cmdFence = device.createFenceUnique(fenceInfo);

            /// PIPELINE ///

            vku::DescriptorSetLayoutMaker dslm;
            dslm.buffer(0, vk::DescriptorType::eUniformBuffer, vk::ShaderStageFlagBits::eVertex|vk::ShaderStageFlagBits::eFragment, 1);
            // images...
            descriptorSetLayout = dslm.createUnique(device);

            vku::DescriptorSetMaker dsm;
            dsm.layout(*descriptorSetLayout);
            descriptorSets = dsm.create(device, fw.descriptorPool());

            vku::PipelineLayoutMaker plm;
            plm.descriptorSetLayout(*descriptorSetLayout);
            pipelineLayout = plm.createUnique(device);

            // Temp stuff
            shader = LoadShader("teapot.vert", "teapot.frag");

            model = LoadModelOBJ("core/models/teapot.obj");
            vbuffer = vku::HostVertexBuffer(device, fw.memprops(), model.vertices);
            ibuffer = vku::HostIndexBuffer(device, fw.memprops(), model.indices);

            /// UNIFORMS (SHADER CONSTANTS) ///

            ubuffer = vku::UniformBuffer(fw.device(), fw.memprops(), sizeof(Uniform));


            vku::DescriptorSetUpdater update;
            update.beginDescriptorSet(descriptorSets[0]);

            // Initial uniforms' values
            update.beginBuffers(0, 0, vk::DescriptorType::eUniformBuffer);
            update.buffer(ubuffer.buffer(), 0, sizeof(Uniform));

            // images...

            update.update(device);

            // Temp: View Matrices
            {
                using namespace hlslpp;

                float3 at = float3(0, 0, 0);
                float3 eye = float3(0, 0, -35);
                float4x4 view = float4x4::look_at(eye, at, float3(0, 1, 0));

                frustum f = frustum::field_of_view_y(radians(float1(60.0f)), GetAspectRatio(), 0.1f, 100.f);
                float4x4 proj = float4x4::perspective(projection(f, zclip::zero));

                SetViewTransform(view, proj);
            }

            /*
            window.setStaticCommands([&, this](
                                        vk::CommandBuffer cb, int imageIndex,
                                        vk::RenderPassBeginInfo &rpbi) {
                static auto ww = window.width();
                static auto wh = window.height();
                if (ww != window.width() || wh != window.height()) {
                    ww = window.width();
                    wh = window.height();
                    //pipeline = buildPipeline();
                }
            });

            // Handle window resize
            window->SetResizeCallback([=, this](uint width, uint height) {
            });
            */
        }

        // TEMPORARY
        void _Setup()
        {
            Uniform uniform;
            uniform.modelToPerspective = viewMatrix * projMatrix;
            uniform.modelToWorld = Matrix4x4::identity();
            uniform.normalToWorld = Matrix4x4::identity();
            uniform.modelToLight = Matrix4x4::identity();
            uniform.cameraPos = Vector4(0.f);
            uniform.lightPos = Vector4(0.f);

            cmd->updateBuffer(ubuffer.buffer(), 0, sizeof(Uniform), (void*)&uniform);
        }

        // TODO: TEMP Remove this
        void _DoFrame()
        {
            SetShader(shader);
            cmd->bindVertexBuffers(0, vbuffer.buffer(), vk::DeviceSize(0));
            cmd->bindIndexBuffer(ibuffer.buffer(), 0, vk::IndexType::eUint32);
            cmd->bindDescriptorSets(vk::PipelineBindPoint::eGraphics, *pipelineLayout, 0, descriptorSets, nullptr);
            cmd->drawIndexed(model.indices.size(), 1, 0, 0, 0);
            //cmd->draw(3, 1, 0, 0);
        }

        void BeginFrame()
        {
            /*static auto start = std::chrono::high_resolution_clock::now();
            auto time = std::chrono::high_resolution_clock::now();
            auto delta = time - start;
            start = time;
            // uncomment to get frame time.
            //std::cout << std::chrono::duration_cast<std::chrono::microseconds>(delta).count() << "us frame time\n";*/

            auto umax = std::numeric_limits<uint64_t>::max();
            auto acquired = device.acquireNextImageKHR(window.swapchain(), umax, window.imageAcquireSemaphore(), vk::Fence(), &framebufferIndex);
            if (acquired != vk::Result::eSuccess) {
                window.recreate();
                return;
            }

            device.waitForFences(*cmdFence, 1, umax);
            device.resetFences(*cmdFence);

            // Clear the screen
            std::vector<vk::ClearValue> clearColors;

            if (clearEnabled) {
                vk::ClearColorValue clearColorValue;
                std::copy_n(clearColor.data, 4, clearColorValue.float32.begin());
                clearColors.push_back(clearColorValue);
            }
            if (clearDepthEnabled) {
                clearColors.push_back(vk::ClearDepthStencilValue {clearDepth, clearStencil});
            }

            vk::RenderPassBeginInfo rpbi;
            rpbi.renderPass = window.renderPass();
            rpbi.framebuffer = *window.framebuffers()[framebufferIndex];
            rpbi.renderArea = vk::Rect2D{{0, 0}, {window.width(), window.height()}};
            rpbi.clearValueCount = uint32(clearColors.size());
            rpbi.pClearValues = clearColors.data();
            
            // This clears the command buffer too
            cmd->begin(vk::CommandBufferBeginInfo {});

            // TODO: Some kind of BeforeFrame method
            _Setup();
            
            // Single pass for now
            cmd->beginRenderPass(rpbi, vk::SubpassContents::eInline);
        }

        void EndFrame()
        {
            auto umax = std::numeric_limits<uint64_t>::max();

            _DoFrame();

            // End pass from BeginFrame
            cmd->endRenderPass();

            // Finalize command buffer
            cmd->end();

            // Submit command buffer
            vk::Queue graphicsQueue     = fw.graphicsQueue();
            vk::PipelineStageFlags waitStages = vk::PipelineStageFlagBits::eColorAttachmentOutput;
            vk::Semaphore iaSema        = window.imageAcquireSemaphore();
            vk::Semaphore ccSema        = window.commandCompleteSemaphore();
            vk::SubmitInfo submit;
            submit.waitSemaphoreCount   = 1;
            submit.pWaitSemaphores      = &iaSema;
            submit.pWaitDstStageMask    = &waitStages;
            submit.commandBufferCount   = 1;
            submit.pCommandBuffers      = &*cmd;
            submit.signalSemaphoreCount = 1;
            submit.pSignalSemaphores    = &*dynamicSemaphore;
            graphicsQueue.submit(1, &submit, *cmdFence);

            // Submit static command buffers
            vk::Fence cbFence = window.commandBufferFences()[framebufferIndex];
            device.waitForFences(cbFence, 1, umax);
            device.resetFences(cbFence);
            submit.waitSemaphoreCount   = 1;
            submit.pWaitSemaphores      = &*dynamicSemaphore;
            submit.pWaitDstStageMask    = &waitStages;
            submit.commandBufferCount   = 1;
            submit.pCommandBuffers      = &*window.commandBuffers()[framebufferIndex];
            submit.signalSemaphoreCount = 1;
            submit.pSignalSemaphores    = &ccSema;
            graphicsQueue.submit(1, &submit, cbFence);

            // Present!
            vk::PresentInfoKHR presentInfo;
            vk::SwapchainKHR swapchain = window.swapchain();
            presentInfo.pSwapchains = &swapchain;
            presentInfo.swapchainCount = 1;
            presentInfo.pImageIndices = &framebufferIndex;
            presentInfo.waitSemaphoreCount = 1;
            presentInfo.pWaitSemaphores = &ccSema;
            try {
                window.presentQueue().presentKHR(presentInfo);
            } catch (const vk::OutOfDateKHRError) {
                window.recreate();
            }

            // Very crude method to prevent your GPU from overheating.
            std::this_thread::sleep_for(std::chrono::milliseconds(16));
        }

        void Shutdown()
        {
            device.waitIdle();
        }

    public:

        Shader* LoadShader(const char* vertexShader, const char* pixelShader)
        {
            std::string path = "./core/shaders/spirv/";
            
            ShaderVK& shader = shaders.emplace_back();
            shader.vert = vku::ShaderModule(device, path + vertexShader + ".spv");
            shader.frag = vku::ShaderModule(device, path + pixelShader + ".spv");
            shader.pipeline = CreatePipeline(&shader);

            return &shader;
        }

        void SetShader(Shader* shader)
        {
            ShaderVK* shadervk = static_cast<ShaderVK*>(shader);

            cmd->bindPipeline(vk::PipelineBindPoint::eGraphics, *shadervk->pipeline);
        }

        void Submit()
        {
            
        }

        float GetAspectRatio()
        {
            return float(state.width) / float(state.height);
        }

        void SetViewTransform(hlslpp::float4x4& view, hlslpp::float4x4& proj)
        {
            viewMatrix = view;
            projMatrix = proj;
        }

        void SetTransform(hlslpp::float4x4& matrix)
        {
            //bgfx::setTransform(&matrix._m00);
        }

        void SetClearColor(bool clear, Color color)
        {
            clearEnabled = clear;
            clearColor   = color;
        }

        void SetClearDepth(bool clear, float depth)
        {
            clearDepthEnabled = clear;
            clearDepth        = depth;
        }

    private:

        vk::UniquePipeline CreatePipeline(ShaderVK* shader)
        {
            // Make a pipeline to use the vertex format and shaders.
            vku::PipelineMaker pm{window.width(), window.height()};
            pm.shader(vk::ShaderStageFlagBits::eVertex, shader->vert);
            pm.shader(vk::ShaderStageFlagBits::eFragment, shader->frag);
            
            pm.vertexBinding(0, (uint32_t)sizeof(Vertex));
            pm.vertexAttribute(0, 0, vk::Format::eR32G32B32Sfloat,
                                (uint32_t)offsetof(Vertex, pos));
            pm.vertexAttribute(1, 0, vk::Format::eR32G32B32Sfloat,
                                (uint32_t)offsetof(Vertex, color));
            pm.vertexAttribute(2, 0, vk::Format::eR32G32Sfloat,
                                (uint32_t)offsetof(Vertex, uv));

            // Create a pipeline using a renderPass built for our window.
            auto renderPass = window.renderPass();
            auto cache = fw.pipelineCache();

            return pm.createUnique(device, cache, *pipelineLayout, renderPass);
        }
    };
    
    Render* Render::Create()
    {
        return new RenderVK();
    }

}