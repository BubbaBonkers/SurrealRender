#include "DisplayAgent.h"
#include <iostream>
#include <string>

#define SAFE_RELEASE(ptr) { if(ptr) { ptr->Release(); ptr = nullptr; } }
#define D3DXToRadian(degree) ((degree) * (D3DX_PI / 180.0f))

// Called when this object is first created. (Game Start)
void DisplayAgent::StartPlay()
{
    // Simple Testing Cube.
    Object* CubeTest = CreateObject("TestingCube", "Assets/cube.mesh", "Assets/Crate.dds");
    Object* MultipleObjectTest = CreateObject("SecondCube", "Assets/FancyBox.mesh", "Assets/FancyBoxDDS.dds");
    MultipleObjectTest->AddMovementInput(300.0f, -50.0f, 55.0f, true);
    MultipleObjectTest->AddRotationInput(-5.0f, 3.0f, 0.0f, true);

    // Waving Cube.
    Object* WavingCube = CreateObject("PlanetWave", "Assets/Planet1.mesh", "Assets/Planet1.dds");
    WavingCube->EmissiveColor = { 1.0f, 0.2f, 0, 1.0f };
    WavingCube->AddMovementInput(2000.0f, 100.0f, -2000.0f, true);
    WavingCube->Scale(3.0f, 3.0f, 3.0f);
    WavingCube->WavingIntensity = 1.0f;

    // Disco Cube.
    Object* StaticSinWaveCube = CreateObject("TestingCubeWave", "Assets/cube.mesh", "Assets/Crate.dds");
    StaticSinWaveCube->DiscoIntensity = 1.0f;
    StaticSinWaveCube->AddMovementInput(-300.0f, 110.0f, -250.0f, true);

    // Testing Torch Mesh.
    Object* Torch = CreateObject("TorchTest", "Assets/Torch.mesh", "Assets/TorchTexture.dds");
    Torch->Scale(30.0f, 30.0f, 30.0f);
    Torch->AddMovementInput(-250.0f, 150.0f, 150.0f, true);

    // Testing Bamboo Mesh.
    Object* Bamboo = CreateObject("Bamboo", "Assets/Bamboo.mesh", "Assets/BambooT.dds");
    Bamboo->Scale(30.0f, 30.0f, 30.0f);
    Bamboo->AddMovementInput(250.0f, 50.0f, -150.0f, true);

    // Testing Bamboo Mesh for Alpha Emissive Light.
    Object* EmissiveBamboo = CreateObject("Bamboo", "Assets/Bamboo.mesh", "Assets/BambooT.dds");
    EmissiveBamboo->EmissiveColor = { 3, 3, 3, 3 };
    EmissiveBamboo->Scale(30.0f, 30.0f, 30.0f);
    EmissiveBamboo->AddMovementInput(250.0f, 50.0f, -150.0f, true);
    EmissiveBamboo->AddRotationInput(0, 15.0f, 0, true);

    // Testing Emmisive Material for Mesh.
    Object* EmissiveCube = CreateObject("EmissiveObject", "Assets/cube.mesh", "Assets/Crate.dds");
    EmissiveCube->EmissiveColor = { 3, 3, 3, 3 };
    EmissiveCube->AddMovementInput(250.0f, -150.0f, -150.0f, true);

    // Simple Wall.
    Object* WallTest = CreateObject("TestingWall", "Assets/cube.mesh", "Assets/Crate.dds");
    WallTest->Scale(10.0f, 10.0f, 1.0f);
    WallTest->AddMovementInput(400.0f, -250.0f, 500.0f, true);

    Object* Origin = CreateObject("TestingCube", "Assets/cube.mesh", "Assets/Crate.dds");
    //Origin->AddMovementInput(0.0f, 10.0f, 0.0f);
    Origin->Scale(0.5f, 0.5f, 0.5f);

    // Camera Acting as Eyes.
    Camera* MainCamera = CreateCamera("Eyes");
    MainCamera->AddMovementInput(0, 3.0f, -5.0f);
    MainCamera->AddRotationInput(-0.45f, 0.0f, 0.0f, true, true);
    MainCamera->LookAtTarget = MultipleObjectTest;

    // Directional Light.
    CreateDirectionalLight("Sunlight", { 1, 1, 1, 1 }, { 1, 1, 1, 1 }, 1.0f);

    // Dynamic Point Light.
    CreatePointLight("PointLightTest", { 0, 1, 1, 1 }, 1.0f);
    XMStoreFloat4x4(&WorldPointLights[0]->WorldMatrix, XMMatrixMultiply(XMMatrixTranslation(13.0f, 15.0f, 0.0f), XMLoadFloat4x4(&WorldPointLights[0]->WorldMatrix)));

    // Static Point Light.
    CreatePointLight("StaticPointLight", { 0, 0, 1.0f, 1 }, 0.0f);
    XMStoreFloat4x4(&WorldPointLights[1]->WorldMatrix, XMMatrixMultiply(XMMatrixTranslation(0.0f, -25.0f, -50.0f), XMLoadFloat4x4(&WorldPointLights[1]->WorldMatrix)));

    // SimpleSpot Light Test.
    CreateSpotLight("SpotLightTest", { 1, 0, 0, 1 }, 1.0f);
    //WorldSpotLights[0]->AddMovementInput(250.0f, 50.0f, -150.0f, true);
    WorldSpotLights[0]->AddRotationInput(0.0f, -90.0f, 0.0f, true);
    //XMMATRIX LookNew = XMMatrixLookAtLH({ WorldSpotLights[0]->WorldMatrix._41, WorldSpotLights[0]->WorldMatrix._42, WorldSpotLights[0]->WorldMatrix._43 }, { WorldObjects[2]->WorldMatrix._41, WorldObjects[2]->WorldMatrix._42, WorldObjects[2]->WorldMatrix._43 }, { 0, 1, 0 });
    //XMStoreFloat4x4(&WorldSpotLights[0]->WorldMatrix, LookNew);
}

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
    for (unsigned int i = 0; i < WorldPointLights.size(); ++i)
    {
        WorldPointLights[i]->Update(DeltaTime);
    }
    for (unsigned int i = 0; i < WorldDirectionalLights.size(); ++i)
    {
        WorldDirectionalLights[i]->Update(DeltaTime);
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
Object* DisplayAgent::CreateObject(const char* DebugName, const char* FileName, const char* TextureDDS, bool bHide)
{
    // Create the cube.mesh for testing the object creation.
    Object* NewObject = new Object();
    NewObject->CreateObject(DebugName, FileName, TextureDDS, bHide);             // Create the object and initialize information.
    WorldObjects.push_back(NewObject);
    NewObject->BeginPlay(this);

    return NewObject;
}

// Creates a new object in 3D space.
Object* DisplayAgent::CreateObject(const char* DebugName, const char* TextureDDS, std::vector<Vertex> VertexData, std::vector<int> IndexData, bool bHide)
{
    // Create the cube.mesh for testing the object creation.
    Object* NewObject = new Object();
    NewObject->CreateObject(DebugName, TextureDDS, VertexData, IndexData, bHide);             // Create the object and initialize information.
    WorldObjects.push_back(NewObject);
    NewObject->BeginPlay(this);

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

// Create a new spot light.
SpotLight* DisplayAgent::CreateSpotLight(const char* DebugName, XMFLOAT4 color, float intensity)
{
    SpotLight* NewSpotLight = new SpotLight();
    NewSpotLight->CreateLight(DebugName, color, intensity);
    WorldSpotLights.push_back(NewSpotLight);

    return NewSpotLight;
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
    // Assert that the constant buffer remains 16-byte aligned.
    //static_assert((sizeof(ConstantBuffer) % 16) == 0, "Constant Buffer size must be 16-byte aligned!");

    // Increase global game time.
    G_GameTime += DeltaTime;

    // Update all objects and cameras.
    Context->ClearRenderTargetView(RenderTargetView, RenderBackgroundColor);
    Context->ClearDepthStencilView(ZBufferView, D3D11_CLEAR_DEPTH, 1.0f, 0);

    // Some movement testing stuff.

    // Translate the object in world space.
    WorldObjects[0]->AddRotationInput(0, 1.0f, 0);
    WorldObjects[3]->AddRotationInput(0, 0, 1.0f);
    WorldObjects[2]->AddRotationInput(0, -0.05f, 0);
    WorldObjects[1]->AddMovementInput(0.0f, sin(G_GameTime) * 100.0f, 0.0f);
    WorldPointLights[0]->AddRotationInput(-5.0f, 0.0f, 0.0f);
    //WorldSpotLights[0]->AddRotationInput(0.0f, 0.5f, 0.0f);
    WorldPointLights[0]->AddMovementInput(-5.0f, 0.0f, 0.0f);
    WorldSpotLights[0]->AddRotationInput(0.0f, 0.0f, 5.0f);

    XMStoreFloat4x4(&WorldSpotLights[0]->WorldMatrix, XMMatrixMultiply(XMMatrixRotationX(sin(G_GameTime) * 10.0f * DeltaTime), XMLoadFloat4x4(&WorldSpotLights[0]->WorldMatrix)));
    XMStoreFloat4x4(&WorldSpotLights[0]->WorldMatrix, XMMatrixMultiply(XMMatrixTranslation(0.0f, sin(G_GameTime) * 10.0f * DeltaTime, 0.0f), XMLoadFloat4x4(&WorldSpotLights[0]->WorldMatrix)));
    WorldSpotLights[0]->AddMovementInput(0.0f, 50.0f, 0.0f);
    //XMStoreFloat4x4(&WorldSpotLights[0]->WorldMatrix, XMMatrixMultiply(XMMatrixRotationY(0.5f * DeltaTime), XMLoadFloat4x4(&WorldSpotLights[0]->WorldMatrix)));
    //XMStoreFloat4x4(&WorldSpotLights[0]->WorldMatrix, XMMatrixMultiply(XMMatrixRotationZ(0.5f * DeltaTime), XMLoadFloat4x4(&WorldSpotLights[0]->WorldMatrix)));

    if (Cam->RotateDirLight)
    {
        WorldDirectionalLights[0]->AddRotationInput(2.0f, 3.0f, 1.0f);
    }

    for (unsigned int i = 0; i < WorldObjects.size(); ++i)
    {
        // Setup Render Targets.
        ID3D11RenderTargetView* TempRTV[] = { RenderTargetView };       // Output manager.
        Context->OMSetRenderTargets(1, TempRTV, ZBufferView);
        //Context->OMSetBlendState(BlendState, 0, 0xffffffff);
        Context->RSSetViewports(1, &Viewport);                          // This is the Rasterizer.

        // Load the Cube object data into the video card.
        D3D11_BUFFER_DESC BufferDescription;
        D3D11_SUBRESOURCE_DATA SubData;
        ZeroMemory(&BufferDescription, sizeof(BufferDescription));
        ZeroMemory(&SubData, sizeof(SubData));

        BufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        BufferDescription.ByteWidth = sizeof(Vertex) * WorldObjects[i]->Vertices.size();
        BufferDescription.CPUAccessFlags = 0;
        BufferDescription.MiscFlags = 0;
        BufferDescription.StructureByteStride = 0;
        BufferDescription.Usage = D3D11_USAGE_IMMUTABLE;

        SubData.pSysMem = WorldObjects[i]->Vertices.data();

        // Index Buffer.
        BufferDescription.BindFlags = D3D11_BIND_INDEX_BUFFER;
        BufferDescription.ByteWidth = sizeof(int) * WorldObjects[i]->Indices.size();
        SubData.pSysMem = WorldObjects[i]->Indices.data();

        // Immediate Context. Set the pipeline.
        UINT MeshStrides[] = { sizeof(Vertex) };
        UINT MeshOffsets[] = { 0 };
        ID3D11Buffer* TempMeshVertexBuffer[] = { WorldObjects[i]->VertexBuffer };
        Context->IASetPrimitiveTopology(WorldObjects[i]->TopologyType);
        Context->IASetVertexBuffers(0, 1, TempMeshVertexBuffer, MeshStrides, MeshOffsets);
        Context->IASetIndexBuffer(WorldObjects[i]->IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
        Context->IASetInputLayout(InputLayout);

        // GPU Mapping.

        // Create the ConstBuffer to send to the graphics card ConstantBuffer.
        ConstBuffer cb1;
        cb1.WorldMatrix = XMLoadFloat4x4(&WorldObjects[i]->WorldMatrix);
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
        cb1.PointLightColors[1] = WorldPointLights[1]->Color;
        cb1.PointLightPositions[1] = XMFLOAT4(WorldPointLights[1]->WorldMatrix._41, WorldPointLights[1]->WorldMatrix._42, WorldPointLights[1]->WorldMatrix._43, WorldPointLights[1]->WorldMatrix._44);
        cb1.CameraWorldMatrix = XMLoadFloat4x4(&Cam->SpacialEnvironment.WorldMatrix);
        cb1.BlinnPhongIntensity = 16.0f;
        cb1.WorldTime = G_GameTime;
        cb1.DeltaTime = DeltaTime;
        cb1.DiscoIntensity = WorldObjects[i]->DiscoIntensity;
        cb1.WavingIntensity = WorldObjects[i]->WavingIntensity;
        cb1.SpotLightColors[0] = WorldSpotLights[0]->Color;
        cb1.SpotLightDirections[0] = XMFLOAT4(WorldSpotLights[0]->WorldMatrix._31, WorldSpotLights[0]->WorldMatrix._22, WorldSpotLights[0]->WorldMatrix._13, WorldSpotLights[0]->WorldMatrix._44);
        cb1.SpotLightPositions[0] = XMFLOAT4(WorldSpotLights[0]->WorldMatrix._41, WorldSpotLights[0]->WorldMatrix._42, WorldSpotLights[0]->WorldMatrix._43, WorldSpotLights[0]->WorldMatrix._44);
        cb1.SpotLightConeRatios = WorldSpotLights[0]->ConeRatio;
        cb1.SpotLightIntensities = WorldSpotLights[0]->Intensity;
        cb1.EmissiveColor = WorldObjects[i]->EmissiveColor;

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
        Context->PSSetSamplers(0, 1, &LinearSamplerState);
        Context->PSSetConstantBuffers(0, 1, Constants);
        Context->PSSetShaderResources(0, 1, &WorldObjects[i]->ShaderResourceView);

        unsigned int sizer = sizeof(ConstantBuffer);
        unsigned int size = sizeof(cb1);
        assert((sizeof(cb1) % 16) == 0);

        // Draw and Present.

        Context->DrawIndexed(WorldObjects[i]->Indices.size(), 0, 0);
    }

    // Present the SwapChain with the option to Cap the framerate to moniters maximum.
    SwapChain->Present(FrameSyncControl, 0);
}

// Releases *Device, *SwapChain, *Context, *RenderTargetView, and Viewport from memory.
void DisplayAgent::ReleaseInterfaces()
{
    SAFE_RELEASE(MeshVertexShader);
    SAFE_RELEASE(PixelShader);
    SAFE_RELEASE(ConstantBuffer);
    SAFE_RELEASE(DiffuseTexture);
    SAFE_RELEASE(BlendState);
    SAFE_RELEASE(InputLayout);
    SAFE_RELEASE(RenderTargetView);
    SAFE_RELEASE(LinearSamplerState);
    SAFE_RELEASE(ZBuffer);
    SAFE_RELEASE(ZBufferView);
    SAFE_RELEASE(SwapChain);
    SAFE_RELEASE(Context);
    SAFE_RELEASE(Device);
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