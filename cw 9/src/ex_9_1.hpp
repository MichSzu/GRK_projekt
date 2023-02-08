#include "features/imports.hpp"

const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

namespace models {
	Core::RenderContext bedContext;
	Core::RenderContext chairContext;
	Core::RenderContext deskContext;
	Core::RenderContext doorContext;
	Core::RenderContext drawerContext;
	Core::RenderContext marbleBustContext;
	Core::RenderContext materaceContext;
	Core::RenderContext pencilsContext;
	Core::RenderContext planeContext;
	Core::RenderContext roomContext;
	Core::RenderContext spaceshipContext;
	// sun 1
	Core::RenderContext sphereContext;
	// added lamp
	Core::RenderContext lampContext;
	Core::RenderContext windowContext;
	Core::RenderContext testContext;
	Core::RenderContext computerContext;
}

GLuint depthMapFBO;
GLuint depthMap;

//sun 2
GLuint programSun;
GLuint programLamp;
GLuint programTest;
GLuint programTex;
GLuint programSkybox;

// NASZE KONTEKSTY MODELI (robione dla zmiany pozycji)
Core::RenderContext shipContext;
//sun 3
Core::RenderContext sphereContext;
Core::RenderContext lampContext;
Core::RenderContext bedContext;

// SUN 4 object/model - to jest dziwne słońce, które koloruje podłogę, ale światło nie pochodzi z lampy na suficie
glm::vec3 sunPos = glm::vec3(-4.740971f, 2.149999f, 0.369280f);
glm::vec3 sunDir = glm::vec3(-0.93633f, 0.351106, 0.003226f);
glm::vec3 sunColor = glm::vec3(0.9f, 0.9f, 0.7f)*5;

/* SUNLIGHT 5 position */
glm::vec3 pointlightPos = glm::vec3(0, 2, 0); // pozycja lampy pod sufitem
// to światło działa
glm::vec3 pointlightColor = glm::vec3(5.9, 5.6, 5.6);


// Lamp object/model
glm::vec3 lampModelPos = glm::vec3(-4.740971f, 2.149999f, 0.369280f);
glm::vec3 lampModelDir = glm::vec3(-0.93633f, 0.351106, 0.003226f);
glm::vec3 lampModelColor = glm::vec3(0.9f, 0.9f, 100.7f) * 5;

/* LAMPLIGHT position */
// pierwszy to szerokość, drugi to wysokość a trzeci głębokość
glm::vec3 lamplightPos = glm::vec3(-1.1, 1.2, 0.2); // lamp position
glm::vec3 lamplightColor = glm::vec3(0.9, 0.6, 0.6);

//glm::vec3 lamplightConeDir = glm::vec3(0, 0, 0);
//float lamplightPhi = 3.14 / 4;

// SHIPLIGHT
glm::vec3 spotlightPos = glm::vec3(0, 0, 0);
glm::vec3 spotlightConeDir = glm::vec3(0, 0, 0);
glm::vec3 spotlightColor = glm::vec3(0.4, 0.4, 0.9)*3;
float spotlightPhi = 3.14 / 4;

