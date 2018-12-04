#include "graphics.h"

Graphics::Graphics() {

}

Graphics::~Graphics() {

}

bool Graphics::Initialize(int width, int height) {
	// Used for the linux OS
#if !defined(__APPLE__) && !defined(MACOSX)
	// cout << glewGetString(GLEW_VERSION) << endl;
	glewExperimental = GL_TRUE;

	auto status = glewInit();

	// This is here to grab the error that comes from glew init.
	// This error is an GL_INVALID_ENUM that has no effects on the performance
	glGetError();

	//Check for error
	if (status != GLEW_OK) {
		std::cerr << "GLEW Error: " << glewGetErrorString(status) << "\n";
		return false;
	}
#endif

	// For OpenGL 3
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Init Camera
	m_camera = new Camera();
	if (!m_camera->Initialize(width, height)) {
		printf("Camera Failed to Initialize\n");
		return false;
	}

	// Create the object
	m_cube = new Object();

	// Set up the shaders
	//setup screen shader
	m_screenShader = new Shader();
	if (!m_screenShader->CreateShader("assets/screen.vert", "assets/screen.frag"))
		return false;

	//setup screen shader
	m_shadowShader = new Shader();
	if (!m_shadowShader->CreateShader("assets/passthrough.vert", "assets/passthrough.frag"))
		return false;

	//setup link shader
	m_linkShader = new Shader();
	if (!m_linkShader->CreateShader("assets/link.vert", "assets/link.frag"))
		return false;

	//setup screen shader
	m_shader = new Shader();
	if (!m_shader->CreateShader("assets/vertexShader.s", "assets/fragmentShader.s"))
		return false;

	// Locate the projection matrix in the shader
	m_projectionMatrix = m_shader->GetUniformLocation("projectionMatrix");
	if (m_projectionMatrix == INVALID_UNIFORM_LOCATION) {
		printf("m_projectionMatrix not found\n");
		return false;
	}

	// Locate the view matrix in the shader
	m_viewMatrix = m_shader->GetUniformLocation("viewMatrix");
	if (m_viewMatrix == INVALID_UNIFORM_LOCATION) {
		printf("m_viewMatrix not found\n");
		return false;
	}

	// Locate the model matrix in the shader
	m_modelMatrix = m_shader->GetUniformLocation("modelMatrix");
	if (m_modelMatrix == INVALID_UNIFORM_LOCATION) {
		printf("m_modelMatrix not found\n");
		return false;
	}

	//create a new frame buffer
	m_frameBuffer = new FrameBuffer(width, height);

	//create the shadow frame buffer
	m_shadowBuffer = new FrameBuffer(width,height);

	//setup shadowed light
	lightMatrix = glm::lookAt(glm::vec3(2.0f, 3.0f, -2.0f),
                                  glm::vec3( 0.0f, 0.0f,  0.0f), 
                                  glm::vec3( 0.0f, 1.0f,  0.0f));  

	//enable depth testing
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	//initialize Dynamic router network
	drn.Initialize();

	//initialize link array
	glGenBuffers(1, &connections);

	return true;
}

void Graphics::Update(unsigned int dt) {
	// Update the object
	m_cube->Update(dt);
	drn.Update();
}

void Graphics::RenderObjects(Shader *shader) {
	//read graph state and render routers
	Graph<Router> graph = drn.GetGraph();
	int size = graph.GetMap().size();
	for (int i = 0; i < size; i++) {
		//put in a circle based on router count
		auto vertex = graph.GetVertexWithID(i);
		if (vertex != NULL) {
			RenderRouter(shader, RouterPosition(i, size));
		}
	}
}

