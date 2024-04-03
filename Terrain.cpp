#include "Terrain.h"

#include <vector>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <fstream>
#include <iostream>

Terrain::Terrain( glm::vec3 topLeft, glm::vec3 _size, bool _Textured, Texture* _Texture, float mass, float restitution, bool movable)
    : Models(mass, restitution, movable, _Textured, _Texture, topLeft, _size), size(_size)
{
    std::ifstream fichier("./ressources/map/tile01.txt");
    const int taille = 20; //Taille de lecture défini à 20 car on récupère un Terrain, A CHANGER POUR LES AUTRES OBJETS
    float vertices[taille];

    if (fichier) {
        std::cout << "Ouverture du fichier tile01.txt reussie !" << std::endl;

        for (int i = 0; i < taille; i++) {  
            float valeur;
            fichier >> valeur; // On lit un chiffre dans le fichier (déplace le curseur et s'arrête à l'espace)

            vertices[i] = valeur;

            std::cout << valeur << std::endl;
        }
        fichier.close();
    }
	else {
		std::cout << "Erreur lors de l'ouverture du fichier tile01.txt !" << std::endl;
    }

    // Define vertices for a flat grid
    //float vertices[] = {
    //    // Positions        // Texture Coords
    //    0.0f, 0.0f, 0.0f,    0.0f, 0.0f,
    //    1.0f, 0.0f, 0.0f,    1.0f, 0.0f,
    //    1.0f, 0.0f, 1.0f,    1.0f, 1.0f,
    //    0.0f, 0.0f, 1.0f,    0.0f, 1.0f,
    //};
    unsigned int indices[] = {
        0, 1, 2, // First Triangle
        2, 3, 0  // Second Triangle
    };

    Models::init(vertices, sizeof(vertices), indices, sizeof(indices));
}

Terrain::~Terrain() {
    Models::~Models();
}

void Terrain::Draw(Shader *shaderProgram, float dt) {

    rb.update(dt);
    glm::mat4 model = glm::translate(glm::mat4(1.0f), rb.position);
    model = glm::scale(model, size);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram->ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
    VAOModel.Bind();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    VAOModel.Unbind();
}

