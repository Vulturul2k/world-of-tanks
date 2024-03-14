#include "lab_m1/Tema2/Tema2.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */

struct cladire {
    float x;
    float y;
    float scale;
    float scaleY;
};

cladire cladiri[32];
float red;
float blue;
float green;
struct proiectil {
     float rotationAngleProiectil;
     float timp;
     float x;
     float y;
     bool shoot;
 };

struct tank {
    float xTankT;
    float zTankT;
    float rotationAngleT;
    float rotationAngleTureta;
    bool shoot;
    float life;
    proiectil proiectil;
};

vector<tank> badTanks;
bool logan;

int numCladire;
tank myTank;
float globalTime;
int globalScoare;
Tema2::Tema2()
{
}


Tema2::~Tema2()
{
}
bool ok;
float lifeGlobal;
bool endGame;

void Tema2::Init()
{
    endGame = true;
    logan = false;
    renderCameraTarget = false;
    lifeGlobal = 5;
    projectionType = true;
    ok = true;
    right = 10.f;
    left = .01f;
    bottom = .01f;
    top = 10.f;
    fov = 40.f;
    globalScoare = 0;
    myTank.xTankT = 0;
    myTank.zTankT = 0;
    myTank.rotationAngleT = 0.0f;
    myTank.rotationAngleTureta = 0.0f;

    myTank.proiectil.x = 99999;
    myTank.proiectil.y = 99999;
    myTank.proiectil.shoot = false;
    myTank.life = 5;
    globalTime = 0;
    camera = new implemented::CameraTema();
    camera->Set(glm::vec3(0, 2, 3.5f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));

    {
        Mesh* mesh = new Mesh("plane");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "plane50.obj");
        meshes[mesh->GetMeshID()] = mesh;
        Mesh* mesh1 = new Mesh("senile");
        mesh1->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "senile.obj");
        meshes[mesh1->GetMeshID()] = mesh1;
        Mesh* mesh2 = new Mesh("corp");
        mesh2->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "corpTank.obj");
        meshes[mesh2->GetMeshID()] = mesh2;
        Mesh* mesh3 = new Mesh("turela");
        mesh3->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "turela.obj");
        meshes[mesh3->GetMeshID()] = mesh3;
        Mesh* mesh4 = new Mesh("tun");
        mesh4->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "tun.obj");
        meshes[mesh4->GetMeshID()] = mesh4;        
        Mesh* mesh5 = new Mesh("logan");
        mesh5->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "logan.obj");
        meshes[mesh5->GetMeshID()] = mesh5;
    }

    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    // TODO(student): After you implement the changing of the projection
    // parameters, remove hardcodings of these parameters
    projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);


    {
        vector<VertexFormat> vertices
        {
            VertexFormat(glm::vec3(-1, -1,  1), glm::vec3(0, 1, 1), glm::vec3(0.2, 0.8, 0.2)),
            VertexFormat(glm::vec3(1, -1,  1), glm::vec3(1, 0, 1), glm::vec3(0.9, 0.4, 0.2)),
            VertexFormat(glm::vec3(-1,  1,  1), glm::vec3(1, 0, 0), glm::vec3(0.7, 0.7, 0.1)),
            VertexFormat(glm::vec3(1,  1,  1), glm::vec3(0, 1, 0), glm::vec3(0.7, 0.3, 0.7)),
            VertexFormat(glm::vec3(-1, -1, -1), glm::vec3(1, 1, 1), glm::vec3(0.3, 0.5, 0.4)),
            VertexFormat(glm::vec3(1, -1, -1), glm::vec3(0, 1, 1), glm::vec3(0.5, 0.2, 0.9)),
            VertexFormat(glm::vec3(-1,  1, -1), glm::vec3(1, 1, 0), glm::vec3(0.7, 0.0, 0.7)),
            VertexFormat(glm::vec3(1,  1, -1), glm::vec3(0, 0, 1), glm::vec3(0.1, 0.5, 0.8)),
        };

        vector<unsigned int> indices =
        {
            0, 1, 2,        1, 3, 2,
            2, 3, 7,        2, 7, 6,
            1, 7, 3,        1, 5, 7,
            6, 7, 4,        7, 5, 4,
            0, 4, 1,        1, 4, 5,
            2, 6, 4,        0, 2, 4,
        };

        CreateMesh("cube", vertices, indices);
    }

    {
        Shader* shader = new Shader("LabShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }
    numCladire = 30;
    for (int i = 0; i < numCladire; i++) {
        cladiri[i].x = rand() % 200 - 100;
        cladiri[i].y = rand() % 200 - 100;
        cladiri[i].scale = rand() % 4 + 2;
        cladiri[i].scaleY = rand() % 4 + 3;
        bool ok = true;
        while (ok)
        {
            if (abs(cladiri[i].x) > 3 + cladiri[i].scale || abs(cladiri[i].y) > 3 + cladiri[i].scale)
                ok = false;
            else 
            {
                cladiri[i].x = rand() % 200 - 100;
                cladiri[i].y = rand() % 200 - 100;
            }
        }



    }
    for (int i = 0; i < 7; i++) {
        tank newTank;
        newTank.life = 3;
        newTank.xTankT = rand() % 108 - 54;
        newTank.zTankT = rand() % 108 - 54;
        newTank.rotationAngleT = rand() % 5;
        bool ok = true;
        while (ok)
        {
            if (newTank.xTankT > 10 || newTank.xTankT < -10
                || newTank.zTankT > 10 || newTank.zTankT < -10) {
                bool okCladire = true;
                for (int j = 0; j < numCladire; j++) {
                    if (abs(newTank.xTankT - cladiri[j].x) < cladiri[j].scale + 5
                        && abs(newTank.zTankT - cladiri[j].y) < cladiri[j].scale + 5)
                        okCladire = false;
                }
                
                bool okTanks = true;
                for (auto it = badTanks.begin(); it != badTanks.end();) {
                    if (abs(newTank.xTankT - it->xTankT) < 10 && abs(newTank.zTankT - it->zTankT) < 10)
                    {
                        okTanks = false;;
                    }
                    it++;
                }
                if (okCladire && okTanks) {
                    ok = false;
                }

            } 
            if (ok)
            {
                newTank.xTankT = rand() % 108 - 54;
                newTank.zTankT = rand() % 108 - 54;
            }
        }
        newTank.proiectil.x = 99999;
        newTank.proiectil.y = 99999;
        newTank.proiectil.shoot = false;
        newTank.proiectil.timp = 0;
        badTanks.push_back(newTank);
    }
}
// functie preluata din laborator
Mesh* Tema2::CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned int>& indices)
{
    unsigned int VAO = 0;
    // Create the VAO and bind it
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Create the VBO and bind it
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Send vertices data into the VBO buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

    // Create the IBO and bind it
    unsigned int IBO;
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

    // Send indices data into the IBO buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

    // ========================================================================
    // This section demonstrates how the GPU vertex shader program
    // receives data.

    // TODO(student): If you look closely in the `Init()` and `Update()`
    // functions, you will see that we have three objects which we load
    // and use in three different ways:
    // - LoadMesh   + LabShader (this lab's shader)
    // - CreateMesh + VertexNormal (this shader is already implemented)
    // - CreateMesh + LabShader (this lab's shader)
    // To get an idea about how they're different from one another, do the
    // following experiments. What happens if you switch the color pipe and
    // normal pipe in this function (but not in the shader)? Now, what happens
    // if you do the same thing in the shader (but not in this function)?
    // Finally, what happens if you do the same thing in both places? Why?

    // Set vertex position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);

    // Set vertex normal attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));

    // Set texture coordinate attribute
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3)));

    // Set vertex color attribute
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));
    // ========================================================================

    // Unbind the VAO
    glBindVertexArray(0);

    // Check for OpenGL errors
    CheckOpenGLError();

    // Mesh information is saved into a Mesh object
    meshes[name] = new Mesh(name);
    meshes[name]->InitFromBuffer(VAO, static_cast<unsigned int>(indices.size()));
    meshes[name]->vertices = vertices;
    meshes[name]->indices = indices;
    return meshes[name];
}


