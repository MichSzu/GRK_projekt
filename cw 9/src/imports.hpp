#include "glew.h"
#include <GLFW/glfw3.h>
#include "glm.hpp"
#include "ext.hpp"
#include <iostream>
#include <cmath>

#include "Shader_Loader.h"
#include "Render_Utils.h"
#include "Texture.h"
#include "SOIL/SOIL.h"
#include "Box.cpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>

// pliki wydzielone z g³ównego rendera
#include "../src/skybox/skybox.hpp"
#include "../src/features/time_manager.hpp"
#include "../src/features/movement.hpp"
#include "../src/features/cameraMatrix.hpp"
#include "../src/features/app_controller.hpp"
#include "../src/models/loadModelToContext.hpp"
