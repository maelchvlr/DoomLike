#include "Terrain.h"

#include <vector>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

Terrain::Terrain(std::string path, glm::vec3 topLeft, glm::vec3 _size, bool _Textured, Texture* _Texture, float mass, float restitution, bool movable)
    : Models(mass, restitution, movable, _Textured, _Texture, topLeft, _size), size(_size)
{
    //Idem que pour le main, on ouvre le fichier puis on stocke les valeurs dans un tableau de vertices
    std::ifstream fichier(path);
    const int taille = 20; //Taille de lecture défini à 20 car on récupère un Terrain, A CHANGER POUR LES AUTRES OBJETS / à faire dynamiquement
    float vertices[taille];

    if (fichier) {
        //std::cout << "Ouverture du fichier " << path << " reussie !" << std::endl; //Debug purpose

        for (int i = 0; i < taille; i++) {
            float valeur;
            fichier >> valeur; //On lit un chiffre dans le fichier
            vertices[i] = valeur;
        }
        fichier.close();
    }
    else {
        std::cout << "Erreur lors de l'acquisition des coordonnées ! " << path << " " << std::endl;
    }

    unsigned int indices[] = {
        0, 1, 2, // First Triangle
        2, 3, 0  // Second Triangle
    };

    Models::init(vertices, sizeof(vertices), indices, sizeof(indices));
    //std::cout << "Terrain rb positon : " << rb.position.x << " " << rb.position.y << " " << rb.position.z << std::endl;
}

Terrain::~Terrain() {
    Models::~Models();
}

void Terrain::Draw(Shader *shaderProgram, float dt) {
    rb.update(dt);
    //Translation de la matrice identité 4x4 avec un vecteur nul pour éviter tout offset avec les coordonnées des Terrains
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f));
    model = glm::scale(model, size);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram->ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
    VAOModel.Bind();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    VAOModel.Unbind();
}