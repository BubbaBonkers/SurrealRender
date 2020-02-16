// D3D11_SurrealRender.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "D3D11_SurrealRender.h"

// Include the DirectX libraries.
#include "d3d11.h"
// Tell the linker about the d3d11 library to include it as if it were from the source file.
#pragma comment(lib, "d3d11.lib")

#include <d3dcompiler.h>

// Custom library includes.
#include "DisplayAgent.h"       // Holds *Device, *SwapChain, *Context, *RenderTargetView, and Viewport.
#include "NotReallyBasics.h"    // Some basic functions and structs for math and color type things.
#include "Timer.h"				// Timer for things that need it.
#include "NotLights.h"          // Lighting objects to emit light, duh!

// Shader files.
//#include "GeneralVertexShaders.csh"
#include "GeneralPixelShaders.csh"
#include "GeneralMeshVertexShaders.csh"

using namespace NRB;
using namespace DirectX;

// For debug console.
FILE* conout;
FILE* conerr;

// Holds *Device, *SwapChain, *Context, *RenderTargetView, and Viewport.
DisplayAgent* MainDisplay = nullptr;
Timer Clock;

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // Instance of the window created for the viewport.
WCHAR szTitle[MAX_LOADSTRING];                  // Text in the title bar.
WCHAR szWindowClass[MAX_LOADSTRING];            // Name of the main window class.

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

