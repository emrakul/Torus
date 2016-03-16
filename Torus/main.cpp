#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")
#pragma comment(lib,"GL/Win32/glew32.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"GLFW/glfw3.lib")
//#pragma comment(lib,"GLFW/glfw3dll.lib")

#include "main.h"
Camera * camera;
float fov;
float con;


std::vector<ControlPoint> wave;


static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}


float sdTorus(vec3 p)
{	
	vec2 t = vec2(6.0, 2);
	vec2 q = vec2(length(vec2(p.x, p.z) ) - t.x, p.y);
	return length(q) - t.y;
}






vec3 intersect(vec3 origin, vec3 direction)
{
	float rayLength = 0.1;

	float maxt = 59.0;
	float i;
	for (i = 0; i < 100; ++i)
	{
		vec3 point = origin + direction*rayLength;

		float dist = sdTorus(point);
		if (dist < 0.001)
			return vec3(point);

		rayLength += dist;
	}

	return vec3(origin + direction*maxt);
}

vec3 getNormal(vec3 p){

	float eps = 0.00001;
	float center = sdTorus(p);
	vec3 n = vec3(sdTorus(vec3(p.x - eps, p.y, p.z)) - sdTorus(vec3(p.x + eps, p.y, p.z)),
		sdTorus(vec3(p.x, p.y - eps, p.z)) - sdTorus(vec3(p.x, p.y + eps, p.z)),
		sdTorus(vec3(p.x, p.y, p.z - eps)) - sdTorus(vec3(p.x, p.y, p.z + eps)));
	return normalize(n);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{	
	vec3 spawnPoint;
	vec3 normal;
	vec3 tangent;
	vec3 bitangent;

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key == GLFW_KEY_LEFT && action == GLFW_PRESS){
		con -= 1;
		std::cout << fov;
	}
	if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS){
		con += 1;
		std::cout << fov;
	}

	
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS){
		spawnPoint = intersect(camera->position, camera->direction);
		std::cout << spawnPoint.x << " " << spawnPoint.y << " " << spawnPoint.z << std::endl;
		normal = getNormal(spawnPoint);
		//tangent.x = 1;		
		//tangent.z = 0;
	//	tangent.y = -normal.x / (normal.y + 0.01);
		//tangent = normalize(tangent);
		tangent = camera->upVector;
		bitangent = cross(tangent, normal);

		wave.resize(0);
		wave.resize(800);
		
		for (int i = 0; i < 800; ++i) {
			wave[i].distance = sdTorus(spawnPoint);
			wave[i].position = spawnPoint;		
			wave[i].bitangent = bitangent;
			wave[i].tangent = tangent;

			wave[i].normal = normal;		
			wave[i].direction = vec3( cos((i-400) * 3.1415/400.0), 0, sin((i-400) * 3.1415/400.0) );
		}
	}	
	

}

int main(void)
{
	GLFWwindow* window;
	camera = new Camera();
	

	fov = 67;
	con = 0.0;
	int width, height;

	glfwSetErrorCallback(error_callback);

	if (!glfwInit())
		exit(EXIT_FAILURE);

	window = glfwCreateWindow(1600, 900, "Simple example", glfwGetPrimaryMonitor(), NULL);

	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	glewInit();

	glfwSetKeyCallback(window, key_callback);


	glfwGetFramebufferSize(window, &width, &height);

	glViewport(0, 0, width, height);	
	glEnable(GL_DEPTH_TEST);	
	glDepthFunc(GL_LEQUAL);	
	glDisable(GL_CULL_FACE);
	
	

	Quad* quad = new Quad;
	Shader * geom = new Shader("geometry.vert", "geometry.frag");

	geom->setAttribute(0, "point");
	geom->linkProgram();
	geom->bind();	
	geom->unbind();
	
	
	float deltaTime = float(0.005);

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDepthMask(GL_TRUE);
		camera->Update();

		geom->bind();			
			geom->passUniform(camera->position, "position");			
			geom->passUniform(camera->direction, "direction");
			geom->passUniform(camera->upVector, "upVector");
			geom->passUniform(camera->viewMatrix, "viewMatrix");
			geom->passUniform(camera->projectionMatrix, "projectionMatrix");
			geom->passUniform(con, "con");
			quad->Render();
		geom->unbind();
		glDepthMask(GL_TRUE);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		

		for (int i = 0; i < wave.size(); ++i) {
			mat3 tangentSpace(wave[i].bitangent, wave[i].normal, wave[i].tangent);
			float previousDistance = wave[i].distance;
			wave[i].position += tangentSpace * wave[i].direction  * deltaTime;

			
			wave[i].distance = sdTorus(wave[i].position);
			wave[i].position += wave[i].normal * wave[i].distance;

			wave[i].normal = getNormal(wave[i].position);

			vec3 tangent, bitangent, normal;
			vec3 position = wave[i].position;
			normal = wave[i].normal;
			mat4 rotation = glm::rotate(0.05f, vec3(0, 1.0, 0));
			vec3 delta = vec3( rotation*vec4(position, 1.0) );
			
			tangent = delta - wave[i].position; 
			
			
			tangent = normalize(tangent);			

			bitangent = cross(tangent, normal);
			wave[i].bitangent = bitangent;
			wave[i].tangent = tangent;
		}		

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(67.3801, (GLfloat)1600.0 / (GLfloat)900.0, 0.5, 50.0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(
			camera->position.x, camera->position.y, camera->position.z,

			camera->position.x + camera->direction.x,
			camera->position.y + camera->direction.y,
			camera->position.z + camera->direction.z,

			
			camera->upVector.x, camera->upVector.y, camera->upVector.z);


		if (wave.size() > 0) {
			vec3 arr[800];
			glBegin(GL_LINE_LOOP);
			for (int i = 0; i < 800; ++i){
				arr[i] = wave[i].position;
				glColor3f(1.0, 1.0, 1.0);
				glVertex3f(arr[i].x, arr[i].y, arr[i].z);

				
				/*
				glBegin(GL_LINE_LOOP);
				glColor3f(1.0, 0.0, 0.0);
				
				arr[i] = wave[i].position;
				vec3 nrm = wave[i].normal * float(0.1);
				glVertex3f(arr[i].x, arr[i].y, arr[i].z);
				glVertex3f(arr[i].x+nrm.x, arr[i].y +nrm.y, arr[i].z+nrm.z);
				glEnd();
				

				glBegin(GL_LINE_LOOP);
				glColor3f(0.0, 1.0, 0.0);

				arr[i] = wave[i].position;
				nrm = wave[i].tangent * float(0.1);
				glVertex3f(arr[i].x, arr[i].y, arr[i].z);
				glVertex3f(arr[i].x + nrm.x, arr[i].y + nrm.y, arr[i].z + nrm.z);
				glEnd();

				

				glBegin(GL_LINE_LOOP);
				glColor3f(0.0, 0.0, 1.0);

				arr[i] = wave[i].position;
				nrm = wave[i].bitangent * float(0.1);
				glVertex3f(arr[i].x, arr[i].y, arr[i].z);
				glVertex3f(arr[i].x + nrm.x, arr[i].y + nrm.y, arr[i].z + nrm.z);
				glEnd();
				
				
				*/
				


			}
			glEnd();

		}

		glfwSwapBuffers(window);
		glfwPollEvents();


	}

	glfwDestroyWindow(window);

	glfwTerminate();
	exit(EXIT_SUCCESS);
}


