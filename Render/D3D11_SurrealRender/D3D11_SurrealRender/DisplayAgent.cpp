#include "DisplayAgent.h"

// Releases *Device, *SwapChain, *Context, *RenderTargetView, and Viewport from memory.
void DisplayAgent::ReleaseInterfaces()
{
    Context->Release();
    SwapChain->Release();
    Device->Release();
    RenderTargetView->Release();
    InputLayout->Release();
    VertexBuffer->Release();
    VertexShader->Release();
    PixelShader->Release();
}

void DisplayAgent::PresentFromRenderTarget()
{
    float Color[] = { 0.0f, 1.0f, 1.0f, 1.0f };
    Context->ClearRenderTargetView(RenderTargetView, Color);

    // Setup Render Targets.
    ID3D11RenderTargetView* TempRTV[] = { RenderTargetView };   // Output manager.
    Context->OMSetRenderTargets(1, TempRTV, 0);
    Context->RSSetViewports(1, &Viewport);                      // This is the Rasterizer.
    Context->IASetInputLayout(InputLayout);                     // Input assembler.

    UINT Strides[] = { sizeof(Vertex) };
    UINT Offsets[] = { 0 };
    ID3D11Buffer* TempVertexBuffer[] = { VertexBuffer };
    Context->IASetVertexBuffers(0, 1, TempVertexBuffer, Strides, Offsets);
    Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    Context->VSSetShader(VertexShader, 0, 0);                         // Vertex Shader stage.
    Context->PSSetShader(PixelShader, 0, 0);                         // Pixel Shader stage.

    Context->Draw(3, 0);

    // Present the SwapChain with the option to Cap the framerate to moniters maximum.
    SwapChain->Present(FrameSyncControl, 0);
}