HRESULT CompileShaderFromFile(const WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.
    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_D3D11SURREALRENDER, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Create the display agent.
    MainDisplay = new DisplayAgent();    

    // Perform application initialization:
    // Attempt to initialize the hInstance of the window, if failed, return a false value for the app.
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    // Load the accelerator for user input and key events.
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_D3D11SURREALRENDER));

    // Create the message structure.
    MSG msg;


    // Start internal timer.
    Clock.Start();

    // Main message loop:
    while (true)
    {
        // Update timer for clock.
        float DeltaTime = (float)(Clock.GetElapsedMiliseconds() * 0.001f);
        Clock.Restart();

        MainDisplay->Update(DeltaTime);

        // Peek at a message in the queue. 
        //Format: message (MSG Structure), the window to peek at the messages from (nullptr is all in current thread), first message in range, last message in range (both can also retrieve last mouse or last key input specifically).
        PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE);

        // Check if allowed to Dispatch Messages.
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        // Check if the lParam value is WM_QUIT and exit the loop if it is.
        if (msg.message == WM_QUIT)
        {
            break;
        }

        MainDisplay->PresentFromRenderTarget(MainDisplay->WorldCameras[0], MainDisplay->WorldObjects[0], DeltaTime);
    }

    // Release all used D3D interfaces.
    MainDisplay->EndPlay();

    return (int) msg.wParam;
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    // Create the window class variable.
    WNDCLASSEXW wcex;

    // Set window class size of structure.
    wcex.cbSize = sizeof(WNDCLASSEX);

    // Setup remaining window class data members.
    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_D3D11SURREALRENDER));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_D3D11SURREALRENDER);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    // Return the validity of the newly (attempted) registered window class.
    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

    // Create the window for DirectX to display through.
    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPED | WS_SYSMENU | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SIZEBOX,
    CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);
    // Hide the menu bar with File options.
    SetMenu(hWnd, NULL);

    // For debug console.
    AllocConsole();
    freopen_s(&conout, "CONOUT$", "w", stdout);
    freopen_s(&conerr, "CONOUT$", "w", stderr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   RECT WindowRectangle;
   GetClientRect(hWnd, &WindowRectangle);

   // Attach DirectX 11 to the window.
   D3D_FEATURE_LEVEL dx11 = D3D_FEATURE_LEVEL_11_0;     // Set the allowed feature level to only DirextX 11.
   DXGI_SWAP_CHAIN_DESC Swap;                           // Swap Chain being sent to the CreateDeviceAndSwapChain function.
   ZeroMemory(&Swap, sizeof(DXGI_SWAP_CHAIN_DESC));     // Clear the Swap variable out and set all of the values to zero; avoid garbage values.

   // Fill out Swap structure information.
   Swap.BufferCount = 1;
   Swap.OutputWindow = hWnd;
   Swap.Windowed = true;
   Swap.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
   Swap.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
   Swap.BufferDesc.Width = WindowRectangle.right - WindowRectangle.left;
   Swap.BufferDesc.Height = WindowRectangle.bottom - WindowRectangle.top;
   Swap.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
   Swap.SampleDesc.Count = 8;
   Swap.SampleDesc.Quality = 0;

   // Set aspect ratio for world and cameras.
   MainDisplay->ChangeAspectRatio((float)Swap.BufferDesc.Width / (float)Swap.BufferDesc.Height);

   UINT flags = D3D11_CREATE_DEVICE_SINGLETHREADED;
#ifdef _DEBUG
   flags = flags | D3D11_CREATE_DEVICE_DEBUG;
#endif

   // Create and set the Device and SwapChain variables using DirectX.
   HRESULT hr;
   hr = D3D11CreateDeviceAndSwapChain(  nullptr, D3D_DRIVER_TYPE_HARDWARE, NULL, D3D11_CREATE_DEVICE_DEBUG, 
                                        &dx11, 1, D3D11_SDK_VERSION, &Swap, &MainDisplay->SwapChain, &MainDisplay->Device, 0, &MainDisplay->Context);

   ID3D11Resource* BackBuffer = nullptr;                                                                          // Create the Back Buffer resource in DirectX.
   hr = MainDisplay->SwapChain->GetBuffer(0, __uuidof(BackBuffer), (void**)&BackBuffer);                 // Get the Buffer through the Swap Chain.
   hr = MainDisplay->Device->CreateRenderTargetView(BackBuffer, NULL, &MainDisplay->RenderTargetView);    // Create the RenterTargetView for the Device.
   BackBuffer->Release();                                                                               // Remove one from the internal count of the Device.

   // Create Z-Buffer and View. Multisampling/Antialiasing can be done here.
   D3D11_TEXTURE2D_DESC ZDesc;
   ZeroMemory(&ZDesc, sizeof(ZDesc));
   ZDesc.ArraySize = 1;
   ZDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
   ZDesc.Width = Swap.BufferDesc.Width;
   ZDesc.Height = Swap.BufferDesc.Height;
   ZDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
   ZDesc.Usage = D3D11_USAGE_DEFAULT;
   ZDesc.CPUAccessFlags = 0;
   ZDesc.MipLevels = 1;
   ZDesc.SampleDesc.Count = 8;
   ZDesc.SampleDesc.Quality = 0;
   hr = MainDisplay->Device->CreateTexture2D(&ZDesc, nullptr, &MainDisplay->ZBuffer);

   // Create the depth stencil view
   D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
   descDSV.Format = ZDesc.Format;
   descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
   descDSV.Texture2D.MipSlice = 0;
   MainDisplay->Device->CreateDepthStencilView(MainDisplay->ZBuffer, nullptr, &MainDisplay->ZBufferView);

   // Setup the Viewport.
   MainDisplay->Viewport.Width = (float)Swap.BufferDesc.Width;
   MainDisplay->Viewport.Height = (float)Swap.BufferDesc.Height;
   MainDisplay->Viewport.TopLeftX = MainDisplay->Viewport.TopLeftY = 0;
   MainDisplay->Viewport.MinDepth = 0;
   MainDisplay->Viewport.MaxDepth = 1;

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
   hr = MainDisplay->Device->CreateSamplerState(&sampDesc, &MainDisplay->LinearSamplerState);

   // Load the new mesh shader.
   hr = MainDisplay->Device->CreateVertexShader(GeneralMeshVertexShaders, sizeof(GeneralMeshVertexShaders), nullptr, &MainDisplay->MeshVertexShader);

   // Write, Compile, and Load the shaders.
   hr = MainDisplay->Device->CreatePixelShader(GeneralPixelShaders, sizeof(GeneralPixelShaders), nullptr, &MainDisplay->PixelShader);

   // Describe it to DirectX.
   D3D11_INPUT_ELEMENT_DESC InputDesc[] =
   {
       {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
       {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
       {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
   };

   hr = MainDisplay->Device->CreateInputLayout(InputDesc, 3, GeneralMeshVertexShaders, sizeof(GeneralMeshVertexShaders), &MainDisplay->InputLayout);

   MainDisplay->Context->IASetInputLayout(MainDisplay->InputLayout);                         // Input assembler.

   /*
   D3D11_BLEND_DESC BlendDescState;
   D3D11_RENDER_TARGET_BLEND_DESC BlendState;
   ZeroMemory(&BlendState, sizeof(D3D10_BLEND_DESC));

   BlendState.BlendEnable = TRUE;
   BlendState.SrcBlend = D3D11_BLEND_SRC_ALPHA;
   BlendState.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
   BlendState.BlendOp = D3D11_BLEND_OP_ADD;
   BlendState.SrcBlendAlpha = D3D11_BLEND_ZERO;
   BlendState.DestBlendAlpha = D3D11_BLEND_ZERO;
   BlendState.BlendOpAlpha = D3D11_BLEND_OP_ADD;
   BlendState.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

   BlendDescState.AlphaToCoverageEnable = TRUE;
   BlendDescState.IndependentBlendEnable = TRUE;
   BlendDescState.RenderTarget[0] = BlendState;

   MainDisplay->Device->CreateBlendState(&BlendDescState, &MainDisplay->BlendState);
   */

   // Create the constant buffer.
   D3D11_BUFFER_DESC BufferDescription;
   ZeroMemory(&BufferDescription, sizeof(BufferDescription));

   BufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
   BufferDescription.ByteWidth = sizeof(DisplayAgent::ConstBuffer);
   BufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
   BufferDescription.MiscFlags = 0;
   BufferDescription.StructureByteStride = 0;
   BufferDescription.Usage = D3D11_USAGE_DYNAMIC;

   // Create the Buffer to put the model on.
   hr = MainDisplay->Device->CreateBuffer(&BufferDescription, nullptr, &MainDisplay->ConstantBuffer);

   // Load the texture into the graphics card.
   D3D11_TEXTURE2D_DESC TextureDesc;
   D3D11_SUBRESOURCE_DATA TextureSource;
   ZeroMemory(&TextureDesc, sizeof(TextureDesc));
   ZeroMemory(&TextureSource, sizeof(TextureSource));

   TextureDesc.ArraySize = 1;
   TextureDesc.MipLevels = 1;
   TextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
   TextureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;
   TextureDesc.Height = 256;
   TextureDesc.Width = 256;
   TextureDesc.SampleDesc.Count = 1;
   TextureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
   TextureDesc.MiscFlags = 0;

   MainDisplay->Device->CreateTexture2D(&TextureDesc, nullptr, &MainDisplay->DiffuseTexture);

   // Simple Testing Cube.
   Object* CubeTest = MainDisplay->CreateObject("TestingCube", "Assets/cube.mesh", "Assets/Crate.dds");
   Object* MultipleObjectTest = MainDisplay->CreateObject("SecondCube", "Assets/FancyBox.mesh", "Assets/FancyBoxDDS.dds");
   MultipleObjectTest->AddMovementInput(300.0f, -50.0f, 55.0f, true);
   MultipleObjectTest->AddRotationInput(-5.0f, 3.0f, 0.0f, true);

   // Waving Cube.
   Object* WavingCube = MainDisplay->CreateObject("PlanetWave", "Assets/Planet1.mesh", "Assets/Planet1.dds");
   WavingCube->AddMovementInput(1600.0f, 100.0f, -1600.0f, true);
   WavingCube->Scale(2.0f, 2.0f, 2.0f);
   WavingCube->WavingIntensity = 1.0f;

   // Disco Cube.
   Object* StaticSinWaveCube = MainDisplay->CreateObject("TestingCubeWave", "Assets/cube.mesh", "Assets/Crate.dds");
   StaticSinWaveCube->DiscoIntensity = 1.0f;
   StaticSinWaveCube->AddMovementInput(-300.0f, 110.0f, -250.0f, true);

   // Testing Torch Mesh.
   Object* Torch = MainDisplay->CreateObject("TorchTest", "Assets/Torch.mesh", "Assets/TorchTexture.dds");
   Torch->Scale(30.0f, 30.0f, 30.0f);
   Torch->AddMovementInput(-250.0f, 150.0f, 150.0f, true);

   // Testing Bamboo Mesh.
   Object* Bamboo = MainDisplay->CreateObject("Bamboo", "Assets/Bamboo.mesh", "Assets/BambooT.dds");
   Bamboo->Scale(30.0f, 30.0f, 30.0f);
   Bamboo->AddMovementInput(250.0f, 50.0f, -150.0f, true);

   // Camera Acting as Eyes.
   Camera* MainCamera = MainDisplay->CreateCamera("Eyes");
   MainCamera->AddMovementInput(0, 3.0f, -5.0f);
   MainCamera->AddRotationInput(-0.45f, 0.0f, 0.0f, true, true);
   MainCamera->LookAtTarget = MultipleObjectTest;

   // Testing Emmisive Material for Mesh.
   Object* EmissiveCube = MainDisplay->CreateObject("EmissiveObject", "Assets/cube.mesh", "Assets/Crate.dds");
   EmissiveCube->EmissiveColor = { 3, 3, 3, 3 };
   EmissiveCube->AddMovementInput(250.0f, -150.0f, -150.0f, true);

   // Directional Light.
   MainDisplay->CreateDirectionalLight("Sunlight", { 1, 1, 1, 1 }, { 1, 1, 1, 1 }, 1.0f);

   // Dynamic Point Light.
   MainDisplay->CreatePointLight("PointLightTest", { 0, 1, 1, 1 }, 1.0f);
   XMStoreFloat4x4(&MainDisplay->WorldPointLights[0]->WorldMatrix, XMMatrixMultiply(XMMatrixTranslation(13.0f, 15.0f, 0.0f), XMLoadFloat4x4(&MainDisplay->WorldPointLights[0]->WorldMatrix)));

   // Static Point Light.
   MainDisplay->CreatePointLight("StaticPointLight", { 0, 0, 1.0f, 1 }, 1.0f);
   XMStoreFloat4x4(&MainDisplay->WorldPointLights[1]->WorldMatrix, XMMatrixMultiply(XMMatrixTranslation(0.0f, -25.0f, -50.0f), XMLoadFloat4x4(&MainDisplay->WorldPointLights[1]->WorldMatrix)));

   // SimpleSpot Light Test.
   MainDisplay->CreateSpotLight("SpotLightTest", { 1, 0, 0, 1 }, 1.0f);
   XMMATRIX LookNew = XMMatrixLookAtLH({ MainDisplay->WorldSpotLights[0]->WorldMatrix._41, MainDisplay->WorldSpotLights[0]->WorldMatrix._42, MainDisplay->WorldSpotLights[0]->WorldMatrix._43 }, { MainDisplay->WorldObjects[2]->WorldMatrix._41, MainDisplay->WorldObjects[2]->WorldMatrix._42, MainDisplay->WorldObjects[2]->WorldMatrix._43 }, { 0, 1, 0 });
   XMStoreFloat4x4(&MainDisplay->WorldSpotLights[0]->WorldMatrix, LookNew);
   //MainDisplay->WorldSpotLights[0]->AddMovementInput(350.0f, -50.0f, 55.0f, true);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_SIZE:
    {
        INT NewWidth = LOWORD(lParam);
        INT NewHeight = HIWORD(lParam);

        MainDisplay->ChangeAspectRatio((float)NewWidth / (float)NewHeight);
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}


HRESULT CompileShaderFromFile(const WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
{
    HRESULT hr = S_OK;

    DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
    // Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
    // Setting this flag improves the shader debugging experience, but still allows 
    // the shaders to be optimized and to run exactly the way they will run in 
    // the release configuration of this program.
    dwShaderFlags |= D3DCOMPILE_DEBUG;

    // Disable optimizations to further improve shader debugging
    dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

    ID3DBlob* pErrorBlob = nullptr;
    hr = D3DCompileFromFile(szFileName, nullptr, nullptr, szEntryPoint, szShaderModel,
        dwShaderFlags, 0, ppBlobOut, &pErrorBlob);
    if (FAILED(hr))
    {
        if (pErrorBlob)
        {
            OutputDebugStringA(reinterpret_cast<const char*>(pErrorBlob->GetBufferPointer()));
            pErrorBlob->Release();
        }
        return hr;
    }
    if (pErrorBlob) pErrorBlob->Release();

    return S_OK;
}