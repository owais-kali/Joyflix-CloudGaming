#include "stdio.h"
#include "Vulkan/VulkanBase.h"
class VulkanApp : public VulkanBase{
public:
    VulkanApp() : VulkanBase(true)
    {
    }
    ~VulkanApp(){}

    void draw(){}
    void updateUniformBuffers(){}

    virtual void render()
    {
        if (!prepared)
            return;
        vkDeviceWaitIdle(device);
        draw();
        vkDeviceWaitIdle(device);
        if (!paused)
        {
            updateUniformBuffers();
        }
    }

    virtual void viewChanged()
    {
        updateUniformBuffers();
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