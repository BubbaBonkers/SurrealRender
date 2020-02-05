#include "DisplayAgent.h"

// Releases *Device, *SwapChain, *Context, *RenderTargetView, and Viewport from memory.
void DisplayAgent::ReleaseInterfaces()
{
    Context->Release();
    SwapChain->Release();
    Device->Release();
    RenderTargetView->Release();
}