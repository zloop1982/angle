#include "pch.h"
#include "Sample3DSceneRenderer.h"

#include "..\Common\DirectXHelper.h"

#define STRINGIFY(x) #x

using namespace RotatingCubeXaml_UA;

using namespace DirectX;
using namespace Windows::Foundation;

const char g_colorVertexShader[] = STRINGIFY(
precision mediump float;
attribute vec3 a_position;
attribute vec3 a_color;
varying vec3 v_color;
uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;
void main(void)
{
    gl_Position = u_projection * u_view * u_model * vec4(a_position, 1);
    v_color = a_color;
}
);

const char g_colorFragmentShader[] = STRINGIFY(
precision mediump float;
varying vec3 v_color;
void main(void)
{
    gl_FragColor = vec4(v_color, 1);
}
);

static GLuint LoadShader(GLenum type, const char *shaderSrc)
{
    GLuint shader;
    GLint compiled;

    // Create the shader object
    shader = glCreateShader(type);

    if (shader == 0)
        return 0;

    // Load the shader source
    glShaderSource(shader, 1, &shaderSrc, NULL);

    // Compile the shader
    glCompileShader(shader);

    // Check the compile status
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

    if (!compiled)
    {
        GLint infoLen = 0;

        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);

        if (infoLen > 1)
        {
            char* infoLog = (char*) malloc(sizeof(char) * infoLen);

            glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
            //esLogMessage("Error compiling shader:\n%s\n", infoLog);

            free(infoLog);
        }

        glDeleteShader(shader);
        return 0;
    }

    return shader;

}

static GLuint LoadProgram(const char *vertShaderSrc, const char *fragShaderSrc)
{
    GLuint vertexShader;
    GLuint fragmentShader;
    GLuint programObject;
    GLint linked;

    // Load the vertex/fragment shaders
    vertexShader = LoadShader(GL_VERTEX_SHADER, vertShaderSrc);
    if (vertexShader == 0)
        return 0;

    fragmentShader = LoadShader(GL_FRAGMENT_SHADER, fragShaderSrc);
    if (fragmentShader == 0)
    {
        glDeleteShader(vertexShader);
        return 0;
    }

    // Create the program object
    programObject = glCreateProgram();

    if (programObject == 0)
        return 0;

    glAttachShader(programObject, vertexShader);
    glAttachShader(programObject, fragmentShader);

    // Link the program
    glLinkProgram(programObject);

    // Check the link status
    glGetProgramiv(programObject, GL_LINK_STATUS, &linked);

    if (!linked)
    {
        GLint infoLen = 0;

        glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &infoLen);

        if (infoLen > 1)
        {
            char* infoLog = (char*) malloc(sizeof(char) * infoLen);

            glGetProgramInfoLog(programObject, infoLen, NULL, infoLog);
            //esLogMessage("Error linking program:\n%s\n", infoLog);

            free(infoLog);
        }

        glDeleteProgram(programObject);
        return 0;
    }

    // Free up no longer needed shader resources
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return programObject;
}

// Loads vertex and pixel shaders from files and instantiates the cube geometry.
Sample3DSceneRenderer::Sample3DSceneRenderer(const std::shared_ptr<DX::DeviceResources>& deviceResources) :
	//m_loadingComplete(false),
	m_degreesPerSecond(45),
	m_indexCount(0),
	m_tracking(false),
	m_deviceResources(deviceResources)
{
	CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();
}

// Initializes view parameters when the window size changes.
void Sample3DSceneRenderer::CreateWindowSizeDependentResources()
{
	Size outputSize = m_deviceResources->GetOutputSize();
	float aspectRatio = outputSize.Width / outputSize.Height;
	float fovAngleY = 70.0f * XM_PI / 180.0f;

	// This is a simple example of change that can be made when the app is in
	// portrait or snapped view.
	if (aspectRatio < 1.0f)
	{
		fovAngleY *= 2.0f;
	}

	// Note that the OrientationTransform3D matrix is post-multiplied here
	// in order to correctly orient the scene to match the display orientation.
	// This post-multiplication step is required for any draw calls that are
	// made to the swap chain render target. For draw calls to other targets,
	// this transform should not be applied.

	// This sample makes use of a right-handed coordinate system using row-major matrices.
	XMMATRIX perspectiveMatrix = XMMatrixPerspectiveFovRH(
		fovAngleY,
		aspectRatio,
		0.01f,
		100.0f
		);

	XMFLOAT4X4 orientation = m_deviceResources->GetOrientationTransform3D();

	XMMATRIX orientationMatrix = XMLoadFloat4x4(&orientation);

	XMStoreFloat4x4(
		&m_constantBufferData.projection,
		perspectiveMatrix * orientationMatrix
		);

	// Eye is at (0,0.7,1.5), looking at point (0,-0.1,0) with the up-vector along the y-axis.
	static const XMVECTORF32 eye = { 0.0f, 0.7f, 1.5f, 0.0f };
	static const XMVECTORF32 at = { 0.0f, -0.1f, 0.0f, 0.0f };
	static const XMVECTORF32 up = { 0.0f, 1.0f, 0.0f, 0.0f };

	XMStoreFloat4x4(&m_constantBufferData.view, XMMatrixLookAtRH(eye, at, up));
}

