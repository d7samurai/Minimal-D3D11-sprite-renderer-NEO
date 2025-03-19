
#pragma comment(lib, "user32")
#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")

///////////////////////////////////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>

#include "spritesheet.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

#define TITLE "Minimal D3D11 sprite renderer NEO by d7samurai"

///////////////////////////////////////////////////////////////////////////////////////////////////

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    WNDCLASSA wndclass = { 0, DefWindowProcA, 0, 0, 0, 0, 0, 0, 0, TITLE };

    RegisterClassA(&wndclass);

    HWND window = CreateWindowExA(0, TITLE, TITLE, WS_POPUP | WS_MAXIMIZE | WS_VISIBLE, 0, 0, 0, 0, nullptr, nullptr, nullptr, nullptr);

    ///////////////////////////////////////////////////////////////////////////////////////////////

    D3D_FEATURE_LEVEL featurelevels[] = { D3D_FEATURE_LEVEL_11_0 };

    DXGI_SWAP_CHAIN_DESC swapchaindesc = {};
    swapchaindesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    swapchaindesc.SampleDesc.Count  = 1;
    swapchaindesc.BufferUsage       = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapchaindesc.BufferCount       = 2;
    swapchaindesc.OutputWindow      = window;
    swapchaindesc.Windowed          = TRUE;
    swapchaindesc.SwapEffect        = DXGI_SWAP_EFFECT_FLIP_DISCARD;

    IDXGISwapChain* swapchain;

    ID3D11Device* device;
    ID3D11DeviceContext* devicecontext;

    D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, D3D11_CREATE_DEVICE_BGRA_SUPPORT, featurelevels, ARRAYSIZE(featurelevels), D3D11_SDK_VERSION, &swapchaindesc, &swapchain, &device, nullptr, &devicecontext);

    swapchain->GetDesc(&swapchaindesc);

    ///////////////////////////////////////////////////////////////////////////////////////////////

    ID3D11Texture2D* framebuffer;

    swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&framebuffer);

    D3D11_RENDER_TARGET_VIEW_DESC framebufferRTVdesc = {}; // needed for SRGB framebuffer when using FLIP model swap effect (line 39)
    framebufferRTVdesc.Format        = DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;
    framebufferRTVdesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

    ID3D11RenderTargetView* framebufferRTV;

    device->CreateRenderTargetView(framebuffer, &framebufferRTVdesc, &framebufferRTV);

    ///////////////////////////////////////////////////////////////////////////////////////////////

    float constants[4] = { 2.0f / swapchaindesc.BufferDesc.Width, -2.0f / swapchaindesc.BufferDesc.Height, 1.0f / SPRITESHEET_WIDTH, 1.0f / SPRITESHEET_HEIGHT };

    D3D11_BUFFER_DESC constantbufferdesc = {};
    constantbufferdesc.ByteWidth = sizeof(constants) + 0xf & 0xfffffff0;
    constantbufferdesc.Usage     = D3D11_USAGE_IMMUTABLE;
    constantbufferdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

    D3D11_SUBRESOURCE_DATA constantbufferSRD = { constants };

    ID3D11Buffer* constantbuffer;

    device->CreateBuffer(&constantbufferdesc, &constantbufferSRD, &constantbuffer);

    ///////////////////////////////////////////////////////////////////////////////////////////////

    D3D11_RASTERIZER_DESC rasterizerdesc = { D3D11_FILL_SOLID, D3D11_CULL_NONE }; // allow horizontal/vertical mirroring of sprite using negative scaling values

    ID3D11RasterizerState* rasterizerstate;

    device->CreateRasterizerState(&rasterizerdesc, &rasterizerstate);

    ///////////////////////////////////////////////////////////////////////////////////////////////

    D3D11_SAMPLER_DESC samplerdesc = { D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_BORDER, D3D11_TEXTURE_ADDRESS_BORDER, D3D11_TEXTURE_ADDRESS_BORDER };

    ID3D11SamplerState* samplerstate;

    device->CreateSamplerState(&samplerdesc, &samplerstate);

    ///////////////////////////////////////////////////////////////////////////////////////////////

    D3D11_BLEND_DESC blenddesc = { FALSE, FALSE, { TRUE, D3D11_BLEND_ONE, D3D11_BLEND_INV_SRC_ALPHA, D3D11_BLEND_OP_ADD, D3D11_BLEND_ZERO, D3D11_BLEND_ONE, D3D11_BLEND_OP_ADD, D3D11_COLOR_WRITE_ENABLE_ALL } };

    ID3D11BlendState* blendstate;

    device->CreateBlendState(&blenddesc, &blendstate);

    ///////////////////////////////////////////////////////////////////////////////////////////////

    ID3DBlob* vertexshaderCSO;

    D3DCompileFromFile(L"gpu.hlsl", 0, 0, "vs", "vs_5_0", 0, 0, &vertexshaderCSO, 0);

    ID3D11VertexShader* vertexshader;

    device->CreateVertexShader(vertexshaderCSO->GetBufferPointer(), vertexshaderCSO->GetBufferSize(), 0, &vertexshader);

    ///////////////////////////////////////////////////////////////////////////////////////////////

    ID3DBlob* pixelshaderCSO;

    D3DCompileFromFile(L"gpu.hlsl", 0, 0, "ps", "ps_5_0", 0, 0, &pixelshaderCSO, 0);

    ID3D11PixelShader* pixelshader;

    device->CreatePixelShader(pixelshaderCSO->GetBufferPointer(), pixelshaderCSO->GetBufferSize(), 0, &pixelshader);

    ///////////////////////////////////////////////////////////////////////////////////////////////

    FLOAT clearcolor[4] = { 0.1337f, 0.1337f, 0.1337f, 1.0f }; // RGBA

    D3D11_VIEWPORT viewport = { 0, 0, (float)swapchaindesc.BufferDesc.Width, (float)swapchaindesc.BufferDesc.Height, 0, 1 };

    ///////////////////////////////////////////////////////////////////////////////////////////////

    D3D11_TEXTURE2D_DESC spritesheetdesc = {};
    spritesheetdesc.Width            = SPRITESHEET_WIDTH;
    spritesheetdesc.Height           = SPRITESHEET_HEIGHT;
    spritesheetdesc.MipLevels        = 1;
    spritesheetdesc.ArraySize        = 1;
    spritesheetdesc.Format           = DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;
    spritesheetdesc.SampleDesc.Count = 1;
    spritesheetdesc.Usage            = D3D11_USAGE_IMMUTABLE;
    spritesheetdesc.BindFlags        = D3D11_BIND_SHADER_RESOURCE;

    D3D11_SUBRESOURCE_DATA spritesheetSRD = {};
    spritesheetSRD.pSysMem     = spritesheetdata;
    spritesheetSRD.SysMemPitch = SPRITESHEET_WIDTH * sizeof(UINT);

    ID3D11Texture2D* spritesheet;

    device->CreateTexture2D(&spritesheetdesc, &spritesheetSRD, &spritesheet);

    ID3D11ShaderResourceView* spritesheetSRV;

    device->CreateShaderResourceView(spritesheet, nullptr, &spritesheetSRV);

    ///////////////////////////////////////////////////////////////////////////////////////////////

    #define MAX_SPRITES 65536 // arbitrary limit

    struct float2     { float x, y; };
    struct spritedesc { float2 location, size, anchor, position, scale; float rotation; int color; };

    ///////////////////////////////////////////////////////////////////////////////////////////////

    D3D11_BUFFER_DESC spritebufferdesc = {};
    spritebufferdesc.ByteWidth           = sizeof(spritedesc) * MAX_SPRITES;
    spritebufferdesc.Usage               = D3D11_USAGE_DYNAMIC;
    spritebufferdesc.BindFlags           = D3D11_BIND_SHADER_RESOURCE;
    spritebufferdesc.CPUAccessFlags      = D3D11_CPU_ACCESS_WRITE;
    spritebufferdesc.MiscFlags           = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
    spritebufferdesc.StructureByteStride = sizeof(spritedesc);

    ID3D11Buffer* spritebuffer;

    device->CreateBuffer(&spritebufferdesc, nullptr, &spritebuffer);

    D3D11_SHADER_RESOURCE_VIEW_DESC spritebufferSRVdesc = {};
    spritebufferSRVdesc.Format             = DXGI_FORMAT_UNKNOWN;
    spritebufferSRVdesc.ViewDimension      = D3D11_SRV_DIMENSION_BUFFER;
    spritebufferSRVdesc.Buffer.NumElements = MAX_SPRITES;

    ID3D11ShaderResourceView* spritebufferSRV;

    device->CreateShaderResourceView(spritebuffer, &spritebufferSRVdesc, &spritebufferSRV);

    ///////////////////////////////////////////////////////////////////////////////////////////////

    spritedesc* spritebatch = (spritedesc*)HeapAlloc(GetProcessHeap(), 0, sizeof(spritedesc) * MAX_SPRITES);

    ///////////////////////////////////////////////////////////////////////////////////////////////

    while (true)
    {
        MSG msg;

        while (PeekMessageA(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_KEYDOWN) return 0; // PRESS ANY KEY TO EXIT
            DispatchMessageA(&msg);
        }

        ///////////////////////////////////////////////////////////////////////////////////////////

        static int   tick = 0;
        static float rotation = 0.0f;

        int spritecount = 0; // start a new batch every frame

        int frame = tick++ / 6 % ANIMATION_FRAMES; // animation running at 1/6 refresh rate
        float2 cell = { (float)(frame % SPRITESHEET_COLUMNS), (float)(frame / SPRITESHEET_COLUMNS) }; // map animation frame # to sprite sheet cell (x, y)

        spritedesc sprite;

        sprite.location = { cell.x * CELL_WIDTH, cell.y * CELL_HEIGHT }; // texture location in sprite sheet
        sprite.size     = { SPRITE_WIDTH, SPRITE_HEIGHT };               // texture size in sprite sheet
        sprite.anchor   = { 0.5, 2.0f };                                 // anchor point relative to texture bounds
        sprite.position = { viewport.Width / 2, viewport.Height / 2};    // sprite position on screen
        sprite.scale    = { 4, 4 };                                      // sprite fat-pixel size
        sprite.rotation =   rotation;                                    // in radians
        sprite.color    = 0xffffffff;                                    // 0xAARRGGBB

        spritebatch[spritecount++] = sprite;                             // add sprite to batch

        rotation += 0.00873f;

        ///////////////////////////////////////////////////////////////////////////////////////////

        D3D11_MAPPED_SUBRESOURCE spritebufferMSR;

        devicecontext->Map(spritebuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &spritebufferMSR);
        {
            memcpy(spritebufferMSR.pData, spritebatch, sizeof(spritedesc) * spritecount); // upload sprite batch to gpu
        }
        devicecontext->Unmap(spritebuffer, 0);

        ///////////////////////////////////////////////////////////////////////////////////////////

        devicecontext->OMSetRenderTargets(1, &framebufferRTV, nullptr);

        devicecontext->ClearRenderTargetView(framebufferRTV, clearcolor);

        devicecontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

        devicecontext->VSSetShader(vertexshader, nullptr, 0);
        devicecontext->VSSetShaderResources(0, 1, &spritebufferSRV);
        devicecontext->VSSetConstantBuffers(0, 1, &constantbuffer);

        devicecontext->RSSetViewports(1, &viewport);
        devicecontext->RSSetState(rasterizerstate);

        devicecontext->PSSetShader(pixelshader, nullptr, 0);
        devicecontext->PSSetShaderResources(1, 1, &spritesheetSRV);
        devicecontext->PSSetConstantBuffers(0, 1, &constantbuffer);
        devicecontext->PSSetSamplers(0, 1, &samplerstate);

        devicecontext->OMSetBlendState(blendstate, nullptr, 0xffffffff);

        ///////////////////////////////////////////////////////////////////////////////////////////

        devicecontext->DrawInstanced(4, spritecount, 0, 0);

        ///////////////////////////////////////////////////////////////////////////////////////////

        swapchain->Present(1, 0);
    }
}