//w modelMatrix jest viewProjectionMatrix, pozycja i rozmiar obiektu
void drawObjectPBR(Core::RenderContext& context, glm::mat4 modelMatrix, glm::vec3 color, float roughness, float metallic) {

	glm::mat4 viewProjectionMatrix = createPerspectiveMatrix() * createCameraMatrix();
	glm::mat4 transformation = viewProjectionMatrix * modelMatrix;

	glUniformMatrix4fv(glGetUniformLocation(program, "transformation"), 1, GL_FALSE, (float*)&transformation);
	glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, GL_FALSE, (float*)&modelMatrix);

	glUniform1f(glGetUniformLocation(program, "exposition"), exposition);

	glUniform1f(glGetUniformLocation(program, "roughness"), roughness);
	glUniform1f(glGetUniformLocation(program, "metallic"), metallic);

	glUniform3f(glGetUniformLocation(program, "color"), color.x, color.y, color.z);

	glUniform3f(glGetUniformLocation(program, "cameraPos"), cameraPos.x, cameraPos.y, cameraPos.z);

	//SUN 6
	glUniform3f(glGetUniformLocation(program, "sunDir"), sunDir.x, sunDir.y, sunDir.z);
	glUniform3f(glGetUniformLocation(program, "sunColor"), sunColor.x, sunColor.y, sunColor.z);

	// SUNLIGHT 7
	// lightPos i lightColor są deklaracjami głównego słońca... Nie mają definicji i nie da się ich użyć dwa razy, bo się nadpisuje
	glUniform3f(glGetUniformLocation(program, "lightPos"), pointlightPos.x, pointlightPos.y, pointlightPos.z);
	glUniform3f(glGetUniformLocation(program, "lightColor"), pointlightColor.x, pointlightColor.y, pointlightColor.z);

	// LAMP
	glUniform3f(glGetUniformLocation(program, "lampModelDir"), lampModelDir.x, lampModelDir.y, lampModelDir.z);
	glUniform3f(glGetUniformLocation(program, "lampModelColor"), lampModelColor.x, lampModelColor.y, lampModelColor.z);


	// LAMPLIGHT - tu zamiast lamplightpos powinno być lightpos, ale może być tylko jedno źródło światła
	glUniform3f(glGetUniformLocation(program, "lamplightPos"), lamplightPos.x, lamplightPos.y, lamplightPos.z);
	glUniform3f(glGetUniformLocation(program, "lamplightColor"), lamplightColor.x, lamplightColor.y, lamplightColor.z);
	// lampka stożkowa, nie kulista
	//glUniform3f(glGetUniformLocation(program, "lamplightConeDir"), lamplightConeDir.x, lamplightConeDir.y, lamplightConeDir.z);
	//glUniform1f(glGetUniformLocation(program, "lightPhi"), lamplightPhi);

	
	// SPACESHIPLIGHTS
	glUniform3f(glGetUniformLocation(program, "spotlightConeDir"), spotlightConeDir.x, spotlightConeDir.y, spotlightConeDir.z);
	glUniform3f(glGetUniformLocation(program, "spotlightPos"), spotlightPos.x, spotlightPos.y, spotlightPos.z);
	glUniform3f(glGetUniformLocation(program, "spotlightColor"), spotlightColor.x, spotlightColor.y, spotlightColor.z);
	glUniform1f(glGetUniformLocation(program, "spotlightPhi"), spotlightPhi);
	Core::DrawContext(context);

}

void renderShadowapSun() {
	float time = glfwGetTime();
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	//uzupelnij o renderowanie glebokosci do tekstury

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, WIDTH, HEIGHT);
}

