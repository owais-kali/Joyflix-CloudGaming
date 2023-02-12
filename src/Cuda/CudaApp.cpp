#include <iostream>
#include "CudaContext.h"

int main(int argc, char * argv[])
{
    CudaContext* cuda_ctx = new CudaContext();
    cuda_ctx->Init(nullptr, nullptr);
    delete cuda_ctx;
}