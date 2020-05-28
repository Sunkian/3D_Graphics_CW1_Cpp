//
//  Mesh.h
//  SDL
//
//  Created by Alice Pagnoux on 04/12/2019.
//  Copyright © 2019 Alice Pagnoux. All rights reserved.
//
#pragma once

#include <assimp/types.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;

struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TextureCoordinates; //We only have 2d textures in an image
};

struct Texture
{
    GLuint id;
    string type;
    aiString path;
};

class Mesh
{
public:
    vector<Vertex> vertices;
    vector <GLuint> indices;
    vector <Texture> textures;
    
    /* Constructor */
    Mesh(vector<Vertex> vertices, vector <GLuint> indices, vector <Texture> textures)
    {
        this->vertices = vertices;
        this->indices = indices;
        this->textures = textures;
        
        /* Set up vertex buffers */
        this->SetupMesh();
    }
    
    void Draw(Shader shader)
    {
        GLuint diffuseNb = 1;
        GLuint specularNb = 1;
        
        for (GLuint i=0; i<this->textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i);
            stringstream ss;
            string number;
            string name = this->textures[i].type;
            
            /* Is it texture diffuse or texture specular */
            if ("texture_diffuse" == name)
            {
                ss << diffuseNb++;
            }
            else if ("texture_specular" == name)
            {
                ss << specularNb++;
            }
            
            number = ss.str();
            glUniform1i(glGetUniformLocation(shader.Program, (name + number).c_str()), i);
            /* Binding the texture */
            glBindTexture(GL_TEXTURE_2D, this->textures[i].id);
            // texture is applied until another activated
        }
        
        glUniform1f(glGetUniformLocation(shader.Program, "material.shininess"), 16.0f);
        glBindVertexArray(this->vao);
        glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
        /* Bind the vertex array */
        glBindVertexArray(0);
        
        for (GLuint i=0; i<this->textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    }
private:
    GLuint vao, vbo, ebo;
    void SetupMesh()
    {
        glGenVertexArrays(1, &this->vao);
        glGenBuffers(1, &this->vbo);
        glGenBuffers(1, &this->ebo);
        
        glBindVertexArray(this->vao);
        
        /* Load the data into the vertex buffers */
        glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
        glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW);
        
        /* Bind the buffer for the element buffer object (ebo) */
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), &this->indices[0], GL_STATIC_DRAW);
        
        /* Vertex attribute pointers */
        // Vertex positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0,3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid * )0);
        //Vertex normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1,3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid * )offsetof(Vertex, Normal));
        //Vertex texture coordinates
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2,3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid * )offsetof(Vertex, TextureCoordinates));
        
        glBindVertexArray(0);

    }
};
