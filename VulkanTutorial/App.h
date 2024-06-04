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

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

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
			m_SwapChainExtent({0, 0}) {}
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

	// Challenge Problems
	void AllExtensionsRequired(std::vector<const char*> glfwExtensions, uint32_t glfwExtensionCount);

private:
	// Custom debug callback
	static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData) {
		// TODO: Implement console text coloring based on severity
		std::cerr << "validation layer " << pCallbackData->pMessage << std::endl;

		return VK_FALSE;
	}
};