void Tema2::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}

void Tema2::Cladire() {
    for (int i = 0; i < numCladire; i++) {
        {
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(cladiri[i].x, 0, cladiri[i].y));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(1.0f, cladiri[i].scaleY, 1.0f));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(cladiri[i].scale));
            RenderMesh(meshes["cube"], shaders["VertexNormal"], modelMatrix);

        }
        if (abs(myTank.xTankT - cladiri[i].x) < cladiri[i].scale + 3.2 && abs(myTank.zTankT - cladiri[i].y) < cladiri[i].scale + 3.2) {
            glm::vec3 Centru_Tanc_1 = glm::vec3(myTank.xTankT, 1, myTank.zTankT);
            glm::vec3 Centru_Tanc_2 = glm::vec3(cladiri[i].x, 1, cladiri[i].y);
            glm::vec3 P = glm::vec3(cladiri[i].scale + 3.2 - abs(myTank.xTankT - cladiri[i].x), 1, cladiri[i].scale + 3.2 - abs(myTank.zTankT - cladiri[i].y));
            glm::vec3 Dif = Centru_Tanc_2 - Centru_Tanc_1;
            P = abs(P) * glm::normalize(Dif);
            myTank.xTankT += (-0.5f * P.x);
            myTank.zTankT += (-0.5f * P.z);
        }

        if (abs(myTank.proiectil.x - cladiri[i].x) < cladiri[i].scale && abs(myTank.proiectil.y - cladiri[i].y) < cladiri[i].scale) {
            myTank.proiectil.x = 99999;
            myTank.proiectil.y = 99999;
            myTank.proiectil.timp = 0;
            myTank.proiectil.rotationAngleProiectil = 0;
        }

        for (auto its = badTanks.begin(); its != badTanks.end();) {
            if (abs(its->xTankT - cladiri[i].x) < cladiri[i].scale + 3.2 && abs(its->zTankT - cladiri[i].y) < cladiri[i].scale + 3.2) {
                glm::vec3 Centru_Tanc_1 = glm::vec3(its->xTankT, 1, its->zTankT);
                glm::vec3 Centru_Tanc_2 = glm::vec3(cladiri[i].x, 1, cladiri[i].y);
                glm::vec3 P = glm::vec3(cladiri[i].scale + 3.2 - abs(its->xTankT - cladiri[i].x), 1, cladiri[i].scale + 3.2 - abs(its->zTankT - cladiri[i].y));
                glm::vec3 Dif = Centru_Tanc_2 - Centru_Tanc_1;
                P = abs(P) * glm::normalize(Dif);
                its->xTankT += (-0.5f * P.x);
                its->zTankT += (-0.5f * P.z);
            }
            its++;

        }
    }
}

