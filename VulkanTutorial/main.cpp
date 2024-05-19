#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <vector>
#include <algorithm>

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

const std::vector<const char*> validationLayers = {
	"VK_LAYER_KHRONOS_validation"
};

#ifndef _DEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

class HelloTriangleApplication {
public:
	void run() {
		initWindow();
		initVulkan();
		mainLoop();
		cleanup();
	}
private:
	// glfw window object
	GLFWwindow* window;
	// Vulkan instance handle
	VkInstance instance;

private:
	void initWindow() {
		// Initialize GLFW
		glfwInit();

		// Disable OpenGL API
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		// Do not allow window resizing... for now
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		// Create a pointer to the window object
		window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
	}

	void initVulkan() {
		createInstance();
	}

	void createInstance() {
		// Check validation layers
		if (enableValidationLayers && !checkValidationLayerSupport()) {
			throw std::runtime_error("validation layers requested, but not available!");
		}

		// Define application struct
		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Hello Triangle";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "No Engine";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_0;

		// Define instance struct
		VkInstanceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;

		// Check for extensions required for vulkan
		uint32_t glfwExtensionCount = 0;

		const char** glfwExtensions;

		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

#ifdef _DEBUG
		allExtensionsRequired(glfwExtensions, glfwExtensionCount);
#endif
		createInfo.enabledExtensionCount = glfwExtensionCount;
		createInfo.ppEnabledExtensionNames = glfwExtensions;
		if (enableValidationLayers) {
			createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
			createInfo.ppEnabledLayerNames = validationLayers.data();
		}
		else {
			createInfo.enabledLayerCount = 0;
		}

		// Create the instance
		if(vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
			throw std::runtime_error("failed to create instance!");
		}
	}

	bool checkValidationLayerSupport() {
		// Find all requested validation layers
		uint32_t layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

		std::vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
		bool layerFound;

		for (const char* layerName : validationLayers) {
			layerFound = false;

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

	void mainLoop() {
		// Poll for events while the window is open
		while (!glfwWindowShouldClose(window)) {
			glfwPollEvents();
		}
	}

	void cleanup() {
		// Cleanup vulkan instance
		vkDestroyInstance(instance, nullptr);

		// Cleanup window object
		glfwDestroyWindow(window);
		
		// Cleanup glfw
		glfwTerminate();
	}

	// Challenge Problem: Check if all supported
	// extensions are required
	void allExtensionsRequired(const char** glfwExtensions, uint32_t glfwExtensionCount) {
		// Get all supported extensions
		uint32_t extensionCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

		std::vector<VkExtensionProperties> extensions(extensionCount);

		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

		bool allRequiredExtensionsSupported = true;
		bool foundExtension;

		std::vector<VkExtensionProperties> nonRequiredExtensions(extensionCount);
		std::copy(extensions.begin(), extensions.end(), nonRequiredExtensions.begin());

		std::cout << "The following extensions are required:" << std::endl;

		for (uint32_t i = 0; i < glfwExtensionCount; i++) {
			foundExtension = false;
			for (auto& extension : extensions) {
				if (strcmp(extension.extensionName, glfwExtensions[i]) == 0) {
					foundExtension = true;

					std::cout << "\t" << extension.extensionName << std::endl;
					auto extensionIterator = std::find_if(nonRequiredExtensions.begin(), nonRequiredExtensions.end(), 
												[extension](VkExtensionProperties extensionFind) -> bool 
													{ return strcmp(extension.extensionName, extensionFind.extensionName) == 0; });
					if (extensionIterator != nonRequiredExtensions.end()) {
						nonRequiredExtensions.erase(extensionIterator);
					}
					break;
				}
			}

			if (!foundExtension) {
				allRequiredExtensionsSupported = false;
				std::cout << "Unable to find required extension " << glfwExtensions[i] << std::endl;
			}
		}

		std::cout << "The following extensions are not required:" << std::endl;
		for (auto& extension : nonRequiredExtensions) {
			std::cout << "\t" << extension.extensionName << std::endl;
		}
	}
};

int main() {
	HelloTriangleApplication app;

	try {
		app.run();
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}