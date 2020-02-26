#include "DisplayAgent.h"
#include <iostream>
#include <string>
#include <time.h>
#include <math.h>

#define SAFE_RELEASE(ptr) { if(ptr) { ptr->Release(); ptr = nullptr; } }
#define D3DXToRadian(degree) ((degree) * (D3DX_PI / 180.0f))

// Called when this object is first created. (Game Start)
void DisplayAgent::StartPlay()
{
    // Simple Testing Cube.
    Object* CubeTest = CreateObject("TestingCube", "Assets/cube.mesh", "Assets/Crate.dds");
    CubeTest->SpecularIntensity = 32.0f;

    // Star.
    Object* WavingCube = CreateObject("Star", "Assets/Planet1.mesh", "Assets/Planet1.dds", false, false);
    WavingCube->EmissiveColor = { 1.5f, 0.2f, 0, 1.0f };
    WavingCube->AddMovementInput(600.0f, 10.0f, -450.0f, true);
    WavingCube->Scale(12.0f, 12.0f, 12.0f);
    WavingCube->WavingIntensity = 1.0f;
    WavingCube->WavingMovement = 8.0f;
    WavingCube->WavingOffset = 3.0f;

    // Red Planet.
    Object* RedPlanet = CreateObject("RedPlanet", "Assets/RedPlanet.mesh", "Assets/RedPlanet.dds", false, false);
    RedPlanet->Scale(1.0f, 1.0f, 1.0f);
    RedPlanet->EmissiveColor = { 0.3f, 0.025f, 0.04f, 1.0f };
    RedPlanet->SpecularIntensity = 32.0f;
    RedPlanet->AttachToParent(WavingCube);
    RedPlanet->AddLocalMovementInput(-100.0f, 15.0f, 250.0f, true);

    // Disco Cube.
    Object* StaticSinWaveCube = CreateObject("TestingCubeWave", "Assets/cube.mesh", "Assets/Crate.dds");
    StaticSinWaveCube->DiscoIntensity = 1.0f;
    StaticSinWaveCube->AddMovementInput(-30.0f, 11.0f, -25.0f, true);

    Object* Origin = CreateObject("Origin", "Assets/cube.mesh", "Assets/Crate.dds");
    Origin->Scale(0.5f, 0.5f, 0.5f);

    // Red Planet.
    Object* RedPlanetDome = CreateObject("RedPlanetDome", "Assets/RedPlanetDome.mesh", "Assets/RedPlanetDome.dds");
    RedPlanetDome->Scale(1.0f, 1.0f, 1.0f);
    RedPlanetDome->AddMovementInput(-100.0f, 15.0f, 250.0f, true);
    DirectX::XMStoreFloat4x4(&RedPlanetDome->WorldMatrix, XMMatrixMultiply(XMLoadFloat4x4(&RedPlanetDome->WorldMatrix), XMLoadFloat4x4(&WavingCube->WorldMatrix)));
    RedPlanetDome->SpecularIntensity = 8.0f;
    RedPlanetDome->AttachToParent(RedPlanet);

    // Red Planet.
    Object* RedPlanetDomeB = CreateObject("DepthSorted", "Assets/RedPlanetDome.mesh", "Assets/RedPlanetDome.dds");
    RedPlanetDomeB->Scale(1.0f, 1.0f, 1.0f);
    RedPlanetDomeB->AddMovementInput(-200.0f, 15.0f, 350.0f, true);
    DirectX::XMStoreFloat4x4(&RedPlanetDomeB->WorldMatrix, XMMatrixMultiply(XMLoadFloat4x4(&RedPlanetDomeB->WorldMatrix), XMLoadFloat4x4(&WavingCube->WorldMatrix)));
    RedPlanetDomeB->SpecularIntensity = 8.0f;
    RedPlanetDomeB->bEnableDepthSort = true;

    // Red Planet.
    Object* RedPlanetDomeC = CreateObject("DepthSorted", "Assets/RedPlanetDome.mesh", "Assets/RedPlanetDome.dds");
    RedPlanetDomeC->Scale(1.0f, 1.0f, 1.0f);
    RedPlanetDomeC->AddMovementInput(-300.0f, 15.0f, 450.0f, true);
    DirectX::XMStoreFloat4x4(&RedPlanetDomeC->WorldMatrix, XMMatrixMultiply(XMLoadFloat4x4(&RedPlanetDomeC->WorldMatrix), XMLoadFloat4x4(&WavingCube->WorldMatrix)));
    RedPlanetDomeC->SpecularIntensity = 8.0f;
    RedPlanetDomeC->bEnableDepthSort = true;

    // Red Planet.
    Object* RedPlanetDomeD = CreateObject("DepthSorted", "Assets/RedPlanetDome.mesh", "Assets/RedPlanetDome.dds");
    RedPlanetDomeD->Scale(1.0f, 1.0f, 1.0f);
    RedPlanetDomeD->AddMovementInput(-400.0f, 15.0f, 550.0f, true);
    DirectX::XMStoreFloat4x4(&RedPlanetDomeD->WorldMatrix, XMMatrixMultiply(XMLoadFloat4x4(&RedPlanetDomeD->WorldMatrix), XMLoadFloat4x4(&WavingCube->WorldMatrix)));
    RedPlanetDomeD->SpecularIntensity = 8.0f;
    RedPlanetDomeD->bEnableDepthSort = true;

    // Moon.
    Object* Moon = CreateObject("Moon", "Assets/Moon.mesh", "Assets/Moon.dds", false, false);
    Moon->Scale(1.0f, 1.0f, 1.0f);
    Moon->AttachToParent(RedPlanet);
    Moon->AddLocalMovementInput(40.0f, 15.0f, -100.0f, true);
    Moon->EmissiveColor = { 0.3f, 0.025f, 0.04f, 1.0f };
    Moon->SpecularIntensity = 32.0f;

    for (int x = 0; x < 10; ++x)
    {
        int RandomHelper = time(NULL);

        for (int y = 0; y < 10; ++y)
        {
            srand(RandomHelper);
            int RandX = (rand() % 11);
            int RandY = (rand() % 11);

            // Simple Testing Cube.
            Object* PalmTree = CreateObject("Palm1", "Assets/Palm1.mesh", "Assets/Palm1.dds");
            PalmTree->Scale((1.0f + (RandX / 7.0f)), (1.0f + (RandY / 7.0f)), (1.0f + (RandX / 7.0f)));
            PalmTree->bDisableBackfaceCulling = true;
            PalmTree->SpecularIntensity = 16.0f;

            PalmTree->AddMovementInput((-410.0f + ((x * 5) + RandX)), (183.0f - (x * 0.5f)), (-350.0f + ((y * 10) + RandY)), true);
            PalmTree->AddRotationInput(0.0f, (1.0f + (RandX * 3.0f)), 0.0f, true);

            RandomHelper += 7;
        }

        RandomHelper += 1;
    }

    // Red Planet.
    Object* Earth = CreateObject("Earth", "Assets/Earth.mesh", "Assets/Earth.dds");
    Earth->Scale(3.0f, 3.0f, 3.0f);
    Earth->AddMovementInput(-400.0f, 15.0f, -300.0f, true);
    Earth->AddRotationInput(20.0f, 140.0f, 120.0f, true);
    Earth->EmissiveColor = { 0.3f, 0.001f, 0.04f, 1.0f };
    Earth->SpecularIntensity = 32.0f;
    Earth->BlueWavingIntensity = 1.0f;
    Earth->WavingIntensity = 0.0f;
    Earth->WavingMovement = 0.5f;
    Earth->WavingOffset = 2.0f;

    // Spaceship.
    Object* Spaceship = CreateObject("Spaceship", "Assets/Talon.mesh", "Assets/Talon.dds");
    Spaceship->SpecularIntensity = 8.0f;
    Spaceship->Scale(30.0f, 30.0f, 30.0f);
    Spaceship->bDisableBackfaceCulling = true;
    Spaceship->AddMovementInput(0.0f, 10.0f, 0.0f, true);

    // Skybox.
    Object* Skybox = CreateObject("Skybox", "Assets/cube.mesh", "Assets/Nebula.dds");
    Skybox->Scale(-8000.0f, -8000.0f, -8000.0f);
    Skybox->AddMovementInput(0.0f, 4000.0f, 0.0f, true);
    Skybox->EmissiveColor = { 1, 1, 1, 1 };

    // Camera Acting as Eyes.
    Camera* MainCamera = CreateCamera("Eyes");
    MainCamera->AddMovementInput(0, 3.0f, -5.0f);
    MainCamera->AddRotationInput(-0.45f, 0.0f, 0.0f, true, true);
    MainCamera->LookAtTarget = Spaceship;

    // Directional Light.
    CreateDirectionalLight("Sunlight", { 1, 1, 1, 1 }, { 1, 1, 1, 1 }, 0.8f);

    // Dynamic Point Light.
    CreatePointLight("PointLightTest", { 0, 1, 1, 1 }, 1.0f);
    DirectX::XMStoreFloat4x4(&WorldPointLights[0]->WorldMatrix, XMMatrixMultiply(XMMatrixTranslation(13.0f, 15.0f, 0.0f), XMLoadFloat4x4(&WorldPointLights[0]->WorldMatrix)));

    // Static Point Light.
    CreatePointLight("StaticPointLight", { 0, 0, 1.0f, 1 }, 0.0f);
    DirectX::XMStoreFloat4x4(&WorldPointLights[1]->WorldMatrix, XMMatrixMultiply(XMMatrixTranslation(0.0f, -25.0f, -50.0f), XMLoadFloat4x4(&WorldPointLights[1]->WorldMatrix)));

    // SimpleSpot Light Test.
    CreateSpotLight("SpotLightTest", { 1, 0, 0, 1 }, 1.0f);
    DirectX::XMStoreFloat4x4(&WorldSpotLights[0]->WorldMatrix, XMMatrixMultiply(XMMatrixRotationY(-90.0f), XMLoadFloat4x4(&WorldSpotLights[0]->WorldMatrix)));
    WorldSpotLights[0]->AddMovementInput(10.0f, 0.0f, 0.0f, true);
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

// Get an object by its debug name. This returns the first one found with the name, it will not return any after that. Returns nullptr if no object is found.
Object* DisplayAgent::GetObjectByName(std::string Name)
{
    for (unsigned int i = 0; i < WorldObjects.size(); ++i)
    {
        if (WorldObjects[i]->Name == Name)
        {
            return WorldObjects[i];
        }
    }

    return nullptr;
}

// Get an object by its debug name. This returns the first one found with the name, it will not return any after that. Returns nullptr if no object is found.
std::vector<Object*> DisplayAgent::GetObjectsByName(std::string Name)
{
    std::vector<Object*> ReturnElements;

    for (unsigned int i = 0; i < WorldObjects.size(); ++i)
    {
        if (WorldObjects[i]->Name == Name)
        {
            ReturnElements.push_back(WorldObjects[i]);
        }
    }

    return ReturnElements;
}

// Remove all objects from the world objects if their name matches the input parameter. Returns how many were removed. This does not clear their memory.
int DisplayAgent::RemoveObjectsByName(std::string Name, std::vector<Object*> Add)
{
    int Removed = 0;

    for (unsigned int i = 0; i < WorldObjects.size(); ++i)
    {
        const char* CharName = Name.c_str();

        if (WorldObjects[i]->Name = CharName)
        {
            WorldObjects.erase(WorldObjects.begin() + i);
            Removed++;
        }
    }

    for (unsigned int i = 0; i < Add.size(); ++i)
    {
        WorldObjects.push_back(Add[i]);
    }

    return Removed;
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
Object* DisplayAgent::CreateObject(const char* DebugName, const char* FileName, const char* TextureDDS, bool bHide, bool RenderInUI)
{
    // Create the cube.mesh for testing the object creation.
    Object* NewObject = new Object();
    NewObject->CreateObject(DebugName, FileName, TextureDDS, bHide);             // Create the object and initialize information.
    WorldObjects.push_back(NewObject);

    if (RenderInUI)
    {
        WorldInterfaceObjects.push_back(NewObject);
    }

    NewObject->BeginPlay(this);

    return NewObject;
}

Object* DisplayAgent::CreateObject(const char* DebugName, const char* FileName, const char* TextureDDS, std::vector<Object*>* OutputVector, bool bHide, bool RenderInUI)
{
    // Create the cube.mesh for testing the object creation.
    Object* NewObject = new Object();
    NewObject->CreateObject(DebugName, FileName, TextureDDS, bHide);             // Create the object and initialize information.
    OutputVector->push_back(NewObject);

    if (RenderInUI)
    {
        WorldInterfaceObjects.push_back(NewObject);
    }

    NewObject->BeginPlay(this);

    return NewObject;
}

// Creates a new object in 3D space.
Object* DisplayAgent::CreateObject(const char* DebugName, const char* TextureDDS, std::vector<Vertex> VertexData, std::vector<int> IndexData, bool bHide, bool RenderInUI)
{
    // Create the cube.mesh for testing the object creation.
    Object* NewObject = new Object();
    NewObject->CreateObject(DebugName, TextureDDS, VertexData, IndexData, bHide);             // Create the object and initialize information.
    WorldObjects.push_back(NewObject);

    if (RenderInUI)
    {
        WorldInterfaceObjects.push_back(NewObject);
    }

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

void DisplayAgent::PresentFromRenderTarget(Camera* Cam, float DeltaTime)
{
    // Increase global game time.
    G_GameTime += DeltaTime;
    
    // Translate the object in world space.
    GetObjectByName("TestingCube")->AddRotationInput(0, 1.0f, 0);
    GetObjectByName("RedPlanet")->AddLocalRotationInput(0.0f, -0.4f, 0.0f);
    GetObjectByName("RedPlanetDome")->AddLocalRotationInput(0.0f, -0.4f, 0.0f);
    GetObjectByName("Moon")->AddLocalRotationInput(0.0f, 2.0f, 0.0f);
    GetObjectByName("Star")->AddRotationInput(0.0f, 0.1f, 0.0f);
    GetObjectByName("TestingCubeWave")->AddRotationInput(-5.0f, 0.0f, 0.0f);
    GetObjectByName("Spaceship")->WorldMatrix = Cam->SpacialEnvironment.WorldMatrix;

    GetObjectByName("Spaceship")->SetLocation(Cam->SpacialEnvironment.WorldMatrix._41, (Cam->SpacialEnvironment.WorldMatrix._42 - 0.2f), (Cam->SpacialEnvironment.WorldMatrix._43 + 1.2f));
    Cam->AddMovementInput(0.0f, sin(G_GameTime) * 0.0005f, 0.0f);
    GetObjectByName("Skybox")->SetLocation(Cam->SpacialEnvironment.WorldMatrix._41, Cam->SpacialEnvironment.WorldMatrix._42 + 6000.0f, Cam->SpacialEnvironment.WorldMatrix._43);

    // Get items needed to be sorted by depth.
    std::vector<Object*> SortObjects = GetObjectsByName("DepthSorted");

    // Depth sort the transparent objects in the scene before drawing them.
    for (unsigned int i = 0; i < SortObjects.size(); ++i)
    {
        for (unsigned int j = i + 1; j < SortObjects.size(); ++j)
        {
            float DistanceI = sqrt(pow(SortObjects[i]->WorldMatrix._41 - Cam->SpacialEnvironment.WorldMatrix._41, 2) +
                pow(SortObjects[i]->WorldMatrix._42 - Cam->SpacialEnvironment.WorldMatrix._42, 2) +
                pow(SortObjects[i]->WorldMatrix._43 - Cam->SpacialEnvironment.WorldMatrix._43, 2));
            float DistanceJ = sqrt(pow(SortObjects[j]->WorldMatrix._41 - Cam->SpacialEnvironment.WorldMatrix._41, 2) +
                pow(SortObjects[j]->WorldMatrix._42 - Cam->SpacialEnvironment.WorldMatrix._42, 2) +
                pow(SortObjects[j]->WorldMatrix._43 - Cam->SpacialEnvironment.WorldMatrix._43, 2));

            if (DistanceI > DistanceJ)
            {
                Object* CurrI = SortObjects[i];
                SortObjects[i] = SortObjects[j];
                SortObjects[j] = CurrI;
            }
        }
    }

    //RemoveObjectsByName("DepthSorted", SortObjects);

    // Setup UI stuffs.
    OffscreenViewportA.Width = 400;
    OffscreenViewportA.Height = 500;
    OffscreenViewportA.TopLeftX = 0;
    OffscreenViewportA.TopLeftY = 0;

    Context->RSSetViewports(1, &OffscreenViewportA);
    Context->OMSetBlendState(BlendState, 0, 0xFFFFFFFF);

    // Update all objects and cameras.
    Context->ClearRenderTargetView(PIP_RenderTargetView, RenderBackgroundColor);
    Context->ClearDepthStencilView(PIP_ZBufferView, D3D11_CLEAR_DEPTH, 1.0f, 0);
    
    /*
    for (unsigned int i = 0; i < WorldInterfaceObjects.size(); ++i)
    {
        // Setup Render Targets.
        ID3D11RenderTargetView* TempRTV[] = { PIP_RenderTargetView };       // Output manager.
        Context->OMSetRenderTargets(1, TempRTV, PIP_ZBufferView);

        // Create the ConstBuffer to send to the graphics card ConstantBuffer.
        ConstBuffer cb1;
        cb1.WorldMatrix = XMLoadFloat4x4(&WorldInterfaceObjects[i]->WorldMatrix);
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
        cb1.BlinnPhongIntensity = WorldInterfaceObjects[i]->SpecularIntensity;
        cb1.WorldTime = G_GameTime;
        cb1.DeltaTime = DeltaTime;
        cb1.DiscoIntensity = WorldInterfaceObjects[i]->DiscoIntensity;
        cb1.WavingIntensity = WorldInterfaceObjects[i]->WavingIntensity;
        cb1.WavingOffset = WorldInterfaceObjects[i]->WavingOffset;
        cb1.WavingMovement = WorldInterfaceObjects[i]->WavingMovement;
        cb1.SpotLightColors[0] = WorldSpotLights[0]->Color;
        cb1.SpotLightDirections[0] = XMFLOAT4(WorldSpotLights[0]->WorldMatrix._31, WorldSpotLights[0]->WorldMatrix._22, WorldSpotLights[0]->WorldMatrix._13, WorldSpotLights[0]->WorldMatrix._44);
        cb1.SpotLightPositions[0] = XMFLOAT4(WorldSpotLights[0]->WorldMatrix._41, WorldSpotLights[0]->WorldMatrix._42, WorldSpotLights[0]->WorldMatrix._43, WorldSpotLights[0]->WorldMatrix._44);
        cb1.SpotLightConeRatios = WorldSpotLights[0]->ConeRatio;
        cb1.SpotLightIntensities = WorldSpotLights[0]->Intensity;
        cb1.EmissiveColor = WorldInterfaceObjects[i]->EmissiveColor;
        cb1.BlueWavingIntensity = WorldInterfaceObjects[i]->BlueWavingIntensity;

        Context->IASetPrimitiveTopology(WorldInterfaceObjects[i]->TopologyType);
        Context->VSSetShader(MeshVertexShader, 0, 0);
        Context->VSSetConstantBuffers(0, 1, { &ConstantBuffer });
        Context->PSSetShader(PixelShader, 0, 0);                            // Pixel Shader stage.
        Context->PSSetSamplers(0, 1, &LinearSamplerState);
        Context->PSSetConstantBuffers(0, 1, { &ConstantBuffer });
        Context->PSSetShaderResources(0, 1, &WorldObjects[i]->ShaderResourceView);

        Context->DrawIndexed(WorldObjects[i]->Indices.size(), 0, 0);

        WorldInterfaceObjects[i]->ShaderResourceView = PIP_ShaderResourceView;

        ID3D11ShaderResourceView* pTempResource = NULL;
        Context->PSSetShaderResources(0, 1, &pTempResource);
    }
    */

    if (Cam->RotateDirLight)
    {
        WorldDirectionalLights[0]->AddRotationInput(0.25f, 3.0f, 0.1f);
    }

    Context->OMSetBlendState(BlendState, 0, 0xFFFFFFFF);

    // Update all objects and cameras.
    Context->ClearRenderTargetView(RenderTargetView, RenderBackgroundColor);
    Context->ClearDepthStencilView(ZBufferView, D3D11_CLEAR_DEPTH, 1.0f, 0);

    Context->RSSetViewports(1, &Viewport);
    Context->OMSetRenderTargets(1, { &RenderTargetView }, ZBufferView);
    Context->IASetInputLayout(InputLayout);

    for (unsigned int i = 0; i < WorldObjects.size(); ++i)
    {
        // Setup Render Targets.
        Context->IASetPrimitiveTopology(WorldObjects[i]->TopologyType);

        // Immediate Context. Set the pipeline.
        UINT MeshStrides[] = { sizeof(Vertex) };
        UINT MeshOffsets[] = { 0 };
        Context->IASetVertexBuffers(0, 1, { &WorldObjects[i]->VertexBuffer }, MeshStrides, MeshOffsets);
        Context->IASetIndexBuffer(WorldObjects[i]->IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

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
        cb1.BlinnPhongIntensity = WorldObjects[i]->SpecularIntensity;
        cb1.WorldTime = G_GameTime;
        cb1.DeltaTime = DeltaTime;
        cb1.DiscoIntensity = WorldObjects[i]->DiscoIntensity;
        cb1.WavingIntensity = WorldObjects[i]->WavingIntensity;
        cb1.WavingOffset = WorldObjects[i]->WavingOffset;
        cb1.WavingMovement = WorldObjects[i]->WavingMovement;
        cb1.SpotLightColors[0] = WorldSpotLights[0]->Color;
        cb1.SpotLightDirections[0] = XMFLOAT4(WorldSpotLights[0]->WorldMatrix._31, WorldSpotLights[0]->WorldMatrix._22, WorldSpotLights[0]->WorldMatrix._13, WorldSpotLights[0]->WorldMatrix._44);
        cb1.SpotLightPositions[0] = XMFLOAT4(WorldSpotLights[0]->WorldMatrix._41, WorldSpotLights[0]->WorldMatrix._42, WorldSpotLights[0]->WorldMatrix._43, WorldSpotLights[0]->WorldMatrix._44);
        cb1.SpotLightConeRatios = WorldSpotLights[0]->ConeRatio;
        cb1.SpotLightIntensities = WorldSpotLights[0]->Intensity;
        cb1.EmissiveColor = WorldObjects[i]->EmissiveColor;
        cb1.BlueWavingIntensity = WorldObjects[i]->BlueWavingIntensity;
        cb1.BWIntensity = WorldObjects[i]->BWIntensity;

        // PIP Constant Buffer.
        PIP_ConstantBuffer PIP_ConstBuff;
        PIP_ConstBuff.CameraWorldMatrix = XMLoadFloat4x4(&Cam->SpacialEnvironment.WorldMatrix);
        PIP_ConstBuff.DeltaTime = DeltaTime;
        PIP_ConstBuff.WorldTime = G_GameTime;

        if (WorldObjects[i]->RenderAsUI < 1.0f)
        {
            // Send data to the graphics card.
            D3D11_MAPPED_SUBRESOURCE GPUBufferCDS;
            Context->Map(ConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &GPUBufferCDS);
            memcpy(GPUBufferCDS.pData, &cb1, sizeof(cb1));
            Context->Unmap(ConstantBuffer, 0);
        }
        else
        {
            // Send data to the graphics card.
            D3D11_MAPPED_SUBRESOURCE PIP_GPUBuffer;
            Context->Map(ConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &PIP_GPUBuffer);
            memcpy(PIP_GPUBuffer.pData, &PIP_ConstBuff, sizeof(PIP_ConstBuff));
            Context->Unmap(ConstantBuffer, 0);
        }


        // Set the culling mode based on the object.
        if (WorldObjects[i]->bDisableBackfaceCulling)
        {
            Context->RSSetState(RasterizerStateNoCull);
        }
        else
        {
            Context->RSSetState(RasterizerState);
        }

        // Vertex and Pixel Shader stages.
        Context->VSSetShader(MeshVertexShader, 0, 0);
        Context->VSSetConstantBuffers(0, 1, { &ConstantBuffer });

        if (WorldObjects[i]->RenderAsUI < 1.0f)
        {
            Context->PSSetShader(PixelShader, 0, 0);                            // Pixel Shader stage.
        }
        else
        {
            Context->PSSetShader(PIP_PixelShader, 0, 0);
        }

        Context->PSSetSamplers(0, 1, &LinearSamplerState);
        Context->PSSetConstantBuffers(0, 1, { &ConstantBuffer });
        Context->PSSetShaderResources(0, 1, &WorldObjects[i]->ShaderResourceView);

        // Draw and Present.
        Context->DrawIndexed(WorldObjects[i]->Indices.size(), 0, 0);
    }
    /*
    // Instanced Drawing Call.
    Context->IASetPrimitiveTopology(TempInstanced[0]->TopologyType);

    // Immediate Context. Set the pipeline.
    UINT MeshStrides[] = { sizeof(Vertex) };
    UINT MeshOffsets[] = { 0 };
    Context->IASetVertexBuffers(0, 1, { &TempInstanced[0]->VertexBuffer }, MeshStrides, MeshOffsets);
    Context->IASetIndexBuffer(TempInstanced[0]->IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

    Instanced InstancedCBuff;
    InstancedCBuff.ViewMatrix = XMMatrixInverse(0, XMLoadFloat4x4(&Cam->SpacialEnvironment.WorldMatrix));
    InstancedCBuff.ProjectionMatrix = XMLoadFloat4x4(&Cam->SpacialEnvironment.ProjectionMatrix);
    for (int i = 0; i < TempInstanced.size(); ++i)
    {
        InstancedCBuff.InstanceLocations[i] = XMLoadFloat4x4(&TempInstanced[i]->WorldMatrix);
    }

    // Send data to the graphics card.
    D3D11_MAPPED_SUBRESOURCE InstancedGPUBuffer;
    Context->Map(ConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &InstancedGPUBuffer);
    memcpy(InstancedGPUBuffer.pData, &InstancedCBuff, sizeof(InstancedCBuff));
    Context->Unmap(ConstantBuffer, 0);

    // Set the culling mode based on the object.
    if (TempInstanced[0]->bDisableBackfaceCulling)
    {
        Context->RSSetState(RasterizerStateNoCull);
    }
    else
    {
        Context->RSSetState(RasterizerState);
    }

    // Vertex and Pixel Shader stages.
    Context->VSSetShader(InstancedTreeVS, 0, 0);
    Context->VSSetConstantBuffers(0, 1, { &ConstantBuffer });
    Context->PSSetSamplers(0, 1, &LinearSamplerState);
    Context->PSSetConstantBuffers(0, 1, { &ConstantBuffer });
    Context->PSSetShaderResources(0, 1, &TempInstanced[0]->ShaderResourceView);

    // Draw the indexed instanced objects.
    Context->DrawIndexedInstanced(TempInstanced[0]->Indices.size(), 100, 0, 0, 0);
    */
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
    SAFE_RELEASE(PIPTexture);
    SAFE_RELEASE(PIP_PixelShader);
    SAFE_RELEASE(PIP_RenderTargetView);
    SAFE_RELEASE(PIP_ShaderResourceView);
    SAFE_RELEASE(PIP_ZBuffer);
    SAFE_RELEASE(PIP_ZBufferView);
    SAFE_RELEASE(RasterizerState);
    SAFE_RELEASE(RasterizerStateNoCull);
    SAFE_RELEASE(ZBuffer);
    SAFE_RELEASE(ZBufferView);
    SAFE_RELEASE(SwapChain);
    SAFE_RELEASE(Context);

    // Find memory leak
    ID3D11Debug* DebugDevice = nullptr;
    HRESULT hr = Device->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&DebugDevice));
    if (SUCCEEDED(hr))
    {
        hr = DebugDevice->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
    }
    SAFE_RELEASE(DebugDevice);
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
            WorldObjects[i]->EndPlay();

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