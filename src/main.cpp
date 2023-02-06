#include "stdio.h"
#include "Vulkan/VulkanBase.h"
class VulkanApp : public VulkanBase{
public:
    VulkanApp() : VulkanBase(true)
    {

    }
};
VulkanApp vulkanApp = {};

void StartVulkanApp(){
    vulkanApp.initVulkan();
}

int main(int argc, char * argv[])
{
    StartVulkanApp();
}