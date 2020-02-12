#include "DisplayAgent.h"
#include "DDSTextureLoader.h"
#include <iostream>

#define SAFE_RELEASE(ptr) { if(ptr) { ptr->Release(); ptr = nullptr; } }
#define D3DXToRadian(degree) ((degree) * (D3DX_PI / 180.0f))

// Called every frame.
void DisplayAgent::Update(float DeltaTime)
{
    // Call update on all objects and cameras to check for changes in state.
    for (unsigned int i = 0; i < WorldCameras.size(); ++i)
    {
        WorldCameras[i]->Update(DeltaTime);
    }
    for (unsigned int i = 0; i < WorldObjects.size(); ++i)
    {
        WorldObjects[i]->Update(DeltaTime);
    }
    for (unsigned int i = 0; i < WorldObjects.size(); ++i)
    {
        WorldPointLights[i]->Update(DeltaTime);
    }
}

// Called before the window closes.
void DisplayAgent::EndPlay()
{
    ReleaseInterfaces();
    ReleasePointerObjects(true, true, true);
}

// Create a new camera, leave AttachTo as "nullptr" to not attach to an object.
Camera* DisplayAgent::CreateCamera(const char* DebugName, Object* AttachTo)
{
    // Create the camera to view the world through.
    Camera* MainCamera = new Camera();;
    MainCamera->CreateCamera(DebugName, AttachTo);
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

// Create a new camera, leave AttachTo as "nullptr" to not attach to an object.
DirectionalLight* DisplayAgent::CreateDirectionalLight(const char* DebugName, XMFLOAT4 direction, XMFLOAT4 color, float intensity)
{
    DirectionalLight* NewDirLight = new DirectionalLight();
    NewDirLight->CreateLight(DebugName, direction, color, intensity);
    WorldDirectionalLights.push_back(NewDirLight);

    return NewDirLight;
}

// Create a new point light.
PointLight* DisplayAgent::CreatePointLight(const char* DebugName, XMFLOAT4 color, float intensity)
{
    PointLight* NewPointLight = new PointLight();
    NewPointLight->CreateLight(DebugName, color, intensity);
    WorldPointLights.push_back(NewPointLight);

    return NewPointLight;
}

void DisplayAgent::ChangeAspectRatio(float InRatio)
{
    for (unsigned int i = 0; i < WorldCameras.size(); ++i)
    {
        WorldCameras[i]->RefreshCameraAspectRatio(InRatio);
    }
}

void DisplayAgent::PresentFromRenderTarget(Camera* Cam, Object* Obj, float DeltaTime)
{
    // Update all objects and cameras.
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

    // Create the sample state
    D3D11_SAMPLER_DESC sampDesc = {};
    sampDesc.Filter = D3D11_FILTER_ANISOTROPIC;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

    // Load the Texture
    HRESULT hr = Device->CreateSamplerState(&sampDesc, &LinearSamplerState);

    // Math Stuffs.

    // Translate the object in world space.
    Obj->AddRotationInput(0, 1.0f, 0);
    WorldPointLights[0]->AddRotationInput(-5.0f, -5.0f, -5.0f, false);
    WorldPointLights[0]->AddMovementInput(-5.0f, 0.0f, 0.0f, false);

    // GPU Mapping.

    // Create the ConstBuffer to send to the graphics card ConstantBuffer.
    ConstBuffer cb1;
    cb1.WorldMatrix = XMLoadFloat4x4(&Obj->WorldMatrix);
    cb1.ViewMatrix = XMMatrixInverse(0, XMLoadFloat4x4(&Cam->SpacialEnvironment.WorldMatrix));
    cb1.ProjectionMatrix = XMLoadFloat4x4(&Cam->SpacialEnvironment.ProjectionMatrix);
    cb1.DirectionalLightDirections[0] = WorldDirectionalLights[0]->Direction;
    cb1.DirectionalLightColors[0] = WorldDirectionalLights[0]->Color;
    cb1.DirectionalLightIntensities = WorldDirectionalLights[0]->Intensity;
    cb1.AmbientLightColor = AmbientLightCol;
    cb1.AmbientLightIntensity = AmbientLightIntense;
    cb1.PointLightColors[0] = WorldPointLights[0]->Color;
    cb1.PointLightIntensities = WorldPointLights[0]->Intensity;
    cb1.PointLightPositions[0] = XMFLOAT4(WorldPointLights[0]->WorldMatrix._41, WorldPointLights[0]->WorldMatrix._42, WorldPointLights[0]->WorldMatrix._43, WorldPointLights[0]->WorldMatrix._44);

    // Send data to the graphics card.
    D3D11_MAPPED_SUBRESOURCE GPUBufferCDS;
    Context->Map(ConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &GPUBufferCDS);
    memcpy(GPUBufferCDS.pData, &cb1, sizeof(cb1));
    Context->Unmap(ConstantBuffer, 0);

    // Apply matrix math in vertex shader and connect the constant buffer to the pipeline.
    ID3D11Buffer* Constants[] = { ConstantBuffer };
    // Shader Stuffs.

    // Vertex and Pixel Shader stages.
    Context->VSSetShader(MeshVertexShader, 0, 0);
    Context->VSSetConstantBuffers(0, 1, Constants);
    Context->PSSetShader(PixelShader, 0, 0);                            // Pixel Shader stage.
    Context->PSSetConstantBuffers(0, 1, Constants);
    Context->PSSetShaderResources(0, 1, &ShaderResourceView);
    Context->PSSetSamplers(0, 1, &LinearSamplerState);

    // Draw and Present.

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
void DisplayAgent::ReleasePointerObjects(bool bCameras, bool bObjects, bool bLights)
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

    if (bLights)
    {
        for (unsigned int i = 0; i < WorldDirectionalLights.size(); ++i)
        {
            WorldDirectionalLights[i] = nullptr;
            delete WorldDirectionalLights[i];
        }
    }

    if (bLights)
    {
        for (unsigned int i = 0; i < WorldPointLights.size(); ++i)
        {
            WorldPointLights[i] = nullptr;
            delete WorldPointLights[i];
        }
    }
}