#include "DisplayAgent.h"

#define SAFE_RELEASE(ptr) { if(ptr) { ptr->Release(); ptr = nullptr; } }
#define D3DXToRadian(degree) ((degree) * (D3DX_PI / 180.0f))

// Releases *Device, *SwapChain, *Context, *RenderTargetView, and Viewport from memory.
void DisplayAgent::ReleaseInterfaces()
{
    SAFE_RELEASE(Context);
    SAFE_RELEASE(SwapChain);
    SAFE_RELEASE(Device);
    SAFE_RELEASE(RenderTargetView);
    SAFE_RELEASE(InputLayout);
    SAFE_RELEASE(VertexBuffer);
    SAFE_RELEASE(VertexShader);
    SAFE_RELEASE(PixelShader);
    SAFE_RELEASE(MeshVertexShader);
    SAFE_RELEASE(ConstantBuffer);
    SAFE_RELEASE(MeshVertexBuffer);
    SAFE_RELEASE(MeshIndexBuffer);
}

void DisplayAgent::PresentFromRenderTarget(Object Obj)
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
    Context->VSSetShader(MeshVertexShader, 0, 0);                       // Vertex Shader stage.
    Context->PSSetShader(PixelShader, 0, 0);                            // Pixel Shader stage.
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
    Temp = XMMatrixLookAtLH({ 0, 2, -5 }, { 0, 0, 0 }, { 0, 1, 0 });
    XMStoreFloat4x4(&SpacialEnvironment.ViewMatrix, Temp);

    // Projection.
    Temp = XMMatrixPerspectiveFovLH((XMConvertToRadians(FieldOfViewDeg)), AspectRatio, 0.1f, 1000.0f);
    XMStoreFloat4x4(&SpacialEnvironment.ProjectionMatrix, Temp);

    D3D11_MAPPED_SUBRESOURCE GPUBuffer;
    HRESULT hr = Context->Map(ConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &GPUBuffer);
    memcpy(GPUBuffer.pData, &SpacialEnvironment, sizeof(Environment));
    Context->Unmap(ConstantBuffer, 0);

    // Apply matrix math in vertex shader and connect the constant buffer to the pipeline.
    ID3D11Buffer* Constants[] = { ConstantBuffer };
    Context->VSSetConstantBuffers(0, 1, Constants);

    // Draw? LAST
    Context->Draw(Obj.Vertices.size(), 0);

    // Immediate Context. Set the pipeline.
    UINT MeshStrides[] = { sizeof(Vertex) };
    UINT MeshOffsets[] = { 0 };
    ID3D11Buffer* TempMeshVertexBuffer[] = { MeshVertexBuffer };
    Context->IASetVertexBuffers(0, 1, TempMeshVertexBuffer, MeshStrides, MeshOffsets);
    Context->IASetIndexBuffer(MeshIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
    Context->VSSetShader(MeshVertexShader, 0, 0);
    Context->IASetInputLayout(InputLayout);

    Context->DrawIndexed(Obj.Indices.size(), 0, 0);

    // Present the SwapChain with the option to Cap the framerate to moniters maximum.
    SwapChain->Present(FrameSyncControl, 0);
}