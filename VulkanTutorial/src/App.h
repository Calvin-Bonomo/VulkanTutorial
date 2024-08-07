#pragma once

#define GLM_FORCE_RADIANS
#define GLFW_INCLUDE_VULKAN
#define STB_IMAGE_IMPLEMENTATION

#include <stb_image.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <optional>
#include <set>
#include <limits>
#include <fstream>
#include <chrono>

#include "Vertex.h"

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

const std::vector<Vertex> vertices =
{
	{ { -0.5f, -0.5f }, { 1.0f, 0.0f, 0.0f } },
	{ {  0.5f, -0.5f }, { 0.0f, 1.0f, 0.0f } },
	{ {  0.5f,  0.5f }, { 0.0f, 0.0f, 1.0f } },
	{ { -0.5f,  0.5f }, { 1.0f, 1.0f, 1.0f } },
};

const std::vector<uint16_t> indices =
{
	0, 1, 2, 2, 3, 0
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
		m_DescriptorSetLayout(VK_NULL_HANDLE),
		m_GraphicsPipeline(VK_NULL_HANDLE),
		m_SwapChainFramebuffers({}),
		m_CommandPool(VK_NULL_HANDLE),
		m_CommandBuffers({}),
		m_ImageAvailableSemaphores({}),
		m_RenderFinishedSemaphores({}),
		m_InFlightFences({}),
		m_CurrentFrame(0),
		m_FramebufferResized(false),
		m_VertexBuffer(VK_NULL_HANDLE),
		m_VertexBufferMemory(VK_NULL_HANDLE),
		m_IndexBuffer(VK_NULL_HANDLE),
		m_IndexBufferMemory(VK_NULL_HANDLE),
		m_UniformBuffers({}),
		m_UniformBuffersMemory({}),
		m_UniformBuffersMapped({}),
		m_DescriptorPool(VK_NULL_HANDLE),
		m_DescriptorSets({}) {}
	void Run();

	// Class members
private:
	GLFWwindow *m_pWindow; // glfw window object
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
	VkDescriptorSetLayout m_DescriptorSetLayout;
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
	VkBuffer m_VertexBuffer;
	VkDeviceMemory m_VertexBufferMemory;
	VkBuffer m_IndexBuffer;
	VkDeviceMemory m_IndexBufferMemory;
	std::vector<VkBuffer> m_UniformBuffers;
	std::vector<VkDeviceMemory> m_UniformBuffersMemory;
	std::vector<void*> m_UniformBuffersMapped;
	VkDescriptorPool m_DescriptorPool;
	std::vector<VkDescriptorSet> m_DescriptorSets;
	VkImage m_TextureImage;
	VkDeviceMemory m_TextureImageMemory;

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

	struct UniformBufferObject {
		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 proj;
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
	void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo);
	void SetupDebugMessenger();
	SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);
	VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);
	VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes);
	VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities);
	void CreateImageViews();
	void CreateGraphicsPipeline();
	VkShaderModule CreateShaderModule(const std::vector<char> &code);
	void CreateRenderPass();
	void CreateFramebuffers();
	void CreateCommandPool();
	void CreateCommandBuffers();
	void RecordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
	void DrawFrame();
	void CreateSyncObjects();
	void RecreateSwapChain();
	void CleanupSwapChain();
	void CreateVertexBuffer();
	uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
	void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags flags, VkMemoryPropertyFlags properties, VkBuffer &buffer, VkDeviceMemory &bufferMemory);
	void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
	void CreateIndexBuffer();
	void CreateDescriptorSetLayout();
	void CreateUniformBuffers();
	void UpdateUniformBuffer(uint32_t currentImage);
	void CreateDescriptorPool();
	void CreateDescriptorSets();
	void CreateTextureImage();
	void CreateImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
	VkCommandBuffer BeginSingleTimeCommands();
	void EndSingleTimeCommands(VkCommandBuffer commandBuffer);
	void TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
	void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);


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