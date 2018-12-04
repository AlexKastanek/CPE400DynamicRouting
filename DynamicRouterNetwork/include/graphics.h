#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <iostream>
using namespace std;

#include "graphics_headers.h"
#include "camera.h"
#include "shader.h"
#include "object.h"
#include "frameBuffer.h"
#include "DynamicRouterNetwork.h"

class Graphics
{
  public:
    Graphics();
    ~Graphics();
    bool Initialize(int width, int height);
    void Update(unsigned int dt);
    void Render();

  private:
    void RenderObjects(Shader*);
    void RenderRouter(Shader* , glm::vec3);
    void RenderAllConnections(Shader*);
    void RenderConnection(glm::vec3 from, glm::vec3 to);
    glm::vec3 RouterPosition(int routerNum, int routerCount);
    std::string ErrorString(GLenum error);

	DynamicRouterNetwork drn;

    Camera *m_camera;
    Shader *m_shader;
    Shader *m_screenShader;
    Shader *m_shadowShader;
    Shader *m_linkShader;

    GLint m_projectionMatrix;
    GLint m_viewMatrix;
    GLint m_modelMatrix;
    GLuint connections;

    FrameBuffer *m_frameBuffer;
    FrameBuffer *m_shadowBuffer;
    glm::mat4 lightMatrix;

    Object *m_cube;
};

#endif /* GRAPHICS_H */
