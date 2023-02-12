add_executable(CUDA)

target_sources(CUDA
        PRIVATE
        CudaApp.cpp
        CudaContext.cpp
        CudaContext.h)