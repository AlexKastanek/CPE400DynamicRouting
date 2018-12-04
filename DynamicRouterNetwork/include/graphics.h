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
    void RenderObjects(Shader*);
    void RenderRouter(Shader* , glm::vec3);
    void RenderAllConnections(Shader*);
    void RenderConnection(glm::vec3 from, glm::vec3 to);

  private:
    std::string ErrorString(GLenum error);

	DynamicRouterNetwork drn;

    Camera *m_camera;
    Shader *m_shader;
    Shader *m_screenShader;
    Shader *m_shadowShader;

    GLint m_projectionMatrix;
    GLint m_viewMatrix;
    GLint m_modelMatrix;

    FrameBuffer *m_frameBuffer;
    FrameBuffer *m_shadowBuffer;
    glm::mat4 lightMatrix;

    Object *m_cube;
};

#endif /* GRAPHICS_H */
