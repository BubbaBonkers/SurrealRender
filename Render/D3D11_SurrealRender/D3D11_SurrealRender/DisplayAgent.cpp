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
    ID3D11RenderTargetView* TempRTV[] = { RenderTargetView };       // Output manager.
    Context->OMSetRenderTargets(1, TempRTV, 0);
    Context->RSSetViewports(1, &Viewport);                          // This is the Rasterizer.
    Context->IASetInputLayout(InputLayout);                         // Input assembler.

    UINT Strides[] = { sizeof(Vertex) };
    UINT Offsets[] = { 0 };
    ID3D11Buffer* TempVertexBuffer[] = { VertexBuffer };
    Context->IASetVertexBuffers(0, 1, TempVertexBuffer, Strides, Offsets);
    Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    Context->VSSetShader(VertexShader, 0, 0);                       // Vertex Shader stage.
    Context->PSSetShader(PixelShader, 0, 0);                        // Pixel Shader stage.

    // Timer for rotation.
    static float Rotation = 0;
    Rotation += 0.0001f;

    // World Matrix.
    XMMATRIX Temp = XMMatrixIdentity();                             // High speed type.
    Temp = XMMatrixTranslation(0, 0, 2);                            // Move camera back by 4.
    XMMATRIX Temp2 = XMMatrixRotationY(Rotation);
    Temp = XMMatrixMultiply(Temp2, Temp);
    XMStoreFloat4x4(&SpacialEnvironment.WorldMatrix, Temp);         // Turn the slow speed WorldMatrix into high speed XMMATRIX type.

    // View Matrix.
    Temp = XMMatrixLookAtLH({ 2, 2, -1 }, { 0, 0, 1 }, { 0, 1, 0 });
    XMStoreFloat4x4(&SpacialEnvironment.ViewMatrix, Temp);

    // Projection.
    Temp = XMMatrixPerspectiveFovLH((3.14f / 2.0f), AspectRatio, 0.1f, 1000.0f);
    XMStoreFloat4x4(&SpacialEnvironment.ProjectionMatrix, Temp);

    D3D11_MAPPED_SUBRESOURCE GPUBuffer;
    HRESULT hr = Context->Map(ConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &GPUBuffer);
    //*((XMFLOAT4X4*)(GPUBuffer.pData)) = SpacialEnvironment.WorldMatrix;
    memcpy(GPUBuffer.pData, &SpacialEnvironment, sizeof(Environment));
    Context->Unmap(ConstantBuffer, 0);

    // Apply matrix math in vertex shader and connect the constant buffer to the pipeline.
    ID3D11Buffer* Constants[] = { ConstantBuffer };
    Context->VSSetConstantBuffers(0, 1, Constants);

    // Draw? LAST
    Context->Draw(VertexCount, 0);

    // Immediate Context.


    // Present the SwapChain with the option to Cap the framerate to moniters maximum.
    SwapChain->Present(FrameSyncControl, 0);
}