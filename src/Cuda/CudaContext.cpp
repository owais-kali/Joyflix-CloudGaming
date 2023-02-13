#include "CudaContext.h"

static void* s_hModule = nullptr;

static bool FindModule()
{
    if (s_hModule)
        return true;
    s_hModule = dlopen("libcuda.so.1", RTLD_LAZY | RTLD_GLOBAL);
    if (!s_hModule)
        return false;

    // Close handle immediately because going to call `dlopen` again
    // in the implib module when cuda api called on Linux.
    dlclose(s_hModule);
    s_hModule = nullptr;

    return true;
}

bool LoadInstanceFunctions(const VkInstance instance)
{
//    vkGetInstanceProcAddr(nullptr, "");
//    if (vkGetPhysicalDeviceProperties2KHR == nullptr)
//    {
//        std::cout << "Failed to retrieve vkGetPhysicalDeviceProperties2KHR" << std::endl;
//        return false;
//    }
    return true;
}

static CUresult CheckDriverVersion()
{
    int driverVersion = 0;
    CUresult result = cuDriverGetVersion(&driverVersion);
    if (result != CUDA_SUCCESS)
    {
        return result;
    }

    if (kRequiredDriverVersion > driverVersion)
    {
//        RTC_LOG(LS_ERROR) << "CUDA driver version is not higher than the required version. " << driverVersion;
        return CUDA_ERROR_NO_DEVICE;
    }
    return CUDA_SUCCESS;
}

CudaContext::CudaContext()
        : m_context(nullptr)
{

}

bool CudaContext::GetPhysicalDeviceUUIDInto( VkInstance instance, VkPhysicalDevice phyDevice, std::array<uint8_t, VK_UUID_SIZE>* deviceUUID){
    VkPhysicalDeviceIDPropertiesKHR deviceIDProps = {};
    deviceIDProps.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ID_PROPERTIES_KHR;

    VkPhysicalDeviceProperties2KHR props = {};
    props.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2_KHR;
    props.pNext = &deviceIDProps;

//    vkGetPhysicalDeviceProperties2KHR = (PFN_vkGetPhysicalDeviceProperties2KHR)vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceProperties2KHR");
//    vkGetPhysicalDeviceProperties2KHR(phyDevice, &props);
//    std::memcpy(deviceUUID->data(), deviceIDProps.deviceUUID, VK_UUID_SIZE);

    return true;
}
namespace bytes {
    inline std::ostream &operator<<(std::ostream &os, const char c) {
        return os << (std::is_signed<char>::value
                      ? static_cast<int>(c)
                      : static_cast<unsigned int>(c));
    }

    inline std::ostream &operator<<(std::ostream &os, const signed char c) {
        return os << static_cast<int>(c);
    }

    inline std::ostream &operator<<(std::ostream &os, const unsigned char c) {
        return os << static_cast<unsigned int>(c);
    }
}

CUresult CudaContext::Init()
{
    // dll check
    bool found = FindModule();
    if (!found)
    {
        return CUDA_ERROR_NOT_FOUND;
    }

    CUresult result = CheckDriverVersion();
    if (result != CUDA_SUCCESS)
    {
        return result;
    }

    CUdevice cuDevice = 0;
    result = cuInit(0);
    if (result != CUDA_SUCCESS)
    {
        return result;
    }

    int numDevices = 0;
    result = cuDeviceGetCount(&numDevices);
    if (result != CUDA_SUCCESS)
    {
        return result;
    }

    std::array<uint8_t, VK_UUID_SIZE> deviceUUID;
//    if (!GetPhysicalDeviceUUIDInto(instance, physicalDevice, &deviceUUID))
//    {
//        printf("CUDA_ERROR_INVALID_DEVICE\n");
//        return CUDA_ERROR_INVALID_DEVICE;
//    }

    // Loop over the available devices and identify the CUdevice  corresponding to the physical device in use by
    // this Vulkan instance. This is required because there is no other way to match GPUs across API boundaries.
    bool foundDevice = false;

    cuDeviceGet(&cuDevice, 0);
    cuDeviceGetUuid(&id, cuDevice);

    printf("deviceUUID: size:%d\n", 16);
    for (int i = 0; i < 16; ++i) {
        using namespace bytes;
        printf("%d", (uint8_t)id.bytes[i]);
    }
    printf("\n");

    return CUDA_SUCCESS;
}

