// D3D11_SurrealRender.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "D3D11_SurrealRender.h"

// Include the DirectX libraries.
#include "d3d11.h"
// Tell the linker about the d3d11 library to include it as if it were from the source file.
#pragma comment(lib, "d3d11.lib")

// Custom library includes.
#include "DisplayAgent.h"       // Holds *Device, *SwapChain, *Context, *RenderTargetView, and Viewport.
#include "NotReallyBasics.h"    // Some basic functions and structs for math and color type things.
#include "GeneralVertexShaders.csh"
#include "GeneralPixelShaders.csh"

using namespace NRB;
using namespace DirectX;

// Holds *Device, *SwapChain, *Context, *RenderTargetView, and Viewport.
DisplayAgent MainDisplay;

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

    // Main message loop:
    while (true)    // PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
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

        // This function sets up the render targets and presents the image.
        MainDisplay.PresentFromRenderTarget();
    }

    // Release all used D3D interfaces.
    MainDisplay.ReleaseInterfaces();

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
   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPED | WS_BORDER | WS_SYSMENU,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);
   // Hide the menu bar with File options.
   SetMenu(hWnd, NULL);

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
   Swap.SampleDesc.Count = 1;

   MainDisplay.AspectRatio = (Swap.BufferDesc.Width / Swap.BufferDesc.Height);

   UINT flags = D3D11_CREATE_DEVICE_SINGLETHREADED;
#ifdef _DEBUG
   flags = flags | D3D11_CREATE_DEVICE_DEBUG;
#endif

   // Create and set the Device and SwapChain variables using DirectX.
   HRESULT hr;
   hr = D3D11CreateDeviceAndSwapChain(  nullptr, D3D_DRIVER_TYPE_HARDWARE, NULL, D3D11_CREATE_DEVICE_DEBUG, 
                                        &dx11, 1, D3D11_SDK_VERSION, &Swap, &MainDisplay.SwapChain, &MainDisplay.Device, 0, &MainDisplay.Context);

   ID3D11Resource* BackBuffer;                                                                          // Create the Back Buffer resource in DirectX.
   hr = MainDisplay.SwapChain->GetBuffer(0, __uuidof(BackBuffer), (void**)&BackBuffer);                 // Get the Buffer through the Swap Chain.
   hr = MainDisplay.Device->CreateRenderTargetView(BackBuffer, NULL, &MainDisplay.RenderTargetView);    // Create the RenterTargetView for the Device.
   BackBuffer->Release();                                                                               // Remove one from the internal count of the Device.

   // Setup the Viewport.
   MainDisplay.Viewport.Width = Swap.BufferDesc.Width;
   MainDisplay.Viewport.Height = Swap.BufferDesc.Height;
   MainDisplay.Viewport.TopLeftX = MainDisplay.Viewport.TopLeftY = 0;
   MainDisplay.Viewport.MinDepth = 0;
   MainDisplay.Viewport.MaxDepth = 1;

   // Vertex data for the triangle.
   Vertex Triangle[] =
   {
       // Front Face.
       { { 0, 1.0f, 0, 1 }, { 1, 1, 1, 1 } },
       { { 0.25f, -0.25f, -0.25f, 1 }, { 1, 0, 1, 1 } },
       { { -0.25f, -0.25f, -0.25f, 1 }, { 1, 1, 0, 1 } },

       // Right Face.
       { { 0, 1.0f, 0, 1 }, { 1, 1, 1, 1 } },
       { { 0.25f, -0.25f, 0.25f, 1 }, { 1, 0, 1, 1 } },
       { { 0.25f, -0.25f, -0.25f, 1 }, { 1, 1, 0, 1 } },

       // Back Face.
       { { 0, 1.0f, 0, 1 }, { 1, 1, 1, 1 } },
       { { -0.25f, -0.25f, 0.25f, 1 }, { 1, 0, 1, 1 } },
       { { 0.25f, -0.25f, 0.25f, 1 }, { 1, 1, 0, 1 } },

       // Left Face.
       { { 0, 1.0f, 0, 1 }, { 1, 1, 1, 1 } },
       { { -0.25f, -0.25f, -0.25f, 1 }, { 1, 0, 1, 1 } },
       { { -0.25f, -0.25f, 0.25f, 1 }, { 1, 1, 0, 1 } }
   };

   // Set couter for number of Vertices in the current item being drawn.
   MainDisplay.VertexCount = ARRAYSIZE(Triangle);

   // Load the Triangle onto the card.
   D3D11_BUFFER_DESC BufferDescription;
   D3D11_SUBRESOURCE_DATA SubData;
   ZeroMemory(&BufferDescription, sizeof(BufferDescription));
   ZeroMemory(&SubData, sizeof(SubData));

   BufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;
   BufferDescription.ByteWidth = sizeof(Vertex) * MainDisplay.VertexCount;
   BufferDescription.CPUAccessFlags = 0;
   BufferDescription.MiscFlags = 0;
   BufferDescription.StructureByteStride = 0;
   BufferDescription.Usage = D3D11_USAGE_IMMUTABLE;

   SubData.pSysMem = Triangle;

   // Create the Buffer to put the model on.
   hr = MainDisplay.Device->CreateBuffer(&BufferDescription, &SubData, &MainDisplay.VertexBuffer);

   // Write, Compile, and Load the shaders.
   hr = MainDisplay.Device->CreateVertexShader(GeneralVertexShaders, sizeof(GeneralVertexShaders), nullptr, &MainDisplay.VertexShader);
   hr = MainDisplay.Device->CreatePixelShader(GeneralPixelShaders, sizeof(GeneralPixelShaders), nullptr, &MainDisplay.PixelShader);

   // Describe it to DirectX.
   D3D11_INPUT_ELEMENT_DESC InputDesc[] =
   {
       {"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
       {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0},
   };

   hr = MainDisplay.Device->CreateInputLayout(InputDesc, 2, GeneralVertexShaders, sizeof(GeneralVertexShaders), &MainDisplay.InputLayout);

   // Create the constant buffer.
   ZeroMemory(&BufferDescription, sizeof(BufferDescription));

   BufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
   BufferDescription.ByteWidth = sizeof(DisplayAgent::Environment);
   BufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
   BufferDescription.MiscFlags = 0;
   BufferDescription.StructureByteStride = 0;
   BufferDescription.Usage = D3D11_USAGE_DYNAMIC;

   // Create the Buffer to put the model on.
   hr = MainDisplay.Device->CreateBuffer(&BufferDescription, nullptr, &MainDisplay.ConstantBuffer);

   /*
   // Load the complex mesh onto the video card.
   BufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;
   BufferDescription.ByteWidth = sizeof();
   BufferDescription.CPUAccessFlags = 0;
   BufferDescription.MiscFlags = 0;
   BufferDescription.StructureByteStride = 0;
   BufferDescription.Usage = D3D11_USAGE_IMMUTABLE;
   */

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