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
#include "Wall.h"

#include "Actor.h"
#include "Player.h"
#include "Enemy.h"

#include "Shader.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

#include "Collision.h"

const unsigned int width = 1000;
const unsigned int height = 1000;

void drawCrosshair();
bool verifVerticesBy5(float*, int);

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

    //Terrain initializer
    std::vector<Terrain*> terrains;

    //Wall initializer
    std::vector<Wall*> walls;

    //Cube initializer
    std::vector<Cube*> cubes;

    // Cubes
    Cube* newCube1 = new Cube(glm::vec3(1.4, 4, 1), glm::vec3(1), true, &crateTex, 2.f, 0.5f);
    //cubes.push_back(newCube1);
    Cube* newCube2 = new Cube(glm::vec3(1, 3, 1), glm::vec3(1), true, &crateTex, 2.f, 0.5f);
    //cubes.push_back(newCube2);
    Cube* newCube3 = new Cube(glm::vec3(3, 4, 4), glm::vec3(1), true, &crateTex, 2.f, 0.5f);
    cubes.push_back(newCube3);
    Cube* newCube4 = new Cube(glm::vec3(3, 4, 1), glm::vec3(1), true, &crateTex, 2.f, 0.5f);
    //cubes.push_back(newCube4);

    Cube* raycastCube = new Cube(glm::vec3(0, 0, 0), glm::vec3(0.2), true, &crateTex, 2.f, 0.5f);
    cubes.push_back(raycastCube);

    // Terrains

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

    // Walls

    std::filesystem::path wallPath = "./ressources/wall/";
    for (const auto& wall : std::filesystem::directory_iterator(wallPath)) {
        std::ifstream fichier(wall.path().string());

        //Etant donne que le tableau est entierement parcouru ici, plus la peine de construire le mur 
        // comme on le faisait avec le Terrain
        const int taille = 20;
        glm::vec3 scale = glm::vec3(6, 6, 6);
        float vertices[taille];

        if (fichier) {
            for (int i = 0; i < taille; i++) {
                float valeur;
                fichier >> valeur;  // On lit un chiffre dans le fichier et on deplace le curseur sur le prochain float (type de la variable valeur)
                vertices[i] = valeur;   //On stock les valeurs pour le topLeftCorner
            }
            fichier.close();
        }
        else {
            std::cout << "Erreur lors de l'acquisition du TopLeftCorner ! " << wall.path().string() << " " << std::endl;
        }

        //On scale le topLeftCorner en fonction de la taille du terrain (scale)
        glm::vec3 bottomLeftCorner = glm::vec3(vertices[0] * scale.x, vertices[1] * scale.y, vertices[2] * scale.z);

        //Verif la direction dans laquelle est orientee le mur
        glm::vec3 center;
        if (verifVerticesBy5(vertices, 0)) {
            scale = glm::vec3(0, scale.y, scale.z);
            center = glm::vec3(0, bottomLeftCorner.y + scale.y / 2.0f, bottomLeftCorner.z + scale.z / 2.0);
        }
        else if (verifVerticesBy5(vertices, 1)) {
            scale = glm::vec3(scale.x, 0, scale.z);    //En soit, un mur avec un y a 0 est un sol ? (a creuser)
            center = glm::vec3(bottomLeftCorner.x + scale.x / 2.0f, 0, bottomLeftCorner.z + scale.z / 2.0);
        }
        else if (verifVerticesBy5(vertices, 2)) {   //Derniere verification non obligatoire
            scale = glm::vec3(scale.x, scale.y, 0);
            center = glm::vec3(bottomLeftCorner.x + scale.x / 2.0f, bottomLeftCorner.y + scale.y / 2.0f, 0);
        }

        //On initialise le terrain et on le stocke dans le vecteur des terrains
        Wall* newWall = new Wall(wall.path().string(), center, scale, false, nullptr, 50.0f, 0.5);
        walls.push_back(newWall);
    }
    std::cout << "Walls loaded" << std::endl;

    // Load the shader program
    Shader shaderProgram = Shader("VertexShader.glsl", "FragmentShader.glsl");

    // Create the camera
    Camera* camera = new Camera(width, height, glm::vec3(0.0f, 4.0f, 2.0f), &shaderProgram.ID);

    // Create the orthographic projection matrix
    glm::mat4 orthoProjection = glm::ortho(0.0f, (float)width, (float)height, 0.0f, -1.0f, 1.0f);

    // Player
    Player player = Player(camera->rb->getPosition(), *camera);

    // raycast
    std::vector<glm::vec3> raycast;

    // Enemy
    Enemy enemy(glm::vec3(5.0f, 1.0f, 5.0f), 2.0f);
    bool enemyAlive = true;

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

        glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "orthoProjection"), 1, GL_FALSE, glm::value_ptr(orthoProjection));
        glUniform3f(glGetUniformLocation(shaderProgram.ID, "color"), 1.0f, 1.0f, 1.0f);
        glUniform1i(glGetUniformLocation(shaderProgram.ID, "is2D"), 1);
        drawCrosshair();
        glUniform1i(glGetUniformLocation(shaderProgram.ID, "is2D"), 0);

        // handle enemy
        if(enemyAlive) {
			enemy.Update(deltaTime);
			enemy.Render(&shaderProgram, deltaTime);
			enemy.IsPlayerInRadius(player.getRigidBodyPosition());
		}

        // Draw the cube(s)
        for (Cube* cube : cubes) {
            glUniform3f(glGetUniformLocation(shaderProgram.ID, "color"), 0.0f, 1.0f, 0.0f);
			cube->Draw(&shaderProgram, deltaTime);
		}

        // Draw the terrain(s)
        glUniform3f(glGetUniformLocation(shaderProgram.ID, "color"), 0.0f, 0.0f, 1.0f);
        dirtTex.texUnit(shaderProgram, "tex0", 0);
        glUniform1i(glGetUniformLocation(shaderProgram.ID, "useTexture"), 1);
        dirtTex.Bind();

        for (auto terrain : terrains) {
            terrain->Draw(&shaderProgram, deltaTime);
        }

        for (auto wall : walls) {
            wall->Draw(&shaderProgram, deltaTime);
        }

        dirtTex.Unbind();
        glUniform1i(glGetUniformLocation(shaderProgram.ID, "useTexture"), 0);

        // Handle collisions
        for (Terrain* terrain : terrains) {
            for (Cube* cube : cubes) {
                // Cubes x terrains
                handlePredictiveCollision(cube->getRigidBody(), terrain->getRigidBody(), deltaTime, "cube x terrain");
            }
            // Camera x terrains
            handlePredictiveCollision(camera->rb, terrain->getRigidBody(), deltaTime, "camera x terrain");
            handlePredictiveCollision(enemy.GetRigidBody(), terrain->getRigidBody(), deltaTime, "player x terrain");
        }

        for (Cube* cube1 : cubes) {
            for (Cube* cube2 : cubes) {
				if (cube1 == cube2) continue;
				// Cube x Cube collisions
                handlePredictiveCollision(cube1->getRigidBody(), cube2->getRigidBody(), deltaTime, "cube x cube");
			}
            // Camera x cubes
            handlePredictiveCollision(camera->rb, cube1->getRigidBody(), deltaTime, "camera x cube");
        }
        
        for (Wall* wall : walls) {
            // Camera x wall
            handlePredictiveCollision(camera->rb, wall->getRigidBody(), deltaTime, "camera x wall");
        }

        // Player
        player.Update(deltaTime);
        player.UpdateCamera(deltaTime, window, shaderProgram);
        raycast = player.RayCast(window);

        if (!(raycast[1] == glm::vec3(0.0f, 0.0f, 0.0f)))
        {
            // Calculate the vector from the player's position to the enemy's position
            glm::vec3 playerToEnemy = enemy.GetPosition() - raycast[0];

            // Calculate the distance from the player's position to the enemy's position projected onto the ray direction
            float distanceAlongRay = glm::dot(playerToEnemy, glm::normalize(raycast[1] - raycast[0]));

            // Check if the enemy is in front of the player and within the range of the raycast
            if (distanceAlongRay > 0 && distanceAlongRay < glm::length(raycast[1] - raycast[0])) {
                // Calculate the angle between the player's view direction and the vector from the player to the enemy
                float angle = glm::degrees(glm::acos(glm::dot(glm::normalize(playerToEnemy), glm::normalize(raycast[1] - raycast[0]))));

                // Check if the angle is within a certain threshold (e.g., player's field of view)
                float fieldOfViewThreshold = 1.0f; // Adjust this threshold as needed
                if (angle < fieldOfViewThreshold) {
                    // The enemy is in the player's line of sight
                    std::cout << "Enemy detected in line of sight!" << std::endl;
                    enemyAlive = false;
                    enemy.Destroy();
                }
            }
        }

        glfwSwapBuffers(window);    // Swap front and back buffers
        glfwPollEvents();	        // Poll for and process events
    }

    // De-allocate resources
    shaderProgram.Delete();

    for (auto cube : cubes)
		cube->~Cube();

    for (auto terrain : terrains)
        terrain->~Terrain();

    for (auto wall : walls)
        wall->~Wall();

    // Terminate GLFW
    glfwTerminate();

	return 0;
}


