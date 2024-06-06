#pragma once

#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <optional>
#include <set>
#include <limits>
#include <fstream>

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;
const int MAX_FRAMES_IN_FLIGHT = 2;

const std::vector<const char*> validationLayers = 
{
	"VK_LAYER_KHRONOS_validation",
};

const std::vector<const char*> deviceExtensions = 
{
	VK_KHR_SWAPCHAIN_EXTENSION_NAME,
};

#ifndef _DEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

class HelloTriangleApplication {
public:
	HelloTriangleApplication() : 
			m_pWindow(nullptr), 
			m_Instance(VK_NULL_HANDLE), 
			m_DebugMessenger(VK_NULL_HANDLE),
			m_Surface(VK_NULL_HANDLE),
			m_PhysicalDevice{ VK_NULL_HANDLE },
			m_Device(VK_NULL_HANDLE),
			m_GraphicsQueue(VK_NULL_HANDLE),
			m_PresentQueue(VK_NULL_HANDLE),
			m_SwapChain(VK_NULL_HANDLE),
			m_SwapChainImages({}),
			m_SwapChainImageFormat(VK_FORMAT_UNDEFINED),
			m_SwapChainExtent({0, 0}),
			m_SwapChainImageViews({}),
			m_RenderPass(VK_NULL_HANDLE),
			m_PipelineLayout(VK_NULL_HANDLE),
			m_GraphicsPipeline(VK_NULL_HANDLE),
			m_SwapChainFramebuffers({}),
			m_CommandPool(VK_NULL_HANDLE),
			m_CommandBuffers({}),
			m_ImageAvailableSemaphores({}),
			m_RenderFinishedSemaphores({}),
			m_InFlightFences({}),
			m_CurrentFrame(0),
			m_FramebufferResized(false) {}
	void run();

	// Class members
private:
	GLFWwindow* m_pWindow; // glfw window object
	VkInstance m_Instance; // Vulkan instance handle
	VkDebugUtilsMessengerEXT m_DebugMessenger; // Debug messenger
	VkSurfaceKHR m_Surface; // Render surface
	VkPhysicalDevice m_PhysicalDevice; // Physical device
	VkDevice m_Device; // Logical device
	VkQueue m_GraphicsQueue;
	VkQueue m_PresentQueue;
	VkSwapchainKHR m_SwapChain;
	std::vector<VkImage> m_SwapChainImages;
	VkFormat m_SwapChainImageFormat;
	VkExtent2D m_SwapChainExtent;
	std::vector<VkImageView> m_SwapChainImageViews;
	VkRenderPass m_RenderPass;
	VkPipelineLayout m_PipelineLayout;
	VkPipeline m_GraphicsPipeline;
	std::vector<VkFramebuffer> m_SwapChainFramebuffers;
	VkCommandPool m_CommandPool;
	std::vector<VkCommandBuffer> m_CommandBuffers;
	std::vector<VkSemaphore> m_ImageAvailableSemaphores;
	std::vector<VkSemaphore> m_RenderFinishedSemaphores;
	std::vector<VkFence> m_InFlightFences;
	uint32_t m_CurrentFrame;
	bool m_FramebufferResized;

	// Class structs
private:
	struct SwapChainSupportDetails 
	{
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

	struct QueueFamilyIndices 
	{
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;

		bool IsComplete() {
			return graphicsFamily.has_value() && presentFamily.has_value();
		}
	};

private:
	void InitWindow();
	void InitVulkan();
	void CreateInstance();
	void CreateSurface();
	bool CheckValidationLayerSupport();
	std::vector<const char*> GetRequiredExtensions();
	void PickPhysicalDevice();
	bool IsDeviceSuitable(VkPhysicalDevice device);
	bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
	QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
	void CreateLogicalDevice();
	void CreateSwapChain();
	void MainLoop();
	void Cleanup();
	void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
	void SetupDebugMessenger();
	SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);
	VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
	void CreateImageViews();
	void CreateGraphicsPipeline();
	VkShaderModule CreateShaderModule(const std::vector<char>& code);
	void CreateRenderPass();
	void CreateFramebuffers();
	void CreateCommandPool();
	void CreateCommandBuffers();
	void RecordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
	void DrawFrame();
	void CreateSyncObjects();
	void RecreateSwapChain();
	void CleanupSwapChain();

	// Challenge Problems
	void AllExtensionsRequired(std::vector<const char*> glfwExtensions, uint32_t glfwExtensionCount);

private:
	// Custom debug callback
	static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData) 
	{
		// TODO: Implement console text coloring based on severity
		std::cerr << "validation layer " << pCallbackData->pMessage << std::endl;

		return VK_FALSE;
	}

	static void FramebufferResizeCallback(GLFWwindow* window, int width, int height) 
	{
		auto app = reinterpret_cast<HelloTriangleApplication*>(glfwGetWindowUserPointer(window));
		app->m_FramebufferResized = true;
	}
};