void Tema2::BadTank(float deltaTimeSeconds)
{


    for (auto it = badTanks.begin(); it != badTanks.end();) {
        if (it->life < 0) (it->life) = 0;
        lifeGlobal = (it->life + 1) / 4;
        {
            glm::mat4 modelMatrix = glm::mat4(1);
            red = 0;
            green = 0.5f;
            blue = 1;
            modelMatrix = glm::translate(modelMatrix, glm::vec3(it->xTankT, 1, it->zTankT));
            modelMatrix = glm::rotate(modelMatrix, it->rotationAngleT, glm::vec3(0, 1, 0));
            RenderSimpleMesh(meshes["corp"], shaders["LabShader"], modelMatrix);
        }
        {
            blue = 0;
            green = 0.5f;
            red = 1;
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(it->xTankT, 1, it->zTankT));
            modelMatrix = glm::rotate(modelMatrix, it->rotationAngleTureta, glm::vec3(0, 1, 0));
            RenderSimpleMesh(meshes["turela"], shaders["LabShader"], modelMatrix);
        }
        {
            red = 0;
            blue = 0.5f;
            green = 1;
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(it->xTankT, 1, it->zTankT));
            modelMatrix = glm::rotate(modelMatrix, it->rotationAngleTureta, glm::vec3(0, 1, 0));
            RenderSimpleMesh(meshes["tun"], shaders["LabShader"], modelMatrix);
        }

        {
            red = 0;
            blue = 0.5f;
            green = 1;
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(it->xTankT, 1, it->zTankT));
            modelMatrix = glm::rotate(modelMatrix, it->rotationAngleT, glm::vec3(0, 1, 0));
            RenderMesh(meshes["senile"], shaders["LabShader"], modelMatrix);

        }
        lifeGlobal = 1;

        bool dontMove = false;
        {
            if (abs(myTank.xTankT - it->xTankT) < 18.2f && abs(myTank.zTankT - it->zTankT) < 18.2f && it->life > 0) {
                glm::vec3 direction = glm::normalize(glm::vec3(-it->xTankT + myTank.xTankT, 0.0f, -it->zTankT + myTank.zTankT));
                float angle = atan2(direction.z, direction.x);
                it->rotationAngleTureta = 3.14 - angle;
                dontMove = true;
                if (it->proiectil.x == 99999 && it->proiectil.y == 99999 && globalTime < 100 && myTank.life>0 && (it->life > 0))
                {
                    it->proiectil.shoot = true;
                }
            }


            if (it->proiectil.shoot) {
                it->proiectil.x = it->xTankT - (3) * cos(it->rotationAngleTureta);
                it->proiectil.y = it->zTankT + (3) * sin(it->rotationAngleTureta);
                it->proiectil.shoot = false;
                it->proiectil.rotationAngleProiectil = it->rotationAngleTureta;
            }

            if (it->proiectil.x != 99999 && it->proiectil.y != 99999) {

                it->proiectil.x = it->proiectil.x - (deltaTimeSeconds * 10) * cos(it->proiectil.rotationAngleProiectil);
                it->proiectil.y = it->proiectil.y + (deltaTimeSeconds * 10) * sin(it->proiectil.rotationAngleProiectil);
                {
                    glm::mat4 modelMatrix = glm::mat4(1);
                    red = 0;
                    green = 0;
                    blue = 0;
                    modelMatrix = glm::translate(modelMatrix, glm::vec3(it->proiectil.x, 3.2f, it->proiectil.y));
                    modelMatrix = glm::rotate(modelMatrix, it->proiectil.rotationAngleProiectil, glm::vec3(0, 1, 0));
                    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f));
                    RenderSimpleMesh(meshes["sphere"], shaders["LabShader"], modelMatrix);
                }
                it->proiectil.timp += deltaTimeSeconds;
                if (abs(myTank.xTankT - it->proiectil.x) < 3.4f && abs(myTank.zTankT - it->proiectil.y) < 3.4f) {
                    myTank.life--;
                    it->proiectil.x = 99998;
                    it->proiectil.y = 99998;
                    it->proiectil.timp = 0;
                    it->proiectil.rotationAngleProiectil = 0;
                }
                if (it->proiectil.timp > 7) {
                    it->proiectil.x = 99999;
                    it->proiectil.y = 99999;
                    it->proiectil.timp = 0;
                    it->proiectil.rotationAngleProiectil = 0;

                }
            }



            if (abs(myTank.proiectil.x - it->xTankT) < 3.4f && abs(myTank.proiectil.y - it->zTankT) < 3.4f) {
                myTank.proiectil.x = 99999;
                myTank.proiectil.y = 99999;
                myTank.proiectil.timp = 0;
                myTank.proiectil.rotationAngleProiectil = 0;
                it->life--;
                if (it->life == 0) {
                    globalScoare++;
                }
            }

            if (abs(myTank.xTankT - it->xTankT) < 7.2f && abs(myTank.zTankT - it->zTankT) < 7.2f) {
                glm::vec3 Centru_Tanc_1 = glm::vec3(myTank.xTankT, 1, myTank.zTankT);
                glm::vec3 Centru_Tanc_2 = glm::vec3(it->xTankT, 1, it->zTankT);
                glm::vec3 P = glm::vec3(7.5f - abs(myTank.xTankT - it->xTankT), 1, 7.2f - abs(myTank.zTankT - it->zTankT));
                glm::vec3 Dif = Centru_Tanc_2 - Centru_Tanc_1;
                P = abs(P) * glm::normalize(Dif);
                myTank.xTankT += (-0.5f * P.x);
                myTank.zTankT += (-0.5f * P.z);
                it->xTankT += (0.5f * P.x);
                it->zTankT += (0.5f * P.z);
            }
            for (auto its = badTanks.begin(); its != badTanks.end();) {
                if (its->xTankT == it->xTankT && it->zTankT == its->zTankT) {
                    its++;
                }
                else
                {
                    if (abs(its->xTankT - it->xTankT) < 7.2f && abs(its->zTankT - it->zTankT) < 7.2f) {
                        glm::vec3 Centru_Tanc_1 = glm::vec3(its->xTankT, 1, its->zTankT);
                        glm::vec3 Centru_Tanc_2 = glm::vec3(it->xTankT, 1, it->zTankT);
                        glm::vec3 P = glm::vec3(7.5f - abs(its->xTankT - it->xTankT), 1, 7.2f - abs(its->zTankT - it->zTankT));
                        glm::vec3 Dif = Centru_Tanc_2 - Centru_Tanc_1;
                        P = abs(P) * glm::normalize(Dif);
                        its->xTankT += (-0.5f * P.x);
                        its->zTankT += (-0.5f * P.z);
                        it->xTankT += (0.5f * P.x);
                        it->zTankT += (0.5f * P.z);
                    }
                    its++;
                }

            }
        }
        if (it->life > 0 && !dontMove && globalTime < 100 && myTank.life>0)
        {
            int random = rand() % 7 + 1;
            switch (random) {
            case 1:
                it->rotationAngleT += deltaTimeSeconds;
                it->rotationAngleTureta += deltaTimeSeconds;
                break;
            case 2:
                it->rotationAngleT -= deltaTimeSeconds;
                it->rotationAngleTureta -= deltaTimeSeconds;
                break;
            case 3:
            case 4:
            case 5:
                if (abs(it->xTankT) < 110)
                {
                    it->xTankT += (deltaTimeSeconds * 30) * cos(myTank.rotationAngleT);
                }
                if (abs(it->xTankT) < 110)
                {
                    it->zTankT -= (deltaTimeSeconds * 30) * sin(myTank.rotationAngleT);
                }

                break;
            default:
                if (abs(it->zTankT) < 110)
                    it->xTankT -= (deltaTimeSeconds * 30) * cos(myTank.rotationAngleT);
                if (abs(it->zTankT) < 110)
                    it->zTankT += (deltaTimeSeconds * 30) * sin(myTank.rotationAngleT);
                break;
            }
        }

        it++;
    }
}

