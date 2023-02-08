float aspectRatio = 1.f;

glm::mat4 createCameraMatrix()
{
	glm::vec3 cameraSide = glm::normalize(glm::cross(cameraDir, glm::vec3(0.f, 1.f, 0.f)));
	glm::vec3 cameraUp = glm::normalize(glm::cross(cameraSide, cameraDir));
	glm::mat4 cameraRotrationMatrix = glm::mat4({
		cameraSide.x,cameraSide.y,cameraSide.z,0,
		cameraUp.x,cameraUp.y,cameraUp.z ,0,
		-cameraDir.x,-cameraDir.y,-cameraDir.z,0,
		0.,0.,0.,1.,
		});
	cameraRotrationMatrix = glm::transpose(cameraRotrationMatrix);
	glm::mat4 cameraMatrix = cameraRotrationMatrix * glm::translate(-cameraPos);

	return cameraMatrix;
}

glm::mat4 createPerspectiveMatrix()
{
	glm::mat4 perspectiveMatrix;
	float n = 0.05;
	float f = 20.;
	float a1 = glm::min(aspectRatio, 1.f);
	float a2 = glm::min(1 / aspectRatio, 1.f);
	perspectiveMatrix = glm::mat4({
		1,0.,0.,0.,
		0.,aspectRatio,0.,0.,
		0.,0.,(f + n) / (n - f),2 * f * n / (n - f),
		0.,0.,-1.,0.,
		});

	perspectiveMatrix = glm::transpose(perspectiveMatrix);

	return perspectiveMatrix;
}