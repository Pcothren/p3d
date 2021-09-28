#include "mvRenderer.h"
#include <stdexcept>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>

namespace p3d {

    namespace Renderer
    {
        static void mvSetupImGui(GLFWwindow* window)
        {

            // Setup Dear ImGui context
            IMGUI_CHECKVERSION();
            ImGui::CreateContext();
            ImGuiIO& io = ImGui::GetIO(); (void)io;
            io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls

            // Setup Dear ImGui style
            ImGui::StyleColorsDark();

            // Setup Platform/Renderer backends
            ImGui_ImplGlfw_InitForVulkan(window, true);
            ImGui_ImplVulkan_InitInfo init_info = {};
            init_info.Instance = mvGetVkInstance();
            init_info.PhysicalDevice = mvGetPhysicalDevice();
            init_info.Device = mvGetLogicalDevice();
            init_info.QueueFamily = GContext->graphics.graphicsQueueFamily;
            init_info.Queue = GContext->graphics.graphicsQueue;
            init_info.PipelineCache = nullptr;
            init_info.DescriptorPool = GContext->graphics.descriptorPool;
            init_info.Allocator = nullptr;
            init_info.MinImageCount = GContext->graphics.minImageCount;
            init_info.ImageCount = GContext->graphics.minImageCount;
            init_info.CheckVkResultFn = nullptr;
            ImGui_ImplVulkan_Init(&init_info, GContext->graphics.renderPass);

            // Upload Fonts
            {
                // Use any command queue
                VkCommandBuffer command_buffer = mvBeginSingleTimeCommands();
                ImGui_ImplVulkan_CreateFontsTexture(command_buffer);
                mvEndSingleTimeCommands(command_buffer);
                ImGui_ImplVulkan_DestroyFontUploadObjects();
            }

        }

        static void mvCleanupImGui()
        {
            ImGui_ImplVulkan_Shutdown();
            ImGui_ImplGlfw_Shutdown();
            ImGui::DestroyContext();
        }

        static void mvResizeImGui()
        {
            vkDeviceWaitIdle(mvGetLogicalDevice());
            ImGui_ImplVulkan_SetMinImageCount(GContext->graphics.minImageCount);
        }

        static void mvBeginImGuiFrame()
        {
            ImGui_ImplVulkan_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
        }

        static void mvEndImGuiFrame()
        {
            ImGui::Render();

            ImDrawData* main_draw_data = ImGui::GetDrawData();

            // Record dear imgui primitives into command buffer
            ImGui_ImplVulkan_RenderDrawData(main_draw_data, mvGetCurrentCommandBuffer());
        }

        void mvResize()
        {
            mvRecreateSwapChain();
            mvResizeImGui();
        }

        void mvStartRenderer()
        {
            mvInitializeViewport(500, 500);
            GContext->IO.shaderDirectory = "../../P3D_Vulkan/shaders/";
            GContext->graphics.enableValidationLayers = true;
            GContext->graphics.validationLayers = { "VK_LAYER_KHRONOS_validation" };
            GContext->graphics.deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
            mvSetupGraphicsContext();
            mvSetupImGui(GContext->viewport.handle);
        }

        void mvStopRenderer()
        {
            mvCleanupImGui();
            mvCleanupGraphicsContext();
        }

        void mvBeginFrame()
        {
            vkWaitForFences(mvGetLogicalDevice(), 1, &GContext->graphics.inFlightFences[GContext->graphics.currentFrame], VK_TRUE, UINT64_MAX);

            VkResult result = vkAcquireNextImageKHR(mvGetLogicalDevice(), GContext->graphics.swapChain, UINT64_MAX, GContext->graphics.imageAvailableSemaphores[GContext->graphics.currentFrame],
                VK_NULL_HANDLE, &GContext->graphics.currentImageIndex);

            if (GContext->graphics.imagesInFlight[GContext->graphics.currentImageIndex] != VK_NULL_HANDLE)
                vkWaitForFences(mvGetLogicalDevice(), 1, &GContext->graphics.imagesInFlight[GContext->graphics.currentImageIndex], VK_TRUE, UINT64_MAX);

            GContext->graphics.imagesInFlight[GContext->graphics.currentImageIndex] = GContext->graphics.inFlightFences[GContext->graphics.currentFrame];
        }

