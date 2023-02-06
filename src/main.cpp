#include "stdio.h"
#include "Vulkan/VulkanBase.h"
class VulkanApp : public VulkanBase{
public:
    VulkanApp() : VulkanBase(true)
    {

    }
};

VulkanApp* vulkanApp;

void StartVulkanApp(){
    vulkanApp->initVulkan();
}

int main(int argc, char * argv[])
{
    for (size_t i = 0; i < argc; i++) { VulkanApp::args.push_back(argv[i]); };
    vulkanApp = new VulkanApp();
    StartVulkanApp();

    delete(vulkanApp);
}