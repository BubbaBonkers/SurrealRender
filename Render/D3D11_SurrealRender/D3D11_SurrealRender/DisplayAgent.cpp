#include "DisplayAgent.h"
#include "DDSTextureLoader.h"

#define SAFE_RELEASE(ptr) { if(ptr) { ptr->Release(); ptr = nullptr; } }
#define D3DXToRadian(degree) ((degree) * (D3DX_PI / 180.0f))

// Called every frame.
void DisplayAgent::Update(float DeltaTime)
{

}

// Called before the window closes.
void DisplayAgent::EndPlay()
{
    ReleaseInterfaces();
    ReleasePointerObjects(true, true);
}

// Create a new camera in 3D space.
Camera* DisplayAgent::CreateCamera(const char* DebugName)
{
    // Create the camera to view the world through.
    Camera* MainCamera = new Camera();;
    MainCamera->CreateCamera(DebugName);
    WorldCameras.push_back(MainCamera);

    return MainCamera;
}

// Creates a new object in 3D space.
Object* DisplayAgent::CreateObject(const char* DebugName, const char* FileName, bool bHide)
{
    // Create the cube.mesh for testing the object creation.
    Object* NewObject = new Object();
    NewObject->CreateObject(DebugName, FileName, bHide);             // Create the object and initialize information.
    WorldObjects.push_back(NewObject);

    return NewObject;
}

// Creates a new object in 3D space.
Object* DisplayAgent::CreateObject(const char* DebugName, std::vector<Vertex> VertexData, std::vector<int> IndexData, bool bHide)
{
    // Create the cube.mesh for testing the object creation.
    Object* NewObject = new Object();
    NewObject->CreateObject(DebugName, VertexData, IndexData, bHide);             // Create the object and initialize information.
    WorldObjects.push_back(NewObject);

    return NewObject;
}

void DisplayAgent::PresentFromRenderTarget(Object* Obj)
{
    // Call update on all objects and cameras to check for changes in state.
    for (unsigned int i = 0; i < WorldCameras.size(); ++i)
    {
        WorldCameras[i]->Update();
    }
    for (unsigned int i = 0; i < WorldObjects.size(); ++i)
    {
        WorldObjects[i]->Update();
    }

    Context->ClearRenderTargetView(RenderTargetView, RenderBackgroundColor);
    Context->ClearDepthStencilView(ZBufferView, D3D11_CLEAR_DEPTH, 1.0f, 0);

    // Setup Render Targets.
    ID3D11RenderTargetView* TempRTV[] = { RenderTargetView };       // Output manager.
    Context->OMSetRenderTargets(1, TempRTV, ZBufferView);
    Context->RSSetViewports(1, &Viewport);                          // This is the Rasterizer.
    
    // Immediate Context. Set the pipeline.
    UINT MeshStrides[] = { sizeof(Vertex) };
    UINT MeshOffsets[] = { 0 };
    ID3D11Buffer* TempMeshVertexBuffer[] = { MeshVertexBuffer };
    Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    Context->IASetVertexBuffers(0, 1, TempMeshVertexBuffer, MeshStrides, MeshOffsets);
    Context->IASetIndexBuffer(MeshIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
    Context->IASetInputLayout(InputLayout);

    // Load the Texture
    HRESULT hr;// = CreateDDSTextureFromFile(Device, L"Assets/Crate.dds", nullptr, &ShaderResourceView);

    // Create the sample state
    D3D11_SAMPLER_DESC sampDesc = {};
    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
    hr = Device->CreateSamplerState(&sampDesc, &LinearSamplerState);

    // Timer for rotation.
    static float Rotation = 0;
    Rotation += 0.0001f;
    
    // World Matrix.
    XMMATRIX Temp;                             // High speed type.
    XMMATRIX Temp2;

    // View Matrix.
    Temp = XMMatrixLookAtLH({ 0, 2, -5 }, { 0, 0, 0 }, { 0, 1, 0 });
    XMStoreFloat4x4(&SpacialEnvironment.ViewMatrix, Temp);

    // Projection.
    Temp = XMMatrixPerspectiveFovLH((XMConvertToRadians(FieldOfViewDeg)), AspectRatio, 0.1f, 1000.0f);
    XMStoreFloat4x4(&SpacialEnvironment.ProjectionMatrix, Temp);

    D3D11_MAPPED_SUBRESOURCE GPUBuffer;
    hr = Context->Map(ConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &GPUBuffer);
    memcpy(GPUBuffer.pData, &SpacialEnvironment, sizeof(Environment));
    Context->Unmap(ConstantBuffer, 0);

    // Apply matrix math in vertex shader and connect the constant buffer to the pipeline.
    ID3D11Buffer* Constants[] = { ConstantBuffer };
    Context->VSSetConstantBuffers(0, 1, Constants);

    // Vertex and Pixel Shader stages.
    Context->VSSetShader(MeshVertexShader, 0, 0);
    Context->PSSetShader(PixelShader, 0, 0);                            // Pixel Shader stage.
    Context->PSSetShaderResources(0, 1, &ShaderResourceView);
    Context->PSSetSamplers(0, 1, &LinearSamplerState);

    // Modify the world matrix before drawing the next thing.
    Temp = XMMatrixIdentity();
    Temp2 = XMMatrixRotationY(Rotation);
    Temp = XMMatrixMultiply(Temp2, Temp);
    XMStoreFloat4x4(&SpacialEnvironment.WorldMatrix, Temp);

    // Send data to the video card.
    hr = Context->Map(ConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &GPUBuffer);
    memcpy(GPUBuffer.pData, &SpacialEnvironment, sizeof(Environment));
    Context->Unmap(ConstantBuffer, 0);

    Context->DrawIndexed(Obj->Indices.size(), 0, 0);

    // Present the SwapChain with the option to Cap the framerate to moniters maximum.
    SwapChain->Present(FrameSyncControl, 0);
}

// Releases *Device, *SwapChain, *Context, *RenderTargetView, and Viewport from memory.
void DisplayAgent::ReleaseInterfaces()
{
    SAFE_RELEASE(Context);
    SAFE_RELEASE(SwapChain);
    SAFE_RELEASE(Device);
    SAFE_RELEASE(RenderTargetView);
    SAFE_RELEASE(InputLayout);
    SAFE_RELEASE(VertexBuffer);
    SAFE_RELEASE(PixelShader);
    SAFE_RELEASE(MeshVertexShader);
    SAFE_RELEASE(ConstantBuffer);
    SAFE_RELEASE(MeshVertexBuffer);
    SAFE_RELEASE(MeshIndexBuffer);
    SAFE_RELEASE(ShaderResourceView);
    SAFE_RELEASE(LinearSamplerState);
    SAFE_RELEASE(ZBuffer);
    SAFE_RELEASE(ZBufferView);
}

// Delete all pointers before close.
void DisplayAgent::ReleasePointerObjects(bool bCameras, bool bObjects)
{
    if (bCameras)
    {
        for (unsigned int i = 0; i < WorldCameras.size(); ++i)
        {
            WorldCameras[i] = nullptr;
            delete WorldCameras[i];
        }
    }

    if (bObjects)
    {
        for (unsigned int i = 0; i < WorldObjects.size(); ++i)
        {
            WorldObjects[i] = nullptr;
            delete WorldObjects[i];
        }
    }
}