// Called once per frame, rotates the cube and calculates the model and view matrices.
void Sample3DSceneRenderer::Update(DX::StepTimer const& timer)
{
	if (!m_tracking)
	{
		// Convert degrees to radians, then convert seconds to rotation angle
		float radiansPerSecond = XMConvertToRadians(m_degreesPerSecond);
		double totalRotation = timer.GetTotalSeconds() * radiansPerSecond;
		float radians = static_cast<float>(fmod(totalRotation, XM_2PI));

		Rotate(radians);
	}
}

// Rotate the 3D cube model a set amount of radians.
void Sample3DSceneRenderer::Rotate(float radians)
{
	// Prepare to pass the updated model matrix to the shader
	XMStoreFloat4x4(&m_constantBufferData.model, XMMatrixRotationY(radians));
}

void Sample3DSceneRenderer::StartTracking()
{
	m_tracking = true;
}

// When tracking, the 3D cube can be rotated around its Y axis by tracking pointer position relative to the output screen width.
void Sample3DSceneRenderer::TrackingUpdate(float positionX)
{
	if (m_tracking)
	{
		float radians = XM_2PI * 2.0f * positionX / m_deviceResources->GetOutputSize().Width;
		Rotate(radians);
	}
}

void Sample3DSceneRenderer::StopTracking()
{
	m_tracking = false;
}

// Renders one frame using the vertex and pixel shaders.
void Sample3DSceneRenderer::Render()
{
    glClearColor(0, 0, 0, 1.000f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(m_colorProgram);
    glUniformMatrix4fv(u_model, 1, GL_FALSE, &m_constantBufferData.model.m[0][0]);
    glUniformMatrix4fv(u_view, 1, GL_FALSE, &m_constantBufferData.view.m[0][0]);
    glUniformMatrix4fv(u_projection, 1, GL_FALSE, &m_constantBufferData.projection.m[0][0]);

    static const VertexPositionColor cubeVertices[] = 
	{
		{XMFLOAT3(-0.5f, -0.5f, -0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f)},
		{XMFLOAT3(-0.5f, -0.5f,  0.5f), XMFLOAT3(0.0f, 0.0f, 1.0f)},
		{XMFLOAT3(-0.5f,  0.5f, -0.5f), XMFLOAT3(0.0f, 1.0f, 0.0f)},
		{XMFLOAT3(-0.5f,  0.5f,  0.5f), XMFLOAT3(0.0f, 1.0f, 1.0f)},
		{XMFLOAT3( 0.5f, -0.5f, -0.5f), XMFLOAT3(1.0f, 0.0f, 0.0f)},
		{XMFLOAT3( 0.5f, -0.5f,  0.5f), XMFLOAT3(1.0f, 0.0f, 1.0f)},
		{XMFLOAT3( 0.5f,  0.5f, -0.5f), XMFLOAT3(1.0f, 1.0f, 0.0f)},
		{XMFLOAT3( 0.5f,  0.5f,  0.5f), XMFLOAT3(1.0f, 1.0f, 1.0f)},
	};
    static const unsigned short cubeIndices[] = 
    {

        0,2,1, // -x
        1,2,3,

        4,5,6, // +x
        5,7,6,

        0,1,5, // -y
        0,5,4,

        2,6,7, // +y
        2,7,3,

        0,4,6, // -z
        0,6,2,

        1,3,7, // +z
        1,7,5,
    };

    glEnableVertexAttribArray(a_positionColor);
    glEnableVertexAttribArray(a_colorColor);
    glVertexAttribPointer(a_positionColor, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPositionColor), cubeVertices);
    glVertexAttribPointer(a_colorColor, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPositionColor), reinterpret_cast<const char*>(cubeVertices) + sizeof(XMFLOAT3));
    glDrawElements(GL_TRIANGLES, ARRAYSIZE(cubeIndices), GL_UNSIGNED_SHORT, cubeIndices);
    glDisableVertexAttribArray(a_positionColor);
    glDisableVertexAttribArray(a_colorColor);
}

void Sample3DSceneRenderer::CreateDeviceDependentResources()
{
    m_colorProgram = LoadProgram(g_colorVertexShader, g_colorFragmentShader);
    a_positionColor = glGetAttribLocation(m_colorProgram, "a_position");
    a_colorColor = glGetAttribLocation(m_colorProgram, "a_color");
    u_model = glGetUniformLocation(m_colorProgram, "u_model");
    u_view = glGetUniformLocation(m_colorProgram, "u_view");
    u_projection = glGetUniformLocation(m_colorProgram, "u_projection");
    glEnable(GL_DEPTH_TEST);
}

void Sample3DSceneRenderer::ReleaseDeviceDependentResources()
{
    glDeleteProgram(m_colorProgram);
	//m_vertexShader.Reset();
	//m_inputLayout.Reset();
	//m_pixelShader.Reset();
	//m_constantBuffer.Reset();
	//m_vertexBuffer.Reset();
	//m_indexBuffer.Reset();
}