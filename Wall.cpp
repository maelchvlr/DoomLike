#include "Wall.h"

#include <vector>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

Wall::Wall(std::string path, glm::vec3 topLeft, glm::vec3 _size, bool _Textured, Texture* _Texture, float mass, float restitution, bool movable)
    : Models(mass, restitution, movable, _Textured, _Texture, topLeft, _size)
{
    std::ifstream fichier(path);
    const int taille = 20; //Taille de lecture d�fini � 20 car on r�cup�re un Terrain, A CHANGER POUR LES AUTRES OBJETS / � faire dynamiquement
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
        std::cout << "Erreur lors de l'acquisition des coordonn�es ! " << path << " " << std::endl;
    }

    unsigned int indices[] = {
        0, 1, 2, // First Triangle
        2, 3, 0  // Second Triangle
    };

    Models::init(vertices, sizeof(vertices), indices, sizeof(indices));
    //std::cout << "Wall rb positon : " << rb.position.x << " " << rb.position.y << " " << rb.position.z << std::endl;
}

Wall::~Wall() {
	Models::~Models();
}

void Wall::Draw(Shader* shaderProgram, float dt) {
    rb.update(dt);
    //Translation de la matrice identit� 4x4 avec un vecteur nul pour �viter tout offset avec les coordonn�es des Terrains

    glm::mat4 mat = glm::mat4(1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1);
    size = glm::vec3(6, 6, 6);

    glm::mat4 model = glm::scale(mat, size);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram->ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
    VAOModel.Bind();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    VAOModel.Unbind();
}