void Tema2::MyTank(float deltaTimeSeconds)
{
    lifeGlobal = myTank.life / 5;
    if (logan)
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(myTank.xTankT, 1, myTank.zTankT));
        modelMatrix = glm::rotate(modelMatrix, glm::radians(90.0f), glm::vec3(0, 1, 0));
        modelMatrix = glm::rotate(modelMatrix, myTank.rotationAngleT, glm::vec3(0, 1, 0));
        RenderSimpleMesh(meshes["logan"], shaders["VertexNormal"], modelMatrix);
    }

    if (!logan)
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        red = 0;
        green = 0;
        blue = 1;
        modelMatrix = glm::translate(modelMatrix, glm::vec3(myTank.xTankT, 1, myTank.zTankT));
        modelMatrix = glm::rotate(modelMatrix, myTank.rotationAngleT, glm::vec3(0, 1, 0));
        RenderSimpleMesh(meshes["corp"], shaders["LabShader"], modelMatrix);
    }
    if (!logan)
    {
        blue = 0;
        green = 0;
        red = 1;
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(myTank.xTankT, 1, myTank.zTankT));
        modelMatrix = glm::rotate(modelMatrix, myTank.rotationAngleTureta, glm::vec3(0, 1, 0));
        RenderSimpleMesh(meshes["turela"], shaders["LabShader"], modelMatrix);
    }
    {
        red = 0;
        blue = 0;
        green = 1;
        glm::mat4 modelMatrix = glm::mat4(1);
        if(!logan)
            modelMatrix = glm::translate(modelMatrix, glm::vec3(myTank.xTankT, 1, myTank.zTankT));
        else 
            modelMatrix = glm::translate(modelMatrix, glm::vec3(myTank.xTankT, 2, myTank.zTankT));

        modelMatrix = glm::rotate(modelMatrix, myTank.rotationAngleTureta, glm::vec3(0, 1, 0));
        RenderSimpleMesh(meshes["tun"], shaders["LabShader"], modelMatrix);
    }
    if (!logan)
    {
        red = 0;
        blue = 0;
        green = 1;
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(myTank.xTankT, 1, myTank.zTankT));
        modelMatrix = glm::rotate(modelMatrix, myTank.rotationAngleT, glm::vec3(0, 1, 0));
        RenderMesh(meshes["senile"], shaders["LabShader"], modelMatrix);

    }

    if (myTank.proiectil.shoot) {
        myTank.proiectil.x = myTank.xTankT - (3) * cos(myTank.rotationAngleTureta);
        myTank.proiectil.y = myTank.zTankT + (3) * sin(myTank.rotationAngleTureta);
        myTank.proiectil.shoot = false;
        myTank.proiectil.rotationAngleProiectil = myTank.rotationAngleTureta;
    }

    if (myTank.proiectil.x != 99999 && myTank.proiectil.y != 99999) {

        myTank.proiectil.x = myTank.proiectil.x - (deltaTimeSeconds * 10) * cos(myTank.proiectil.rotationAngleProiectil);
        myTank.proiectil.y = myTank.proiectil.y + (deltaTimeSeconds * 10) * sin(myTank.proiectil.rotationAngleProiectil);
        {
            glm::mat4 modelMatrix = glm::mat4(1);
            red = 0;
            green = 0;
            blue = 0;
            modelMatrix = glm::translate(modelMatrix, glm::vec3(myTank.proiectil.x, 3.2f, myTank.proiectil.y));
            modelMatrix = glm::rotate(modelMatrix, myTank.proiectil.rotationAngleProiectil, glm::vec3(0, 1, 0));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f));
            RenderSimpleMesh(meshes["sphere"], shaders["LabShader"], modelMatrix);
        }
        myTank.proiectil.timp += deltaTimeSeconds;
        if (myTank.proiectil.timp > 3) {
            myTank.proiectil.x = 99999;
            myTank.proiectil.y = 99999;
            myTank.proiectil.timp = 0;
            myTank.proiectil.rotationAngleProiectil = 0;

        }
    }
    lifeGlobal = 1;
}

