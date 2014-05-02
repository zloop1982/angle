#include "precompiled.h"
#include "libGLESv2/renderer/d3d/HLSLCompiler.h"
#include "libGLESv2/Program.h"
#include "libGLESv2/main.h"

#include "common/utilities.h"

#include "third_party/trace_event/trace_event.h"

namespace rx
{

HLSLCompiler::HLSLCompiler()
    : mD3DCompilerModule(NULL),
      mD3DCompileFunc(NULL),
	  mHasCompiler(true)
{
}

HLSLCompiler::~HLSLCompiler()
{
    release();
}

bool HLSLCompiler::initialize()
{
    TRACE_EVENT0("gpu", "initializeCompiler");
#if defined(ANGLE_PLATFORM_WP8)
    mD3DCompilerModule = NULL;
    mD3DCompileFunc = NULL;
    mHasCompiler = false;
    return true;
#endif //#if defined(ANGLE_PLATFORM_WP8)

#if defined(ANGLE_PLATFORM_WINRT) && (_MSC_VER >= 1800) && !defined(ANGLE_PLATFORM_WP8)
    ERR("No D3D compiler module available - must use precompiled shaders\n");
    mD3DCompilerModule = NULL;
    mD3DCompileFunc = reinterpret_cast<CompileFuncPtr>(D3DCompile);
    mHasCompiler = true;
    return true;
#endif // #if defined(ANGLE_PLATFORM_WINRT) && (_MSC_VER >= 1800)

#if defined(ANGLE_PRELOADED_D3DCOMPILER_MODULE_NAMES)
    // Find a D3DCompiler module that had already been loaded based on a predefined list of versions.
    static TCHAR* d3dCompilerNames[] = ANGLE_PRELOADED_D3DCOMPILER_MODULE_NAMES;

    for (size_t i = 0; i < ArraySize(d3dCompilerNames); ++i)
    {
        if (GetModuleHandleEx(0, d3dCompilerNames[i], &mD3DCompilerModule))
        {
            break;
        }
    }
    if(!mD3DCompilerModule)
    {
        mD3DCompilerModule = LoadLibrary(D3DCOMPILER_DLL);
    }
    if (!mD3DCompilerModule)
    {
        ERR("No D3D compiler module found - aborting!\n");
        return false;
    }
#else  // ANGLE_PRELOADED_D3DCOMPILER_MODULE_NAMES

    if (!mD3DCompilerModule)
    {
        // Load the version of the D3DCompiler DLL associated with the Direct3D version ANGLE was built with.
#if defined(ANGLE_PLATFORM_WINRT)
#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)
        mD3DCompilerModule = LoadLibrary(D3DCOMPILER_DLL);
#elif !defined(ANGLE_PLATFORM_WP8)
        mD3DCompilerModule = LoadPackagedLibrary(D3DCOMPILER_DLL, 0);
#endif //WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)
    }

    if (!mD3DCompilerModule)
    {
        ERR("No D3D compiler module found - aborting!\n");
		mHasCompiler = false;
        return false;
    }
#else
	mD3DCompilerModule = LoadLibrary(D3DCOMPILER_DLL);
#endif // defined(ANGLE_PLATFORM_WINRT)
#endif // defined(ANGLE_PRELOADED_D3DCOMPILER_MODULE_NAMES)

    mD3DCompileFunc = reinterpret_cast<CompileFuncPtr>(GetProcAddress(mD3DCompilerModule, "D3DCompile"));
    ASSERT(mD3DCompileFunc);

    return mD3DCompileFunc != NULL;
}

void HLSLCompiler::release()
{
    if (mD3DCompilerModule)
    {
        FreeLibrary(mD3DCompilerModule);
        mD3DCompilerModule = NULL;
        mD3DCompileFunc = NULL;
    }
}

#if !defined(ANGLE_PLATFORM_WP8)
ShaderBlob *HLSLCompiler::compileToBinary(gl::InfoLog &infoLog, const char *hlsl, const char *profile,
                                         unsigned int optimizationFlags, bool alternateFlags) const
{
#if !defined(ANGLE_PLATFORM_WINRT)
    ASSERT(mD3DCompilerModule && mD3DCompileFunc);
#endif

    if (!hlsl)
    {
        return NULL;
    }

    HRESULT result = S_OK;
    UINT flags = 0;
    std::string sourceText;
    if (gl::perfActive())
    {
        flags |= D3DCOMPILE_DEBUG;

#ifdef NDEBUG
        flags |= optimizationFlags;
#else
        flags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

        std::string sourcePath = getTempPath();
        sourceText = std::string("#line 2 \"") + sourcePath + std::string("\"\n\n") + std::string(hlsl);
        writeFile(sourcePath.c_str(), sourceText.c_str(), sourceText.size());
    }
    else
    {
        flags |= optimizationFlags;
        sourceText = hlsl;
    }

    // Sometimes D3DCompile will fail with the default compilation flags for complicated shaders when it would otherwise pass with alternative options.
    // Try the default flags first and if compilation fails, try some alternatives.
    const static UINT extraFlags[] =
    {
        0,
        D3DCOMPILE_AVOID_FLOW_CONTROL,
        D3DCOMPILE_PREFER_FLOW_CONTROL
    };

    const static char * const extraFlagNames[] =
    {
        "default",
        "avoid flow control",
        "prefer flow control"
    };

    int attempts = alternateFlags ? ArraySize(extraFlags) : 1;
    pD3DCompile compileFunc = reinterpret_cast<pD3DCompile>(mD3DCompileFunc);
    for (int i = 0; i < attempts; ++i)
    {
        ID3DBlob *errorMessage = NULL;
        ID3DBlob *binary = NULL;

        result = compileFunc(hlsl, strlen(hlsl), gl::g_fakepath, NULL, NULL,
            "main", profile, flags | extraFlags[i], 0, &binary, &errorMessage);
        if (errorMessage)
        {
            const char *message = (const char*)errorMessage->GetBufferPointer();

            infoLog.appendSanitized(message);
            TRACE("\n%s", hlsl);
            TRACE("\n%s", message);

            SafeRelease(errorMessage);
        }

        if (SUCCEEDED(result))
        {
            return (ShaderBlob*)binary;
        }
        else
        {
            if (result == E_OUTOFMEMORY)
            {
                return gl::error(GL_OUT_OF_MEMORY, (ShaderBlob*)NULL);
            }

            infoLog.append("Warning: D3D shader compilation failed with ");
            infoLog.append(extraFlagNames[i]);
            infoLog.append(" flags.");
            if (i + 1 < attempts)
            {
                infoLog.append(" Retrying with ");
                infoLog.append(extraFlagNames[i + 1]);
                infoLog.append(".\n");
            }
        }
    }

    return NULL;
}
#endif // #if !defined(ANGLE_PLATFORM_WP8)

}
