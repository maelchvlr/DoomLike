#include "glew.h"
#include "glfw3.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <filesystem>

#define STB_IMAGE_IMPLEMENTATION
#include "Terrain.h"
#include "Texture.h"
#include "Models.h"
#include "Cube.h"
#include "Camera.h"

#include "Shader.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

#include "Collision.h"

const unsigned int width = 800;
const unsigned int height = 800;

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return -1;
    }
    else
        std::cout << "GLFW initialized" << std::endl;

    // Create a windowed mode window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(width, height, "DoomLike", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    if (glewInit() != GLEW_OK)
        std::cout << "Failed to initialize GLEW" << std::endl;
    else
        std::cout << "GLEW initialized" << std::endl;

    // Create textures
    Texture dirtTex = Texture("ressources/textures/dirt.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
    Texture crateTex = Texture("ressources/textures/crate.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);

    // Create the models
    Cube cube1 = Cube(glm::vec3(1.4, 4, 1), glm::vec3(1), true, &crateTex, 0.1f, 1);
    Cube cube2 = Cube(glm::vec3(1, 3, 1), glm::vec3(1), true, &crateTex, 0.5f);
    Cube cube3 = Cube(glm::vec3(3, 4, 3), glm::vec3(1), true, &crateTex, 2.f, 0);
    Cube cube4 = Cube(glm::vec3(3, 4, 1), glm::vec3(1), true, &crateTex, 0.05f);

    //Terrain initializer
    std::vector<Terrain*> terrains;

    //D�fini le path vers le dossier des terrains et it�re dedans
    std::filesystem::path terrainPath = "./ressources/map/";
    for (const auto& terrain : std::filesystem::directory_iterator(terrainPath)) {
        //Ouverture du fichier cible dans le dossier map
        std::ifstream fichier(terrain.path().string());

        //On r�cup�re les coordonn�es du coin sup�rieur gauche du terrain (Les 3 premi�res valeurs du fichier cible)
        const int taille = 3;
        //On d�finit une taille fixe pour scale les morceau de terrain (arbitraire)
        glm::vec3 scale = glm::vec3(6, 0, 6);
        //On initialise un tableau de float pour stocker les valeurs du topLeftCorner initial
        float topLeftCoord[taille];

        if (fichier) {
            for (int i = 0; i < taille; i++) {
                float valeur;
                fichier >> valeur;  // On lit un chiffre dans le fichier et on d�place le curseur sur le prochain float (type de la variable valeur)
                topLeftCoord[i] = valeur;   //On stock les valeurs pour le topLeftCorner
            }
            fichier.close();
        }
        else {
            std::cout << "Erreur lors de l'acquisition du TopLeftCorner ! " << terrain.path().string() << " " << std::endl;
        }

        //On scale le topLeftCorner en fonction de la taille du terrain (scale)
        glm::vec3 topLeftCorner = glm::vec3(topLeftCoord[0] * scale.x, topLeftCoord[1] * scale.y, topLeftCoord[2] * scale.z);
        //On d�finit le centre du terrain en fonction du topLeftCorner et du scale pour �viter les d�calages avec le rigidBody
        glm::vec3 center = topLeftCorner + scale / 2.0f;
        
        //On initialise le terrain et on le stocke dans le vecteur des terrains
        Terrain* newTerrain = new Terrain(terrain.path().string(), center, scale, false, nullptr, 50.0f, 0.5);
        terrains.push_back(newTerrain);
    }
    std::cout << "Terrains loaded" << std::endl;

    // Load the shader program
    Shader shaderProgram = Shader("VertexShader.glsl", "FragmentShader.glsl");

    // Create the camera
    Camera* camera = new Camera(width, height, glm::vec3(0.0f, 4.0f, 2.0f), &shaderProgram.ID);

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    double lastFrame = 0;
    double deltaTime = 0;

    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window)) {
        // Delta time = time between two frames
        double currentTime = glfwGetTime();
        deltaTime = currentTime - lastFrame;
        lastFrame = currentTime;

        // Render clearing
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClearDepth(1.0f);

        // Use the shader program
        shaderProgram.Activate();

        // Draw the cube(s)
        glUniform3f(glGetUniformLocation(shaderProgram.ID, "color"), 0.0f, 1.0f, 0.0f);
        cube1.Draw(&shaderProgram, deltaTime);
        glUniform3f(glGetUniformLocation(shaderProgram.ID, "color"), 1.0f, 0.0f, 0.0f);
        cube2.Draw(&shaderProgram, deltaTime);
        glUniform3f(glGetUniformLocation(shaderProgram.ID, "color"), 1.0f, 0.0f, 1.0f);
        cube3.Draw(&shaderProgram, deltaTime);
        glUniform3f(glGetUniformLocation(shaderProgram.ID, "color"), 0.0f, 1.0f, 1.0f);
        cube4.Draw(&shaderProgram, deltaTime);

        // Draw the terrain(s)
        glUniform3f(glGetUniformLocation(shaderProgram.ID, "color"), 0.0f, 0.0f, 1.0f);
        dirtTex.texUnit(shaderProgram, "tex0", 0);
        glUniform1i(glGetUniformLocation(shaderProgram.ID, "useTexture"), 1);
        dirtTex.Bind();

        for (auto terrain : terrains) {
            terrain->Draw(&shaderProgram, deltaTime);
        }

        dirtTex.Unbind();
        glUniform1i(glGetUniformLocation(shaderProgram.ID, "useTexture"), 0);

        // Handle collisions
        for (Terrain* terrain : terrains) {
            // Camera x terrains
            handlePredictiveCollision(camera->rb, terrain->getRigidBody(), deltaTime, "camera terrain");

            // Cubes x terrains
            handlePredictiveCollision(cube2.getRigidBody(), terrain->getRigidBody(), deltaTime, "cube2 terrain");
            handlePredictiveCollision(cube1.getRigidBody(), terrain->getRigidBody(), deltaTime, "cube1 terrain");
            handlePredictiveCollision(cube3.getRigidBody(), terrain->getRigidBody(), deltaTime, "cube3 terrain1");
            handlePredictiveCollision(cube4.getRigidBody(), terrain->getRigidBody(), deltaTime, "cube4 terrain1");
        }

        // Cubes x Cubes collisions
        handlePredictiveCollision(cube1.getRigidBody(), cube2.getRigidBody(), deltaTime, "cube1 cube2");

        // Camera
        camera->Inputs(window);
        camera->Matrix(45.0f, 0.1f, 100.0f, "camMatrix", &shaderProgram, deltaTime);

        glfwSwapBuffers(window);    // Swap front and back buffers
        glfwPollEvents();	        // Poll for and process events

        // Close the window if the escape key is pressed
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }
    }

    // De-allocate resources
    shaderProgram.Delete();
    cube1.~Cube();
    cube2.~Cube();
    cube3.~Cube();
    cube4.~Cube();

    for (auto terrain : terrains)
        terrain->~Terrain();

    // Terminate GLFW
    glfwTerminate();

	return 0;
}