void renderScene(GLFWwindow* window)
{
	glClearColor(0.4f, 0.4f, 0.8f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	float time = glfwGetTime();
	updateDeltaTime(time);
	renderShadowapSun();

	
	//sun 8
	glUseProgram(programSun);
	glm::mat4 viewProjectionMatrix = createPerspectiveMatrix() * createCameraMatrix();
	glm::mat4 transformation = viewProjectionMatrix * glm::translate(pointlightPos) * glm::scale(glm::vec3(0.1));
	glUniformMatrix4fv(glGetUniformLocation(programSun, "transformation"), 1, GL_FALSE, (float*)&transformation);
	glUniform3f(glGetUniformLocation(programSun, "color"), sunColor.x / 2, sunColor.y / 2, sunColor.z / 2);
	glUniform1f(glGetUniformLocation(programSun, "exposition"), exposition);
	Core::DrawContext(sphereContext);

	// lamp lamp
	glUseProgram(programLamp);
	glm::mat4 transformation2 = viewProjectionMatrix * glm::translate(lamplightPos) * glm::scale(glm::vec3(0.1));
	glUniformMatrix4fv(glGetUniformLocation(programLamp, "transformation"), 1, GL_FALSE, (float*)&transformation2);
	glUniform3f(glGetUniformLocation(programLamp, "color"), lampModelColor.x / 2, lampModelColor.y / 2, lampModelColor.z / 2);
	glUniform1f(glGetUniformLocation(programLamp, "exposition"), exposition);
	Core::DrawContext(lampContext);
	
	glUseProgram(program);

	//Dodatkowa planeta 1 - orbitujące coś
	// DrawobjectPBR(co ma latać, wokół czego (pozycja początkowa) * jaka ma być szerokość orbity * jaka ma być prędkość (kątowa) * też wielkość orbity * rozmiar obiektu, kolor, roughness, metalic (odbicie światła) )
	drawObjectPBR(shipContext, glm::translate(pointlightPos) * glm::scale(glm::vec3(0.5)) * glm::eulerAngleY(time / 3) * glm::translate(glm::vec3(2.f, 0, 0)) * glm::scale(glm::vec3(0.01f)), glm::vec3(0.2, 0.7, 0.3), 0.3, 0.0);
	//drawObjectPBR(lampContext, glm::translate(lamplightPos) * glm::scale(wglm::vec3(0.1)) * glm::eulerAngleY(time / 3) * glm::translate(glm::vec3(4.f, 0, 0)) * glm::scale(glm::vec3(0.3f)), glm::vec3(0.2, 0.7, 0.3), 0.3, 0.0);

	// Dodatkowa planeta 2 - orbitujące coś
	//drawObjectPBR(sphereContext, glm::translate(pointlightPos) * glm::scale(glm::vec3(0.1)) * glm::eulerAngleY(time / 3) * glm::translate(glm::vec3(4.f, 0, 0)) * glm::eulerAngleY(time) * glm::translate(glm::vec3(1.f, 0, 0)) * glm::scale(glm::vec3(0.1f)), glm::vec3(0.5, 0.5, 0.5), 0.7, 0.0);
	//drawObjectPBR(lampContext, glm::translate(lamplightPos) * glm::scale(glm::vec3(0.1)) * glm::eulerAngleY(time / 3) * glm::translate(glm::vec3(4.f, 0, 0)) * glm::eulerAngleY(time) * glm::translate(glm::vec3(1.f, 0, 0)) * glm::scale(glm::vec3(0.1f)), glm::vec3(0.5, 0.5, 0.5), 0.7, 0.0);

	// models
	// Kolejność zmiennych drawObjectPBR(kontekst modelu, poruszanie razem ze sceną, (kolor obiektu), x,  odbijanie światła)
	drawObjectPBR(models::bedContext, glm::mat4(), glm::vec3(0.03f, 0.03f, 0.03f), 0.2f, 0.0f);
	drawObjectPBR(models::chairContext, glm::mat4(), glm::vec3(0.195239f, 0.37728f, 0.8f), 0.4f, 0.0f);
	drawObjectPBR(models::deskContext, glm::mat4(), glm::vec3(0.428691f, 0.08022f, 0.036889f), 0.2f, 0.0f);
	drawObjectPBR(models::doorContext, glm::mat4(), glm::vec3(0.402978f, 0.120509f, 0.057729f), 0.2f, 0.0f);
	drawObjectPBR(models::drawerContext, glm::mat4(), glm::vec3(0.428691f, 0.08022f, 0.036889f), 0.2f, 0.0f);
	drawObjectPBR(models::computerContext, glm::mat4(), glm::vec3(0.428691f, 0.08022f, 0.036889f), 0.2f, 0.0f);
	drawObjectPBR(models::materaceContext, glm::mat4(), glm::vec3(0.9f, 0.9f, 0.9f), 0.8f, 0.0f);
	drawObjectPBR(models::pencilsContext, glm::mat4(), glm::vec3(0.10039f, 0.018356f, 0.001935f), 0.1f, 0.0f);
	drawObjectPBR(models::planeContext, glm::mat4(), glm::vec3(0.402978f, 0.120509f, 0.057729f), 0.2f, 0.0f);
	drawObjectPBR(models::roomContext, glm::mat4(), glm::vec3(0.9f, 0.9f, 0.9f), 0.8f, 0.0f);
	drawObjectPBR(models::windowContext, glm::mat4(), glm::vec3(0.402978f, 0.120509f, 0.057729f), 0.2f, 0.0f);



	glm::vec3 spaceshipSide = glm::normalize(glm::cross(spaceshipDir, glm::vec3(0.f, 1.f, 0.f)));
	glm::vec3 spaceshipUp = glm::normalize(glm::cross(spaceshipSide, spaceshipDir));
	glm::mat4 specshipCameraRotrationMatrix = glm::mat4({
		spaceshipSide.x,spaceshipSide.y,spaceshipSide.z,0,
		spaceshipUp.x,spaceshipUp.y,spaceshipUp.z ,0,
		-spaceshipDir.x,-spaceshipDir.y,-spaceshipDir.z,0,
		0.,0.,0.,1.,
		});

	//drawObjectColor(shipContext,
	//	glm::translate(cameraPos + 1.5 * cameraDir + cameraUp * -0.5f) * inveseCameraRotrationMatrix * glm::eulerAngleY(glm::pi<float>()),
	//	glm::vec3(0.3, 0.3, 0.5)
	//	);

	// Ship 
	drawObjectPBR(shipContext,
		glm::translate(spaceshipPos) * specshipCameraRotrationMatrix * glm::eulerAngleY(glm::pi<float>()) * glm::scale(glm::vec3(0.03f)),
		glm::vec3(0.3, 0.3, 0.5),
		0.2,1.0
	);

	// spaceship
	spotlightPos = spaceshipPos + 0.2 * spaceshipDir;
	spotlightConeDir = spaceshipDir;
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.001f, 100.0f);
	glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content

	//skybox
	glUseProgram(programSkybox);
	glm::mat4 view = glm::mat4(glm::mat3(glm::lookAt(cameraPos, cameraPos + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f))));
	glUniformMatrix4fv(glGetUniformLocation(programSkybox, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(programSkybox, "projection"), 1, GL_FALSE, glm::value_ptr(viewProjectionMatrix));

	//skyboxcube
	glBindVertexArray(skyboxVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS);

	//test depth buffer
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glUseProgram(programTest);
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, depthMap);
	//Core::DrawContext(models::testContext);

	glUseProgram(0);
	glfwSwapBuffers(window);
}


