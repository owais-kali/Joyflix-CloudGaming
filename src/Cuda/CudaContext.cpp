#include "CudaContext.h"

static bool FindModule()
{

}

CUresult CudaContext::Init(const VkInstance instance, VkPhysicalDevice physicalDevice)
{
    // dll check
    bool found = FindModule();
}