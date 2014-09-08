#include "precompiled.h"

#include "winrtangle.h"
#include <windows.h>
#include <crtdbg.h>

using namespace Microsoft::WRL;

#define TRACE OutputDebugString
#define ASSERT _ASSERTE

class WinPhone8XamlD3DWindow : public RuntimeClass<RuntimeClassFlags<ClassicCom>, IWinPhone8XamlD3DWindow>
{
public:
    WinPhone8XamlD3DWindow() 
        : m_device(nullptr)
        , m_context(nullptr)
        , m_renderTarget(nullptr)
    {
        TRACE((LPCWSTR)"WinPhone8XamlD3DWindow()\n");
    }

    ~WinPhone8XamlD3DWindow()
    {
        TRACE((LPCWSTR)"~WinPhone8XamlD3DWindow()\n");
    }

    //
    // IWinPhone8XamlD3DWindow
    //

    virtual ComPtr<IUnknown> __stdcall GetDevice() override
    {
        return m_device;
    }

    virtual ComPtr<IUnknown> __stdcall GetContext() override
    {
        return m_context;
    }

    virtual ComPtr<IUnknown> __stdcall GetRenderTarget() override
    {
        return m_renderTarget;
    }

    virtual void __stdcall Update(ComPtr<IUnknown> device, ComPtr<IUnknown> context, ComPtr<IUnknown> renderTarget) override
    {
        m_device = device;
        m_context = context;
        m_renderTarget = renderTarget;
    }
    
private:
    ComPtr<IUnknown> m_device;
    ComPtr<IUnknown> m_context;
    ComPtr<IUnknown> m_renderTarget;
};

HRESULT __stdcall CreateWinPhone8XamlWindow(IWinPhone8XamlD3DWindow ** result)
{
    ASSERT(result);
    *result = nullptr;

    ComPtr<IWinPhone8XamlD3DWindow> iWindow = Make<WinPhone8XamlD3DWindow>();

    if (!iWindow)
    {
        return E_OUTOFMEMORY;
    }

    *result = iWindow.Detach();
    return S_OK;
}

class WinrtEglWindow : public RuntimeClass<RuntimeClassFlags<ClassicCom>, IWinrtEglWindow, IWinrtEglWindowDimensions>
{
public:
    WinrtEglWindow(ComPtr<IUnknown> windowInterface, ANGLE_D3D_FEATURE_LEVEL featureLevel) 
        : m_windowInterface(windowInterface)
        , m_featureLevel(featureLevel)
        , m_angleD3DDevice(nullptr)
        , m_width(1)
        , m_height(1)
    {
        TRACE((LPCWSTR)"WinrtEglWindow()\n");
    }

    WinrtEglWindow(ComPtr<IUnknown> windowInterface, ANGLE_D3D_FEATURE_LEVEL featureLevel, unsigned int width, unsigned int height)
        : m_windowInterface(windowInterface)
        , m_featureLevel(featureLevel)
        , m_angleD3DDevice(nullptr)
        , m_width(width)
        , m_height(height)
    {
        TRACE((LPCWSTR)"WinrtEglWindow()\n");
    }

    ~WinrtEglWindow()
    {
        TRACE((LPCWSTR)"~WinrtEglWindow()\n");
    }

    //
    // IWinrtEglWindow
    //

    virtual ANGLE_D3D_FEATURE_LEVEL __stdcall GetAngleD3DFeatureLevel() override
    {
        return m_featureLevel;
    }

    virtual ComPtr<IUnknown> __stdcall GetWindowInterface() override
    {
        return m_windowInterface;
    }

    virtual Microsoft::WRL::ComPtr<IUnknown> __stdcall GetAngleD3DDevice() override
    {
		return m_angleD3DDevice;
    }
 
    virtual void __stdcall SetAngleD3DDevice(Microsoft::WRL::ComPtr<IUnknown> device) override
    {
		m_angleD3DDevice = device;
    }

    virtual Microsoft::WRL::ComPtr<IUnknown> __stdcall GetAngleSwapChain() override
    {
		return m_angleSwapChain;
    }
    
    virtual void __stdcall SetAngleSwapChain(Microsoft::WRL::ComPtr<IUnknown> swapChain) override
    {
		m_angleSwapChain = swapChain;
    }

    virtual void __stdcall SetWindowDimensions(int width, int height)
    {
        m_width = width;
        m_height = height;
    }

    virtual void __stdcall GetWindowDimensions(int& width, int& height)
    {
        width = m_width;
        height = m_height;
    }

private:
    ANGLE_D3D_FEATURE_LEVEL m_featureLevel;
    ComPtr<IUnknown> m_windowInterface;
    ComPtr<IUnknown> m_angleD3DDevice;
    ComPtr<IUnknown> m_angleSwapChain;
    int m_width;
    int m_height;
};

HRESULT __stdcall CreateWinrtEglWindow(ComPtr<IUnknown> windowInterface, ANGLE_D3D_FEATURE_LEVEL featureLevel, IWinrtEglWindow ** result)
{
    ASSERT(result);
    *result = nullptr;

    ComPtr<IWinrtEglWindow> iWindow = Make<WinrtEglWindow>(windowInterface, featureLevel);

    if (!iWindow)
    {
        return E_OUTOFMEMORY;
    }

    *result = iWindow.Detach();
    return S_OK;
}

HRESULT __stdcall CreateWinrtEglWindowWithDimensions(ComPtr<IUnknown> windowInterface, ANGLE_D3D_FEATURE_LEVEL featureLevel, unsigned int width, unsigned int height, IWinrtEglWindow ** result)
{
    ASSERT(result);
    *result = nullptr;

    ComPtr<IWinrtEglWindow> iWindow = Make<WinrtEglWindow>(windowInterface, featureLevel, width, height);

    if (!iWindow)
    {
        return E_OUTOFMEMORY;
    }

    *result = iWindow.Detach();
    return S_OK;
}