void Tema2::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;


    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);



    // TODO(student): Get shader location for uniform mat4 "View"
    int location = glGetUniformLocation(shader->program, "View");

    // TODO(student): Set shader uniform "View" to viewMatrix
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));

    // TODO(student): Get shader location for uniform mat4 "Projection"
    location = glGetUniformLocation(shader->program, "Projection");

    // TODO(student): Set shader uniform "Projection" to projectionMatrix
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    // TODO(student): Get shader location for uniform mat4 "Model"
     location = glGetUniformLocation(shader->program, "Model");

    // TODO(student): Set shader uniform "Model" to modelMatrix
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    location = glGetUniformLocation(shader->program, "green");
    glUniform1f(location, green);

    location = glGetUniformLocation(shader->program, "red");
    glUniform1f(location, red);

    location = glGetUniformLocation(shader->program, "blue");
    glUniform1f(location, blue);    
    location = glGetUniformLocation(shader->program, "damageAmount");
    glUniform1f(location, lifeGlobal);


    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}


void Tema2::Update(float deltaTimeSeconds)
{
    globalTime += deltaTimeSeconds;

    BadTank(deltaTimeSeconds);
    if (!(globalTime < 100 && myTank.life>0)) {
        if (endGame)
        {
            cout << "ai un scor de " << globalScoare << endl;
            endGame = false;
        }
    }


    {

        glm::mat4 modelMatrix = glm::mat4(1);
        red = 0.6f;
        green = 0.3f;
        blue = 0.1f;
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(8));
        RenderSimpleMesh(meshes["plane"], shaders["LabShader"], modelMatrix);
    }
    MyTank(deltaTimeSeconds);
    Cladire();




    if (ok) {
        // imi muta camera la pozitia tancului apoi o roteste si o da mai in spate
        camera->MoveToPosition(glm::vec3(myTank.xTankT, 5, myTank.zTankT));
        camera->RotateFirstPerson_OY((myTank.rotationAngleT + 1.5f));
        camera->TranslateForward(-10);

    }

}