void Graphics::RenderAllConnections(Shader *shader) {
	//pass in default matrices

	glUniformMatrix4fv(shader->GetUniformLocation("projectionMatrix"), 1, GL_FALSE,
			glm::value_ptr(m_camera->GetProjection()));

	glUniformMatrix4fv(shader->GetUniformLocation("viewMatrix"), 1, GL_FALSE,
			glm::value_ptr(m_camera->GetView()));

	glUniformMatrix4fv(shader->GetUniformLocation("modelMatrix"), 1, GL_FALSE,
			glm::value_ptr(glm::mat4(1.0)));

	//read graph state and render routers
	Graph<Router> graph = drn.GetGraph();
	int size = graph.GetMap().size();
	for (int i = 0; i < size; i++) {
		auto vertex = graph.GetVertexWithID(i);
		if (vertex != NULL) {
			for (int j = 0; j < size; j++) {
				if (vertex->IsConnectedTo(j)) {
					cout << vertex->GetEdgeCost(j) << endl;
					glm::vec2 color = glm::vec2((vertex->GetEdgeCost(j)-1.5)*1.9, 0.0);
					glUniform2fv(shader->GetUniformLocation("lineColor"), 1,
							glm::value_ptr(color));
					RenderConnection(RouterPosition(i, size), RouterPosition(j, size));
				}
			}
		}
	}
}
void Graphics::RenderConnection(glm::vec3 from, glm::vec3 to) {
	float link[] = {from.x, from.y, from.z, to.x, to.y, to.z};
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, connections);
	glBufferData(GL_ARRAY_BUFFER, sizeof(link), link, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glLineWidth(5.0);
	glDrawArrays(GL_LINES, 0, 6);
	glDisableVertexAttribArray(0);
}

glm::vec3 Graphics::RouterPosition(int routerNum, int routerCount) {
	float radius = 2;
	double angle = glm::radians((360.0/routerCount)*routerNum);
	return glm::vec3(glm::cos(angle)*radius,0,glm::sin(angle)*radius);
}

/**
 * Renders a single router using the default mesh
 */
void Graphics::RenderRouter(Shader *shader , glm::vec3 position) {
	glm::mat4 newPos = glm::translate(position) * m_cube->GetModel();

	// Send in the projection and view to the shader
	glUniformMatrix4fv(shader->GetUniformLocation("projectionMatrix"), 1, GL_FALSE,
			glm::value_ptr(m_camera->GetProjection()));

	glUniformMatrix4fv(shader->GetUniformLocation("viewMatrix"), 1, GL_FALSE,
			glm::value_ptr(m_camera->GetView()));

	glUniformMatrix4fv(shader->GetUniformLocation("lightMatrix"), 1, GL_FALSE,
			glm::value_ptr(lightMatrix));

	// Render the object
	glUniformMatrix4fv(shader->GetUniformLocation("modelMatrix"), 1, GL_FALSE,
			glm::value_ptr(newPos));
	m_cube->Render();
}

void Graphics::Render() {
	//this time, render our scene from the light's perspective
	//bind the shadow buffer
	m_shadowBuffer->useBuffer();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_shadowShader->Enable();

	//render using the light matrix
	RenderObjects(m_shadowShader);

	//use our new frame buffer
	m_frameBuffer->useBuffer();

	//clear the screen
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Start the correct program
	m_shader->Enable();
	m_shadowBuffer->useDepthOutput(GL_TEXTURE0);

	RenderObjects(m_shader);
	m_linkShader->Enable();
	RenderAllConnections(m_linkShader);

	//use the original screen frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//clear the screen
	glClearColor(1.0, 0.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//enable screen shader
	m_screenShader->Enable();
	m_frameBuffer->renderToScreen();

	// Get any errors from OpenGL
	auto error = glGetError();
	if (error != GL_NO_ERROR) {
		string val = ErrorString(error);
		std::cout << "Error initializing OpenGL! " << error << ", " << val
				<< std::endl;
	}
}

std::string Graphics::ErrorString(GLenum error) {
	if (error == GL_INVALID_ENUM) {
		return "GL_INVALID_ENUM: An unacceptable value is specified for an enumerated argument.";
	}

	else if (error == GL_INVALID_VALUE) {
		return "GL_INVALID_VALUE: A numeric argument is out of range.";
	}

	else if (error == GL_INVALID_OPERATION) {
		return "GL_INVALID_OPERATION: The specified operation is not allowed in the current state.";
	}

	else if (error == GL_INVALID_FRAMEBUFFER_OPERATION) {
		return "GL_INVALID_FRAMEBUFFER_OPERATION: The framebuffer object is not complete.";
	}

	else if (error == GL_OUT_OF_MEMORY) {
		return "GL_OUT_OF_MEMORY: There is not enough memory left to execute the command.";
	} else {
		return "None";
	}
}

