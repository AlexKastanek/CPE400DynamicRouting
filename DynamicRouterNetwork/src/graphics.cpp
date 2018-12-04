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
	m_shader = new Shader();
	if (!m_shader->Initialize()) {
		printf("Shader Failed to Initialize\n");
		return false;
	}

	// Add the vertex shader
	if (!m_shader->AddShader(GL_VERTEX_SHADER, "assets/vertexShader.s")) {
		printf("Vertex Shader failed to Initialize\n");
		return false;
	}

	// Add the fragment shader
	if (!m_shader->AddShader(GL_FRAGMENT_SHADER, "assets/fragmentShader.s")) {
		printf("Fragment Shader failed to Initialize\n");
		return false;
	}

	// Connect the program
	if (!m_shader->Finalize()) {
		printf("Program to Finalize\n");
		return false;
	}

	//setup screen shader
	m_screenShader = new Shader();
	if (!m_screenShader->Initialize()) {
		printf("Shader Failed to Initialize\n");
		return false;
	}

	// Add the vertex shader
	if (!m_screenShader->AddShader(GL_VERTEX_SHADER, "assets/screen.vert")) {
		printf("Vertex Shader failed to Initialize\n");
		return false;
	}

	// Add the fragment shader
	if (!m_screenShader->AddShader(GL_FRAGMENT_SHADER, "assets/screen.frag")) {
		printf("Fragment Shader failed to Initialize\n");
		return false;
	}

	// Connect the program
	if (!m_screenShader->Finalize()) {
		printf("Program to Finalize\n");
		return false;
	}

	//setup shadow shader
	m_shadowShader = new Shader();
	if (!m_shadowShader->Initialize()) {
		printf("Shader Failed to Initialize\n");
		return false;
	}

	// Add the vertex shader
	if (!m_shadowShader->AddShader(GL_VERTEX_SHADER, "assets/passthrough.vert")) {
		printf("Vertex Shader failed to Initialize\n");
		return false;
	}

	// Add the fragment shader
	if (!m_shadowShader->AddShader(GL_FRAGMENT_SHADER, "assets/passthrough.frag")) {
		printf("Fragment Shader failed to Initialize\n");
		return false;
	}

	// Connect the program
	if (!m_shadowShader->Finalize()) {
		printf("Program to Finalize\n");
		return false;
	}

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
	lightMatrix = glm::lookAt(glm::vec3(3.0f, 5.0f, -2.0f),
                                  glm::vec3( 0.0f, 0.0f,  0.0f), 
                                  glm::vec3( 0.0f, 1.0f,  0.0f));  

	//enable depth testing
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	//initialize Dynamic router network
	drn.Initialize();

	return true;
}

void Graphics::Update(unsigned int dt) {
	// Update the object
	m_cube->Update(dt);
}

void Graphics::RenderObjects(Shader *shader) {

	//read graph state and render routers
	Graph<Router> graph = drn.GetGraph();
	int i = 0;
	while (graph.GetVertexWithID(i) != NULL) {
		RenderRouter(shader, glm::vec3(i-2,0,0));
		i++;
	}
}

void Graphics::RenderAllConnections(Shader *shader) {
	RenderConnection(glm::vec3(0,0,0), glm::vec3(1,1,1));
}
void Graphics::RenderConnection(glm::vec3 from, glm::vec3 to) {
	//glBegin(GL_LINES);
	//    glVertex3f(from.x, from.y, from.z);
	//    glVertex3f(to.x, to.y, to.z);
	//glEnd();
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
	glClearColor(0.0, 0.2, 0.3, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Start the correct program
	m_shader->Enable();
	m_shadowBuffer->useDepthOutput(GL_TEXTURE0);

	RenderObjects(m_shader);
	RenderAllConnections(m_shader);

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