void init(GLFWwindow* window)
{
	generateSkybox();


	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// wygląda jak test głębokości do cieni
	glEnable(GL_DEPTH_TEST);
	program = shaderLoader.CreateProgram("shaders/shader_9_1.vert", "shaders/shader_9_1.frag");
	programTest = shaderLoader.CreateProgram("shaders/test.vert", "shaders/test.frag");
	programSun = shaderLoader.CreateProgram("shaders/shader_8_sun.vert", "shaders/shader_8_sun.frag");
	programLamp = shaderLoader.CreateProgram("shaders/shader_8_sun.vert", "shaders/shader_8_sun.frag");
	programSkybox = shaderLoader.CreateProgram("shader_skybox.vert", "shader_skybox.frag");

	//SUN and LAMP models
	loadModelToContext("./models/sphere.obj", sphereContext);
	loadModelToContext("./models/lamp.obj", lampContext);


	loadModelToContext("./models/spaceship.obj", shipContext);
	loadModelToContext("./models/bed.obj", models::bedContext);
	loadModelToContext("./models/chair.obj", models::chairContext);
	loadModelToContext("./models/desk.obj", models::deskContext);
	loadModelToContext("./models/door.obj", models::doorContext);
	loadModelToContext("./models/drawer.obj", models::drawerContext);
	loadModelToContext("./models/marbleBust.obj", models::marbleBustContext);
	loadModelToContext("./models/materace.obj", models::materaceContext);
	loadModelToContext("./models/pencils.obj", models::pencilsContext);
	loadModelToContext("./models/plane.obj", models::planeContext);
	loadModelToContext("./models/room.obj", models::roomContext);
	loadModelToContext("./models/spaceship.obj", models::spaceshipContext);
	loadModelToContext("./models/sphere.obj", models::sphereContext);
	loadModelToContext("./models/window.obj", models::windowContext);
	loadModelToContext("./models/test.obj", models::testContext);
	loadModelToContext("./models/pc.obj", models::computerContext);


	glUseProgram(programSkybox);
	glUniform1i(glGetUniformLocation(programSkybox, "skybox"), 0);
}


// funkcja jest glowna petla
void renderLoop(GLFWwindow* window) {
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		renderScene(window);
		glfwPollEvents();
	}
}
//}