#include "graphics.h"

Graphics::Graphics() {
	minCost = 99999;
	maxCost = 0;
	pathAlgorithm = 0;
	path = NULL;
	source = 0;
	destination = 2;
}

Graphics::~Graphics() {

}

bool Graphics::Initialize(int width, int height) {
	this->width = width;
	this->height = height;
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
}

void Graphics::StepSimulation() {
	drn.Update();
	//calulate based on currently selected algorithm
	CalculateShortestPath();

}

void Graphics::CalculateShortestPath() {
  Graph<Router> graph = drn.GetGraph();
  if (!graph.BFS(source, destination) || source == destination)
  {
    if (path != NULL)
    {
      delete path;
      path = NULL;
    }
  }
  else
  {
    if (path != NULL)
    {
      delete path;
      path = NULL;
    }
    switch (pathAlgorithm) {
    case 0:
      //no shortest path, return null
      //delete path;
      //path = NULL;
      break;
    case 1:
      //calculate dijkstras shortest path
      path = drn.BestPathDijsktra(source, destination);
      cout << path->size();
      break;
    case 2:
      //calculate bellman ford shortest path
      path = drn.BestPathBellmanFord(source, destination);
      cout << path->size();
      break;
    }
  }
}

void Graphics::RenderObjects(Shader *shader) {
	//read graph state and render routers
	Graph<Router> graph = drn.GetGraph();
	int size = graph.GetMap().size();
  map<int, Vertex<Router>*> map = graph.GetMap();
  typename map<int, Vertex<Router>*>::iterator it;
	for (it = map.begin(); it != map.end(); it++) {
		//put in a circle based on router count
		auto vertex = graph.GetVertexWithID(it->first);
		if (vertex != NULL) {
			RenderRouter(shader, RouterPosition(it->first, size), vertex);
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
  map<int, Vertex<Router>*> map = graph.GetMap();
	int size = map.size();
  typename map<int, Vertex<Router>*>::iterator it;

	for (it = map.begin(); it != map.end(); it++) {
		Vertex<Router>* vertex = graph.GetVertexWithID(it->first);
			for (int j = 0; j < vertex->m_adjacencyList.size(); j++) 
      {
        Vertex<Router>* neighbor = vertex->m_adjacencyList[j].second;

				//update min and max cost
				double cost = vertex->GetEdgeCost(neighbor->GetID());
				if (cost < minCost)
					minCost = cost;
				if (cost > maxCost)
					maxCost = cost;

				//calculate link quality
				glm::vec2 color = glm::vec2((cost-minCost)/maxCost-minCost, 0.0);

				//display if shortest path
				if (path != NULL) {
					vector<int>::iterator it_path = path->begin();
					it_path++;
					while (it_path != path->end()) {
						if ((*(it_path-1) == it->first && *it_path == neighbor->GetID()) || (*(it_path-1) == neighbor->GetID() && *it_path == it->first)) {
							color.y = 1.0;
						}
						it_path++;
					}
				}

				//render
				glUniform2fv(shader->GetUniformLocation("lineColor"), 1,
				glm::value_ptr(color));
				RenderConnection(RouterPosition(it->first, size), RouterPosition(neighbor->GetID(), size));
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
	//update positioning based on hex grid
	int rowCount = glm::sqrt(routerCount);
	float rowSpace = 2;
	int rowOffset = routerNum % rowCount;
	if (rowOffset % 2 == 0) {
		//if even, put on top row
		rowOffset = 0;
	} else {
		rowOffset = 1;
	}
	int rowNum = routerNum % rowCount;
	int rowPair = routerNum / rowCount;

	//calculate column spacing based on router count
	float colSpace = 1;

	//set position
	float xPos = rowNum*rowSpace - (rowCount-1)*rowSpace/2;
	float zPos = (colSpace*2*rowPair + colSpace*rowOffset) - ((routerCount / rowCount)*colSpace)/2;
	return glm::vec3(xPos,0,zPos);
}

/**
 * Renders a single router using the default mesh
 */
void Graphics::RenderRouter(Shader *shader , glm::vec3 position, Vertex<Router> *vert) {
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

	//Render a UI element on top of the router
	glm::vec4 v = m_camera->GetProjection() * m_camera->GetView() * newPos * glm::vec4(0,0,0,1.0);
	v /= v.w;
	v += glm::vec4(1,1,1,1);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0,0,0,0));
	ImGui::Begin("Menu", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar |ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoInputs);
	ImGui::SetNextWindowPos(ImVec2(0,0));
	ImGui::SetNextWindowSize(ImVec2 (width, height));
	ImGui::SetCursorScreenPos(ImVec2(v.x * width / 2.0, height - v.y * height / 2.0));
	ImGui::Text("Router %i", vert->GetID());
	ImGui::End();
	ImGui::PopStyleColor(1);
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

	//render GUI
	ImGui::Text("Shortest Path Source/Destination:");

	if (ImGui::InputInt("Source", &source)) {
		CalculateShortestPath();
	}

	if (ImGui::InputInt("Destination", &destination)) {
		CalculateShortestPath();
	}

	ImGui::Text("Shortest Path Algorithm:");

	if (ImGui::RadioButton("None", &pathAlgorithm, 0)) {
	    cout << "Setting to no shortest path algorithm" << endl;
	    CalculateShortestPath();
	}

	if (ImGui::RadioButton("Dijkstra's", &pathAlgorithm, 1)) {
		cout << "Setting to Dijkstra's algorithm" << endl;
		CalculateShortestPath();
	}

	if (ImGui::RadioButton("Bellman Ford", &pathAlgorithm, 2)) {
		cout << "Setting to Bellman Ford algorithm" << endl;
		CalculateShortestPath();
	}

	//output a path to screen
	if (path != NULL) {
		ImGui::Text("Path found from Router %i to Router %i:", source, destination);
		for (int i = 0; i < path->size(); i++) {
			ImGui::Text("[%i]", (*path)[i]);
		}
	}
  else
  {
    ImGui::Text("No path is found");
  }

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