void Tema2::FrameEnd()
{
    //DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);
}


void Tema2::RenderMesh(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix)
{
    if (!mesh || !shader || !shader->program)
        return;

    // Render an object using the specified shader and the specified position
    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    mesh->Render();
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema2::OnInputUpdate(float deltaTime, int mods)
{
    // move the camera only if MOUSE_RIGHT button is pressed


    if (window->MouseHold(GLFW_MOUSE_BUTTON_LEFT)) {
        if ((globalTime < 100 && myTank.life>0)) {
            if (myTank.proiectil.timp == 0)
                myTank.proiectil.shoot = true;
        }
    }


    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        ok = false;
        float cameraSpeed = 2.0f;

        if (window->KeyHold(GLFW_KEY_W)) {
            camera->TranslateForward(deltaTime * cameraSpeed);

        }

        if (window->KeyHold(GLFW_KEY_A)) {
            camera->TranslateRight(-deltaTime * cameraSpeed);

        }

        if (window->KeyHold(GLFW_KEY_S)) {
            camera->TranslateForward(-deltaTime * cameraSpeed);

        }

        if (window->KeyHold(GLFW_KEY_D)) {
            camera->TranslateRight(deltaTime * cameraSpeed);

        }

        if (window->KeyHold(GLFW_KEY_Q)) {
            camera->TranslateUpward(-deltaTime * cameraSpeed);

        }

        if (window->KeyHold(GLFW_KEY_E)) {
            camera->TranslateUpward(deltaTime * cameraSpeed);

        }


    }
     else {
         if (globalTime < 100 && myTank.life>0)
         {
             ok = true;
             if (window->KeyHold(GLFW_KEY_W)) {
                 myTank.xTankT = myTank.xTankT - (deltaTime * 10) * cos(myTank.rotationAngleT);
                 myTank.zTankT = myTank.zTankT + (deltaTime * 10) * sin(myTank.rotationAngleT);
             }

             if (window->KeyHold(GLFW_KEY_D)) {
                 myTank.rotationAngleT -= deltaTime;
                 myTank.rotationAngleTureta -= deltaTime;
             }

             if (window->KeyHold(GLFW_KEY_S)) {
                 myTank.xTankT = myTank.xTankT + (deltaTime * 10) * cos(myTank.rotationAngleT);
                 myTank.zTankT = myTank.zTankT - (deltaTime * 10) * sin(myTank.rotationAngleT);
             }

             if (window->KeyHold(GLFW_KEY_A)) {
                 myTank.rotationAngleT += deltaTime;
                 myTank.rotationAngleTureta += deltaTime;
             }
         }
        }
}


void Tema2::OnKeyPress(int key, int mods)
{

    if (key == GLFW_KEY_L)
    {
        logan = !logan;
    }

}


void Tema2::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
    float sensivityOX = 0.001f;

    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        float sensivityOY = 0.001f;

        if (window->GetSpecialKeyState() == 0) {
            renderCameraTarget = false;
            //camera->RotateFirstPerson_OX(sensivityOX * -deltaY);
            camera->RotateFirstPerson_OY(sensivityOY * -deltaX);
        }
    }
        myTank.rotationAngleTureta -= (0.01f * deltaX);
}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema2::OnWindowResize(int width, int height)
{
}