        void mvEndFrame()
        {
            VkSubmitInfo submitInfo{};
            submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

            VkSemaphore waitSemaphores[] = { GContext->graphics.imageAvailableSemaphores[GContext->graphics.currentFrame] };
            VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
            submitInfo.waitSemaphoreCount = 1;
            submitInfo.pWaitSemaphores = waitSemaphores;
            submitInfo.pWaitDstStageMask = waitStages;
            submitInfo.commandBufferCount = 1;
            submitInfo.pCommandBuffers = &GContext->graphics.commandBuffers[GContext->graphics.currentImageIndex];

            VkSemaphore signalSemaphores[] = { GContext->graphics.renderFinishedSemaphores[GContext->graphics.currentFrame] };
            submitInfo.signalSemaphoreCount = 1;
            submitInfo.pSignalSemaphores = signalSemaphores;

            vkResetFences(mvGetLogicalDevice(), 1, &GContext->graphics.inFlightFences[GContext->graphics.currentFrame]);

            if (vkQueueSubmit(GContext->graphics.graphicsQueue, 1, &submitInfo, GContext->graphics.inFlightFences[GContext->graphics.currentFrame]) != VK_SUCCESS)
                throw std::runtime_error("failed to submit draw command buffer!");
        }

        void mvPresent()
        {

            VkSemaphore signalSemaphores[] = { GContext->graphics.renderFinishedSemaphores[GContext->graphics.currentFrame] };

            VkPresentInfoKHR presentInfo{};
            presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
            presentInfo.waitSemaphoreCount = 1;
            presentInfo.pWaitSemaphores = signalSemaphores;

            VkSwapchainKHR swapChains[] = { GContext->graphics.swapChain };
            presentInfo.swapchainCount = 1;
            presentInfo.pSwapchains = swapChains;
            presentInfo.pImageIndices = &GContext->graphics.currentImageIndex;

            VkResult result = vkQueuePresentKHR(GContext->graphics.presentQueue, &presentInfo);

            GContext->graphics.currentFrame = (GContext->graphics.currentFrame + 1) % MV_MAX_FRAMES_IN_FLIGHT;
        }

        void mvRenderMesh(const mvMesh& drawable, mvMaterial& material, glm::mat4 accumulatedTransform, glm::mat4 camera, glm::mat4 projection)
        {
            static VkDeviceSize offsets = { 0 };

            uint32_t material_uniform_offset = 0u;
            vkCmdBindDescriptorSets(mvGetCurrentCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, material.pipeline.pipelineLayout, 0, 1, &material.descriptorSets[GContext->graphics.currentImageIndex], 0, &material_uniform_offset);
            vkCmdBindIndexBuffer(mvGetCurrentCommandBuffer(), drawable.indexBuffer.buffer, 0, VK_INDEX_TYPE_UINT16);
            vkCmdBindVertexBuffers(mvGetCurrentCommandBuffer(), 0, 1, &drawable.vertexBuffer.buffer, &offsets);
            vkCmdBindPipeline(mvGetCurrentCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, material.pipeline.pipeline);

            glm::mat4 localTransform = glm::translate(glm::vec3{ drawable.pos.x, drawable.pos.y, drawable.pos.z }) *
                glm::rotate(drawable.rot.x, glm::vec3{ 1.0f, 0.0f, 0.0f }) *
                glm::rotate(drawable.rot.y, glm::vec3{ 0.0f, 1.0f, 0.0f }) *
                glm::rotate(drawable.rot.z, glm::vec3{ 0.0f, 0.0f, 1.0f });

            mvTransforms transforms;
            transforms.model = accumulatedTransform * localTransform;
            transforms.modelView = camera * transforms.model;
            transforms.modelViewProjection = projection * transforms.modelView;

            vkCmdPushConstants(
                GContext->graphics.commandBuffers[GContext->graphics.currentImageIndex],
                material.pipeline.pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(mvTransforms), &transforms);

            mvDraw(drawable.indexBuffer.count);
        }

        void mvBeginPass(VkCommandBuffer commandBuffer, VkRenderPass renderPass)
        {
            VkCommandBufferBeginInfo beginInfo{};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

            if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
                throw std::runtime_error("failed to begin recording command buffer!");

            VkRenderPassBeginInfo renderPassInfo{};
            renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassInfo.renderPass = renderPass;
            renderPassInfo.framebuffer = GContext->graphics.swapChainFramebuffers[GContext->graphics.currentImageIndex];
            renderPassInfo.renderArea.offset = { 0, 0 };
            renderPassInfo.renderArea.extent = GContext->graphics.swapChainExtent;

            VkClearValue clearValues[2];
            clearValues[0].color = { {0.0f, 0.0f, 0.0f, 1.0f} };
            clearValues[1].depthStencil = { 1.0f, 0 };

            renderPassInfo.clearValueCount = 2;
            renderPassInfo.pClearValues = clearValues;

            vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

            mvBeginImGuiFrame();
        }

        void mvEndPass(VkCommandBuffer commandBuffer)
        {
            mvEndImGuiFrame();

            vkCmdEndRenderPass(commandBuffer);

            if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
                throw std::runtime_error("failed to record command buffer!");
        }
    }
}