void drawCrosshair() {

    // taille des branches du viseur
    float crosshairSize = 10.0f;

    // Position du centre du viseur
    float centerX = width / 2.0f;
    float centerY = height / 2.0f;

    // Vertices du viseur
    GLfloat vertices[] = {
        // Horizontal line
        centerX - crosshairSize, centerY, 0.0f,
        centerX + crosshairSize, centerY, 0.0f,
        // Vertical line
        centerX, centerY - crosshairSize, 0.0f,
        centerX, centerY + crosshairSize, 0.0f
    };


    // setup des VAO et VBO
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Render le crosshair
    glBindVertexArray(VAO);
    glDrawArrays(GL_LINES, 0, 4);
    glBindVertexArray(0);

    // Cleanup
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
}

// Prend un entier i et verifie avec un offset de 5 les valeurs du tableau
// Cette fonction est a utiliser specifiquement pour la verification des vertices
bool verifVerticesBy5(float *vertices, int i) {
    if (vertices[i] == vertices[i + 5]) {           //Exemple : 2 et 7
        i += 5;
        if (vertices[i] == vertices[i + 5]) {       // 7 et 12
            i += 5;
            if (vertices[i] == vertices[i + 5]) {   // 12 et 17
                return true;
            }
        }
    }
    return false;
}