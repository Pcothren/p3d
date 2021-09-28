#include "mvGraphics.h"
#include <stdexcept>
#include <iostream>
#include <set>
#include <optional>
#include <array>
#include "mvContext.h"

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, 
    VkDebugUtilsMessageTypeFlagsEXT messageType, 
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData) {
    std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
    return VK_FALSE;
}

namespace DearPy3D {

    //-----------------------------------------------------------------------------
    // forward declarations
    //-----------------------------------------------------------------------------

    struct SwapChainSupportDetails;
    struct QueueFamilyIndices;

    static SwapChainSupportDetails mvQuerySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);
    static bool                    mvCheckValidationLayerSupport(const std::vector<const char*> layers);
    static bool                    mvIsDeviceSuitable(VkPhysicalDevice device, VkPhysicalDeviceProperties& properties, std::vector<const char*> deviceExtensions);
    static bool                    mvCheckDeviceExtensionSupport(VkPhysicalDevice device, std::vector<const char*> deviceExtensions);
    
    // initialization
    static void mvSetupDebugMessenger();
    static void mvCreateVulkanInstance();
    static void mvCreateSurface();
    static void mvCreatePhysicalDevice();
    static void mvCreateLogicalDevice();
    static void mvCreateSwapChain();
    static void mvCreateMainCommandPool();
    static void mvCreateMainDescriptorPool();
    static void mvCreateMainRenderPass();
    static void mvCreateMainDepthResources();
    static void mvCreateFrameBuffers();
    static void mvCreateSyncObjects();

    //-----------------------------------------------------------------------------
    // internals
    //-----------------------------------------------------------------------------

    struct SwapChainSupportDetails
    {
        VkSurfaceCapabilitiesKHR        capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR>   presentModes;
    };

    struct QueueFamilyIndices
    {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;

        bool isComplete() 
        {
            return graphicsFamily.has_value() && presentFamily.has_value();
        }
    };

    static SwapChainSupportDetails mvQuerySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface)
    {
        SwapChainSupportDetails details;

        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

        uint32_t formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

        // todo: put in appropriate spot
        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, 0, surface, &presentSupport);

        if (formatCount != 0) {
            details.formats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
        }

        uint32_t presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

        if (presentModeCount != 0) {
            details.presentModes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
        }

        return details;
    }

    static bool mvCheckValidationLayerSupport(const std::vector<const char*> layers)
    {
        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        for (const char* layerName : layers) {
            bool layerFound = false;

            for (const auto& layerProperties : availableLayers) {
                if (strcmp(layerName, layerProperties.layerName) == 0) {
                    layerFound = true;
                    break;
                }
            }

            if (!layerFound) {
                return false;
            }
        }

        return true;
    }

    static QueueFamilyIndices mvFindQueueFamilies(VkPhysicalDevice device)
    {

        QueueFamilyIndices indices;

        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

        int i = 0;
        for (const auto& queueFamily : queueFamilies) 
        {
            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
                indices.graphicsFamily = i;

            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, GContext->graphics.surface, &presentSupport);

            if (presentSupport)
                indices.presentFamily = i;

            if (indices.isComplete())
                break;

            i++;
        }

        return indices;
    }

    static bool mvIsDeviceSuitable(VkPhysicalDevice device, VkPhysicalDeviceProperties& properties, std::vector<const char*> deviceExtensions)
    {
        QueueFamilyIndices indices = mvFindQueueFamilies(device);

        bool extensionsSupported = mvCheckDeviceExtensionSupport(device, deviceExtensions);

        bool swapChainAdequate = false;
        if (extensionsSupported)
        {
            swapChainAdequate = true;
            //SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device);
            //swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
        }

        vkGetPhysicalDeviceProperties(device, &properties);

        std::cout << "The GPU has a minimum buffer alignment of " << properties.limits.minUniformBufferOffsetAlignment << std::endl;
        std::cout << "The GPU has a max push constants size of " << properties.limits.maxPushConstantsSize << std::endl;

        return extensionsSupported && swapChainAdequate && properties.limits.maxPushConstantsSize >= 256;
    }

    static bool mvCheckDeviceExtensionSupport(VkPhysicalDevice device, std::vector<const char*> deviceExtensions)
    {
        uint32_t extensionCount;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

        std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

        for (const auto& extension : availableExtensions)
            requiredExtensions.erase(extension.extensionName);

        return requiredExtensions.empty();
    }

    static void mvCreateVulkanInstance()
    {

        if (GContext->graphics.enableValidationLayers && !mvCheckValidationLayerSupport(GContext->graphics.validationLayers))
            throw std::runtime_error("validation layers requested, but not available!");

        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;

        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        // get extensions required to load glfw
        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
        std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
        if (GContext->graphics.enableValidationLayers)
            extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

        createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
        createInfo.ppEnabledExtensionNames = extensions.data();

        // Setup debug messenger for vulkan instance
        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo = {};
        if (GContext->graphics.enableValidationLayers)
        {
            createInfo.enabledLayerCount = static_cast<uint32_t>(GContext->graphics.validationLayers.size());
            createInfo.ppEnabledLayerNames = GContext->graphics.validationLayers.data();
            createInfo.pNext = VK_NULL_HANDLE;

            debugCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
            debugCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
            debugCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
            debugCreateInfo.pfnUserCallback = debugCallback;
            debugCreateInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
        }
        else
        {
            createInfo.enabledLayerCount = 0;
            createInfo.pNext = VK_NULL_HANDLE;
        }

        // create the instance
        if (vkCreateInstance(&createInfo, nullptr, &GContext->graphics.instance) != VK_SUCCESS)
            throw std::runtime_error("failed to create instance!");
    }

    static void mvSetupDebugMessenger()
    {

        if (!GContext->graphics.enableValidationLayers)
            return;

        VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        createInfo.pfnUserCallback = debugCallback;
        createInfo.pNext = VK_NULL_HANDLE;

        auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(mvGetVkInstance(), "vkCreateDebugUtilsMessengerEXT");
        if (func != nullptr)
        {
            VkResult result = func(mvGetVkInstance(), &createInfo, nullptr, &GContext->graphics.debugMessenger);
            if (result != VK_SUCCESS)
                throw std::runtime_error("failed to set up debug messenger!");
        }
        else
            throw std::runtime_error("failed to set up debug messenger!");
    }

    static void mvCreateSurface()
    {
        if (glfwCreateWindowSurface(mvGetVkInstance(), GContext->viewport.handle, nullptr, &GContext->graphics.surface) != VK_SUCCESS)
            throw std::runtime_error("failed to create window surface!");
    }

    static void mvCreatePhysicalDevice()
    {
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(mvGetVkInstance(), &deviceCount, nullptr);

        if (deviceCount == 0)
            throw std::runtime_error("failed to find GPUs with Vulkan support!");

        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(mvGetVkInstance(), &deviceCount, devices.data());
        for (const auto& device : devices)
        {
            if (mvIsDeviceSuitable(device, GContext->graphics.deviceProperties, GContext->graphics.deviceExtensions))
            {
                GContext->graphics.physicalDevice = device;
                break;
            }
        }
        
        if (GContext->graphics.physicalDevice == VK_NULL_HANDLE)
            throw std::runtime_error("failed to find a suitable GPU!");
    }

    static void mvCreateLogicalDevice()
    {

        QueueFamilyIndices indices = mvFindQueueFamilies(mvGetPhysicalDevice());

        GContext->graphics.graphicsQueueFamily = indices.graphicsFamily.value();

        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

        float queuePriority = 1.0f;
        for (uint32_t queueFamily : uniqueQueueFamilies)
        {
            VkDeviceQueueCreateInfo queueCreateInfo{};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = queueFamily;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;
            queueCreateInfos.push_back(queueCreateInfo);
        }

        VkPhysicalDeviceFeatures deviceFeatures{};
        deviceFeatures.samplerAnisotropy = VK_TRUE;
        {
            VkDeviceCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

            createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
            createInfo.pQueueCreateInfos = queueCreateInfos.data();

            createInfo.pEnabledFeatures = &deviceFeatures;

            createInfo.enabledExtensionCount = GContext->graphics.deviceExtensions.size();
            createInfo.ppEnabledExtensionNames = GContext->graphics.deviceExtensions.data();

            if (GContext->graphics.enableValidationLayers)
            {
                createInfo.enabledLayerCount = static_cast<uint32_t>(GContext->graphics.validationLayers.size());
                createInfo.ppEnabledLayerNames = GContext->graphics.validationLayers.data();
            }
            else
                createInfo.enabledLayerCount = 0;

            auto code = vkCreateDevice(mvGetPhysicalDevice(), &createInfo, nullptr, &GContext->graphics.logicalDevice);
            if (code != VK_SUCCESS)
            {
                std::cout << code<< std::endl;
                throw std::runtime_error("failed to create logical device!");
            }
        }

        vkGetDeviceQueue(mvGetLogicalDevice(), indices.graphicsFamily.value(), 0, &GContext->graphics.graphicsQueue);
        vkGetDeviceQueue(mvGetLogicalDevice(), indices.presentFamily.value(), 0, &GContext->graphics.presentQueue);

    }

    static void mvCreateSwapChain()
    {
        SwapChainSupportDetails swapChainSupport = mvQuerySwapChainSupport(mvGetPhysicalDevice(), GContext->graphics.surface);

        // choose swap surface Format
        VkSurfaceFormatKHR surfaceFormat = swapChainSupport.formats[0];
        for (const auto& availableFormat : swapChainSupport.formats)
        {
            if (availableFormat.format == VK_FORMAT_R8G8B8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
            {
                surfaceFormat = availableFormat;
                break;
            }
        }

        // chose swap present mode
        VkPresentModeKHR presentMode = VK_PRESENT_MODE_FIFO_KHR;
        for (const auto& availablePresentMode : swapChainSupport.presentModes)
        {
            if (availablePresentMode == VK_PRESENT_MODE_FIFO_KHR)
            {
                presentMode = availablePresentMode;
                break;
            }
        }

        // chose swap extent
        VkExtent2D extent;
        if (swapChainSupport.capabilities.currentExtent.width != UINT32_MAX)
            extent = swapChainSupport.capabilities.currentExtent;
        else
        {
            VkExtent2D actualExtent = {
                static_cast<uint32_t>(GContext->viewport.width),
                static_cast<uint32_t>(GContext->viewport.height)
            };

            actualExtent.width = std::max(swapChainSupport.capabilities.minImageExtent.width, std::min(swapChainSupport.capabilities.maxImageExtent.width, actualExtent.width));
            actualExtent.height = std::max(swapChainSupport.capabilities.minImageExtent.height, std::min(swapChainSupport.capabilities.maxImageExtent.height, actualExtent.height));

            extent = actualExtent;
        }

        GContext->graphics.minImageCount = swapChainSupport.capabilities.minImageCount + 1;
        if (swapChainSupport.capabilities.maxImageCount > 0 && GContext->graphics.minImageCount > swapChainSupport.capabilities.maxImageCount)
            GContext->graphics.minImageCount = swapChainSupport.capabilities.maxImageCount;

        {
            VkSwapchainCreateInfoKHR createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
            createInfo.surface = GContext->graphics.surface;
            createInfo.minImageCount = GContext->graphics.minImageCount;
            createInfo.imageFormat = surfaceFormat.format;
            createInfo.imageColorSpace = surfaceFormat.colorSpace;
            createInfo.imageExtent = extent;
            createInfo.imageArrayLayers = 1;
            createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

            QueueFamilyIndices indices = mvFindQueueFamilies(mvGetPhysicalDevice());
            uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

            if (indices.graphicsFamily != indices.presentFamily)
            {
                createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
                createInfo.queueFamilyIndexCount = 2;
                createInfo.pQueueFamilyIndices = queueFamilyIndices;
            }
            else
                createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;

            createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
            createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
            createInfo.presentMode = presentMode;
            createInfo.clipped = VK_TRUE;

            createInfo.oldSwapchain = VK_NULL_HANDLE;

            if (vkCreateSwapchainKHR(mvGetLogicalDevice(), &createInfo, nullptr, &GContext->graphics.swapChain) != VK_SUCCESS)
                throw std::runtime_error("failed to create swap chain!");
        }

        vkGetSwapchainImagesKHR(mvGetLogicalDevice(), GContext->graphics.swapChain, &GContext->graphics.minImageCount, nullptr);
        GContext->graphics.swapChainImages.resize(GContext->graphics.minImageCount);
        vkGetSwapchainImagesKHR(mvGetLogicalDevice(), GContext->graphics.swapChain, &GContext->graphics.minImageCount, GContext->graphics.swapChainImages.data());

        GContext->graphics.swapChainImageFormat = surfaceFormat.format;
        GContext->graphics.swapChainExtent = extent;
        GContext->graphics.deletionQueue.pushDeletor([=]() { vkDestroySwapchainKHR(mvGetLogicalDevice(), GContext->graphics.swapChain, nullptr); });

        // creating image views
        GContext->graphics.swapChainImageViews.resize(GContext->graphics.swapChainImages.size());
        for (uint32_t i = 0; i < GContext->graphics.swapChainImages.size(); i++)
            GContext->graphics.swapChainImageViews[i] = mvCreateImageView(GContext->graphics.swapChainImages[i], GContext->graphics.swapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT);
    }

    static void mvCreateMainCommandPool()
    {
        QueueFamilyIndices queueFamilyIndices = mvFindQueueFamilies(mvGetPhysicalDevice());

        VkCommandPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();
        poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

        if (vkCreateCommandPool(mvGetLogicalDevice(), &poolInfo, nullptr, &GContext->graphics.commandPool) != VK_SUCCESS)
            throw std::runtime_error("failed to create command pool!");

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = GContext->graphics.commandPool;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = (uint32_t)(GContext->graphics.swapChainImages.size());

        GContext->graphics.commandBuffers.resize(GContext->graphics.swapChainImages.size());

        if (vkAllocateCommandBuffers(mvGetLogicalDevice(), &allocInfo, GContext->graphics.commandBuffers.data()) != VK_SUCCESS)
            throw std::runtime_error("failed to allocate command buffers!");

        GContext->graphics.deletionQueue.pushDeletor([=]() {vkDestroyCommandPool(mvGetLogicalDevice(), GContext->graphics.commandPool, nullptr); });
    }

    static void mvCreateMainDescriptorPool()
    {
        VkDescriptorPoolSize poolSizes[] =
        {
            { VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
            { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
            { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
            { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
        };
        VkDescriptorPoolCreateInfo poolInfo = {};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        poolInfo.maxSets = 1000 * 11;
        poolInfo.poolSizeCount = 11u;
        poolInfo.pPoolSizes = poolSizes;

        if (vkCreateDescriptorPool(mvGetLogicalDevice(), &poolInfo, nullptr, &GContext->graphics.descriptorPool) != VK_SUCCESS)
            throw std::runtime_error("failed to create descriptor pool!");
    }

    static void mvCreateMainRenderPass()
    {
        VkAttachmentDescription colorAttachment{};
        colorAttachment.format = GContext->graphics.swapChainImageFormat;
        colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        VkAttachmentReference colorAttachmentRef{};
        colorAttachmentRef.attachment = 0;
        colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkAttachmentDescription depthAttachment{};
        depthAttachment.format = VK_FORMAT_D32_SFLOAT;
        depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        VkAttachmentReference depthAttachmentRef{};
        depthAttachmentRef.attachment = 1;
        depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpass{};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &colorAttachmentRef;
        subpass.pDepthStencilAttachment = &depthAttachmentRef;

        VkSubpassDependency dependency{};
        dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        dependency.dstSubpass = 0;
        dependency.srcAccessMask = 0;
        dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

        std::array<VkAttachmentDescription, 2> attachments = { colorAttachment, depthAttachment };
        VkRenderPassCreateInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
        renderPassInfo.pAttachments = attachments.data();
        renderPassInfo.subpassCount = 1;
        renderPassInfo.pSubpasses = &subpass;
        renderPassInfo.dependencyCount = 1;
        renderPassInfo.pDependencies = &dependency;

        if (vkCreateRenderPass(mvGetLogicalDevice(), &renderPassInfo, nullptr, &GContext->graphics.renderPass) != VK_SUCCESS)
            throw std::runtime_error("failed to create render pass!");

        GContext->graphics.deletionQueue.pushDeletor([=]() {
            vkFreeCommandBuffers(mvGetLogicalDevice(),
                GContext->graphics.commandPool,
                static_cast<uint32_t>(GContext->graphics.commandBuffers.size()),
                GContext->graphics.commandBuffers.data());
            vkDestroyRenderPass(mvGetLogicalDevice(), GContext->graphics.renderPass, nullptr); }
        );
    }

    static void mvCreateMainDepthResources()
    {
        VkFormat depthFormat = VK_FORMAT_D32_SFLOAT;

        mvCreateImage(GContext->graphics.swapChainExtent.width, GContext->graphics.swapChainExtent.height, depthFormat,
            VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            GContext->graphics.depthImage, GContext->graphics.depthImageMemory);

        GContext->graphics.depthImageView = mvCreateImageView(GContext->graphics.depthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);

        GContext->graphics.deletionQueue.pushDeletor([=]() {
            vkDestroyImageView(mvGetLogicalDevice(), GContext->graphics.depthImageView, nullptr);
            vkDestroyImage(mvGetLogicalDevice(), GContext->graphics.depthImage, nullptr);
            vkFreeMemory(mvGetLogicalDevice(), GContext->graphics.depthImageMemory, nullptr);
            });
    }

    static void mvCreateFrameBuffers()
    {
        GContext->graphics.swapChainFramebuffers.resize(GContext->graphics.swapChainImageViews.size());

        for (size_t i = 0; i < GContext->graphics.swapChainImageViews.size(); i++)
        {
            VkImageView attachments[] = {
                GContext->graphics.swapChainImageViews[i],
                GContext->graphics.depthImageView
            };

            VkFramebufferCreateInfo framebufferInfo{};
            framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferInfo.renderPass = GContext->graphics.renderPass;
            framebufferInfo.attachmentCount = 2;
            framebufferInfo.pAttachments = attachments;
            framebufferInfo.width = GContext->graphics.swapChainExtent.width;
            framebufferInfo.height = GContext->graphics.swapChainExtent.height;
            framebufferInfo.layers = 1;

            if (vkCreateFramebuffer(mvGetLogicalDevice(), &framebufferInfo, nullptr, &GContext->graphics.swapChainFramebuffers[i]) != VK_SUCCESS)
                throw std::runtime_error("failed to create framebuffer!");

            GContext->graphics.deletionQueue.pushDeletor([=]() {
                vkDestroyFramebuffer(mvGetLogicalDevice(), GContext->graphics.swapChainFramebuffers[i], nullptr);
                vkDestroyImageView(mvGetLogicalDevice(), GContext->graphics.swapChainImageViews[i], nullptr);
                });
        }
    }

    static void mvCreateSyncObjects()
    {
        GContext->graphics.imageAvailableSemaphores.resize(MV_MAX_FRAMES_IN_FLIGHT);
        GContext->graphics.renderFinishedSemaphores.resize(MV_MAX_FRAMES_IN_FLIGHT);
        GContext->graphics.inFlightFences.resize(MV_MAX_FRAMES_IN_FLIGHT);
        GContext->graphics.imagesInFlight.resize(GContext->graphics.swapChainImages.size(), VK_NULL_HANDLE);

        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        for (size_t i = 0; i < MV_MAX_FRAMES_IN_FLIGHT; i++)
        {
            if (vkCreateSemaphore(mvGetLogicalDevice(), &semaphoreInfo, nullptr, &GContext->graphics.imageAvailableSemaphores[i]) != VK_SUCCESS ||
                vkCreateSemaphore(mvGetLogicalDevice(), &semaphoreInfo, nullptr, &GContext->graphics.renderFinishedSemaphores[i]) != VK_SUCCESS ||
                vkCreateFence(mvGetLogicalDevice(), &fenceInfo, nullptr, &GContext->graphics.inFlightFences[i]) != VK_SUCCESS)
                throw std::runtime_error("failed to create synchronization objects for a frame!");
        }
    }

    //-----------------------------------------------------------------------------
    // declarations for public API
    //-----------------------------------------------------------------------------

    VkInstance mvGetVkInstance()
    {
        return GContext->graphics.instance;
    }

    VkDevice mvGetLogicalDevice()
    {
        return GContext->graphics.logicalDevice;
    }

    VkPhysicalDevice mvGetPhysicalDevice()
    {
        return GContext->graphics.physicalDevice;
    }

    VkCommandBuffer  mvGetCurrentCommandBuffer()
    {
        return GContext->graphics.commandBuffers[GContext->graphics.currentImageIndex];
    }

    void mvSetupGraphicsContext()
    {
        mvCreateVulkanInstance();
        mvSetupDebugMessenger();
        mvCreateSurface();
        mvCreatePhysicalDevice();
        mvCreateLogicalDevice();
        mvInitializeAllocator();
        mvCreateSwapChain();
        mvCreateMainCommandPool();
        mvCreateMainDescriptorPool();
        mvCreateMainRenderPass();
        mvCreateMainDepthResources();
        mvCreateFrameBuffers();
        mvCreateSyncObjects();
    }

    void mvCreateImage(uint32_t width, uint32_t height, VkFormat format,
        VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties,
        VkImage& image, VkDeviceMemory& imageMemory)
    {
        VkImageCreateInfo imageInfo{};
        imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageInfo.imageType = VK_IMAGE_TYPE_2D;
        imageInfo.extent.width = width;
        imageInfo.extent.height = height;
        imageInfo.extent.depth = 1;
        imageInfo.mipLevels = 1;
        imageInfo.arrayLayers = 1;
        imageInfo.format = format;
        imageInfo.tiling = tiling;
        imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        imageInfo.usage = usage;
        imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
        imageInfo.flags = 0; // Optional

        if (vkCreateImage(mvGetLogicalDevice() , &imageInfo, nullptr, &image) != VK_SUCCESS)
            throw std::runtime_error("failed to create image!");

        VkMemoryRequirements memRequirements;
        vkGetImageMemoryRequirements(mvGetLogicalDevice(), image, &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = mvFindMemoryType(GContext->graphics.physicalDevice, memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

        if (vkAllocateMemory(mvGetLogicalDevice(), &allocInfo, nullptr, &imageMemory) != VK_SUCCESS)
            throw std::runtime_error("failed to allocate image memory!");

        vkBindImageMemory(mvGetLogicalDevice(), image, imageMemory, 0);
    }

    VkImageView mvCreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags)
    {
        VkImageViewCreateInfo viewInfo{};
        viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.image = image;
        viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        viewInfo.format = format;
        viewInfo.subresourceRange.baseMipLevel = 0;
        viewInfo.subresourceRange.levelCount = 1;
        viewInfo.subresourceRange.baseArrayLayer = 0;
        viewInfo.subresourceRange.layerCount = 1;
        viewInfo.subresourceRange.aspectMask = aspectFlags;

        VkImageView imageView;
        if (vkCreateImageView(mvGetLogicalDevice(), &viewInfo, nullptr, &imageView) != VK_SUCCESS)
            throw std::runtime_error("failed to create texture image view!");

        return imageView;
    }

    std::uint32_t mvFindMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties)
    {
        VkPhysicalDeviceMemoryProperties memProperties;
        vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

        for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
            if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) 
            {
                return i;
            }
        }

        throw std::runtime_error("failed to find suitable memory type!");
    }

    void mvCleanupGraphicsContext()
    {
        
        GContext->graphics.deletionQueue.flush();
        mvShutdownAllocator();
        vkDestroyDescriptorPool(GContext->graphics.logicalDevice, GContext->graphics.descriptorPool, nullptr);

        for (size_t i = 0; i < MV_MAX_FRAMES_IN_FLIGHT; i++)
        {
            vkDestroySemaphore(GContext->graphics.logicalDevice, GContext->graphics.imageAvailableSemaphores[i], nullptr);
            vkDestroySemaphore(GContext->graphics.logicalDevice, GContext->graphics.renderFinishedSemaphores[i], nullptr);
            vkDestroyFence(GContext->graphics.logicalDevice, GContext->graphics.inFlightFences[i], nullptr);
        }

        if (GContext->graphics.enableValidationLayers)
        {
            auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(mvGetVkInstance(), "vkDestroyDebugUtilsMessengerEXT");
            if (func != nullptr)
                func(mvGetVkInstance(), GContext->graphics.debugMessenger, nullptr);
        }

        vkDestroySurfaceKHR(mvGetVkInstance(), GContext->graphics.surface, nullptr);
        vkDestroyDevice(GContext->graphics.logicalDevice, nullptr);
        vkDestroyInstance(mvGetVkInstance(), nullptr);
    }

    void mvRecreateSwapChain()
    {
        vkDeviceWaitIdle(mvGetLogicalDevice());

        GContext->graphics.deletionQueue.flush();

        mvShutdownAllocator();
        mvInitializeAllocator();
        mvCreateSwapChain();
        mvCreateMainCommandPool();
        mvCreateMainRenderPass();
        mvCreateMainDepthResources();
        mvCreateFrameBuffers();
    }

    size_t mvGetRequiredUniformBufferSize(size_t size)
    {
        // Calculate required alignment based on minimum device offset alignment
        size_t minUboAlignment = GContext->graphics.deviceProperties.limits.minUniformBufferOffsetAlignment;
        size_t alignedSize = size;

        if (minUboAlignment > 0)
            alignedSize = (alignedSize + minUboAlignment - 1) & ~(minUboAlignment - 1);

        return alignedSize;
    }

    void mvDraw(uint32_t vertexCount)
    {
        vkCmdDrawIndexed(mvGetCurrentCommandBuffer(), vertexCount, 1, 0, 0, 0);
    }

    VkCommandBuffer mvBeginSingleTimeCommands()
    {
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = GContext->graphics.commandPool;
        allocInfo.commandBufferCount = 1;

        VkCommandBuffer commandBuffer;
        vkAllocateCommandBuffers(mvGetLogicalDevice(), &allocInfo, &commandBuffer);

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        vkBeginCommandBuffer(commandBuffer, &beginInfo);

        return commandBuffer;
    }

    void mvEndSingleTimeCommands(VkCommandBuffer commandBuffer)
    {
        vkEndCommandBuffer(commandBuffer);

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;

        vkQueueSubmit(GContext->graphics.graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
        vkDeviceWaitIdle(mvGetLogicalDevice());

        vkFreeCommandBuffers(mvGetLogicalDevice(), GContext->graphics.commandPool, 1, &commandBuffer);
    }

    void mvCreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory)
    {
        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = size;
        bufferInfo.usage = usage;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateBuffer(mvGetLogicalDevice(), &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
            throw std::runtime_error("failed to create buffer!");
        }

        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(mvGetLogicalDevice(), buffer, &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = mvFindMemoryType(GContext->graphics.physicalDevice, memRequirements.memoryTypeBits, properties);

        if (vkAllocateMemory(mvGetLogicalDevice(), &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate buffer memory!");
        }

        vkBindBufferMemory(mvGetLogicalDevice(), buffer, bufferMemory, 0);
    }

    void mvCopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
    {
        VkCommandBuffer commandBuffer = mvBeginSingleTimeCommands();

        VkBufferCopy copyRegion{};
        copyRegion.size = size;
        vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

        mvEndSingleTimeCommands(commandBuffer);
    }

    void mvTransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout)
    {
        VkCommandBuffer commandBuffer = mvBeginSingleTimeCommands();
        VkImageMemoryBarrier barrier{};
        barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier.oldLayout = oldLayout;
        barrier.newLayout = newLayout;
        barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.image = image;
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        barrier.subresourceRange.baseMipLevel = 0;
        barrier.subresourceRange.levelCount = 1;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount = 1;

        VkPipelineStageFlags sourceStage;
        VkPipelineStageFlags destinationStage;

        if (newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
        {
            barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

            //if (hasStencilComponent(format))
            //    barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
        }
        else
            barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;

        if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
        {
            barrier.srcAccessMask = 0;
            barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

            sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
            destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        }

        else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
        {
            barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

            sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
            destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        }

        else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
        {
            barrier.srcAccessMask = 0;
            barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

            sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
            destinationStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        }
        else
            throw std::invalid_argument("unsupported layout transition!");

        vkCmdPipelineBarrier(
            commandBuffer,
            sourceStage, destinationStage,
            0,
            0, nullptr,
            0, nullptr,
            1, &barrier
        );

        mvEndSingleTimeCommands(commandBuffer);
    }

    void mvCopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height)
    {
        VkCommandBuffer commandBuffer = mvBeginSingleTimeCommands();

        VkBufferImageCopy region{};
        region.bufferOffset = 0;
        region.bufferRowLength = 0;
        region.bufferImageHeight = 0;

        region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        region.imageSubresource.mipLevel = 0;
        region.imageSubresource.baseArrayLayer = 0;
        region.imageSubresource.layerCount = 1;

        region.imageOffset = { 0, 0, 0 };
        region.imageExtent = {
            width,
            height,
            1
        };

        vkCmdCopyBufferToImage(
            commandBuffer,
            buffer,
            image,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            1,
            &region
        );

        mvEndSingleTimeCommands(commandBuffer);
    }

}
