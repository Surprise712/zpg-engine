#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#undef GLAD_GL_IMPLEMENTATION

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Application.h"
#include "Scene.h"
#include "DrawableObject.h"
#include "Model.h"
#include "ShaderProgram.h"
#include "Transformation.h"

#include "Models/tree.h"
#include "Models/bushes.h"
#include "Models/sphere.h"

#include "Models/BEN0339.h"
#include "Models/earth.h"
#include "Models/sun.h"
#include "Models/moon.h"
#include "Models/clock.h"
#include "Models/steelAxe.h"


#include <cstdlib>
#include <ctime>
#include <iostream>
#include <memory>
#include <vector>

#include <glm/glm.hpp>

Application::Application(): window(nullptr),currentScene(0),lastMouseX(0.0),lastMouseY(0.0),firstMouse(true){

}
Application::~Application()
{


    cleanup();
}

bool Application::init( int width, int height,const char* title)

    
{
    if (!glfwInit())
    {
        std::cout << "GLFW initialization failed." << std::endl;
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // tohle zapina MSAA (antialiasing) - vyhladi hranate okraje kulatych modelu jako slunce
    glfwWindowHint(GLFW_SAMPLES, 4);
    
    window = glfwCreateWindow(
        width,
        height,
        title,
        nullptr,
        nullptr);

    if (window == nullptr)
    {
        std::cout << "Window creation failed." << std::endl;
        glfwTerminate();
        return false;
    }

    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress))
    {
        std::cout << "GLAD initialization failed." << std::endl;
        glfwDestroyWindow(window);
        window = nullptr;
        glfwTerminate();
        return false;
    }

    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(
        window,
        framebufferSizeCallback);

    int framebufferWidth = 0;
    int framebufferHeight = 0;
    glfwGetFramebufferSize(
        window,
        &framebufferWidth,
        &framebufferHeight);
    glViewport(0, 0, framebufferWidth, framebufferHeight);

    glEnable(GL_DEPTH_TEST);
    // tohle zapina multisampling
    glEnable(GL_MULTISAMPLE);
    /*glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);*/

    std::cout << "OpenGL Version: "
              << glGetString(GL_VERSION) << std::endl;

    std::cout << "Vendor: "
              << glGetString(GL_VENDOR) << std::endl;

    std::cout << "Renderer: "<< glGetString(GL_RENDERER) << std::endl;

    std::cout << "GLSL: "
              << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    return true;
}

void Application::createScenes()
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    std::unique_ptr<Scene> scene = std::make_unique<Scene>();

    ShaderProgram* shaderProgram = scene->addShaderProgram(
        std::make_unique<ShaderProgram>(
            "../shaders/basic.vert",
            "../shaders/basic.frag"));

    ShaderProgram* shaderProgram2 = scene->addShaderProgram(
        std::make_unique<ShaderProgram>(
            "../shaders/basic.vert",
            "../shaders/yellow.frag"));

    
    static const float triangle[] = {

        -0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,

    };

    Model* model = scene->addModel(std::make_unique<Model>(triangle, sizeof(triangle)));

	auto transformation = std::make_unique<CompositeTransformation>();

    //transformation->addTransformation(std::make_unique<Rotation>(glm::radians(-45.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
	transformation->addTransformation(std::make_unique<Translation>(glm::vec3(0.4f, 0.0f, 0.0f)));
	transformation->addTransformation(std::make_unique<Rotation>(glm::radians(-45.0f), glm::vec3(0.0f, 0.0f, 1.0f)));

    scene->addObject(std::make_unique<DrawableObject>( model, shaderProgram,std::move(transformation)));

    scenes.push_back(std::move(scene));

    scenes[0]->moveCamera(-3.0f, 0.0f);

    //==Forrest Scene==

    auto forestScene = std::make_unique<Scene>();

    Model::resetVertexFormat();

    ShaderProgram* forestShaderProgram = forestScene->addShaderProgram(std::make_unique<ShaderProgram>("../shaders/tree.vert","../shaders/tree.frag"));

    ShaderProgram* bushShaderProgram = forestScene->addShaderProgram(std::make_unique<ShaderProgram>("../shaders/bush.vert","../shaders/bush.frag"));

    ShaderProgram* sunShaderProgram = forestScene->addShaderProgram(std::make_unique<ShaderProgram>("../shaders/basic.vert", "../shaders/yellow.frag"));

    ShaderProgram* groundShaderProgram = forestScene->addShaderProgram( std::make_unique<ShaderProgram>( "../shaders/basic.vert","../shaders/basic.frag"));


    Model* modelTree = forestScene->addModel(std::make_unique<Model>(tree, sizeof(tree)));
    Model* modelBushes = forestScene->addModel(std::make_unique<Model>(bushes, sizeof(bushes)));
    Model* modelsun = forestScene->addModel(std::make_unique<Model>(sphere, sizeof(sphere)));
    Model* modelLogin = forestScene->addModel(std::make_unique<Model>(login, sizeof(login)));


    // Ground
    static const float ground[] = {
        -15.0f, -0.51f, -15.0f, 0.2f, 0.4f, 0.1f,
         15.0f, -0.51f,  15.0f, 0.2f, 0.4f, 0.1f,
         15.0f, -0.51f, -15.0f, 0.2f, 0.4f, 0.1f,

        -15.0f, -0.51f, -15.0f, 0.2f, 0.4f, 0.1f,
        -15.0f, -0.51f,  15.0f, 0.2f, 0.4f, 0.1f,
         15.0f, -0.51f,  15.0f, 0.2f, 0.4f, 0.1f
    };

    Model* modelGround = forestScene->addModel(std::make_unique<Model>(ground, sizeof(ground)));

    auto groundTransformation = std::make_unique<CompositeTransformation>();

    forestScene->addObject(std::make_unique<DrawableObject>(
        modelGround, groundShaderProgram, std::move(groundTransformation)));


    // Trees
    std::vector<glm::vec2> treePositions;

    for (int i = 0; i < 96;)
    {
        float x = (std::rand() % 2401 - 1200) / 100.0f;
        float z = (std::rand() % 2401 - 1200) / 100.0f;

        glm::vec2 position(x, z);
        bool tooClose = false;

        for (const auto& other : treePositions)
        {
            if (glm::distance(position, other) < 2.1f)
            {
                tooClose = true;
                break;
            }
        }

        if (tooClose) continue;

        treePositions.push_back(position);

        float angle = static_cast<float>(std::rand() % 360);
        float size = 0.08f + (std::rand() % 71) / 1000.0f;

        auto transformation = std::make_unique<CompositeTransformation>();

        transformation->addTransformation(std::make_unique<Translation>(glm::vec3(x, -0.5f, z)));
        transformation->addTransformation(std::make_unique<Rotation>(glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f)));
        transformation->addTransformation(std::make_unique<Scale>(glm::vec3(size)));

        forestScene->addObject(std::make_unique<DrawableObject>(
            modelTree, forestShaderProgram, std::move(transformation)));

        i++;
    }


    // Bushes
    std::vector<glm::vec2> bushPositions;

    for (int i = 0; i < 380;)
    {
        float x = (std::rand() % 2401 - 1200) / 100.0f;
        float z = (std::rand() % 2401 - 1200) / 100.0f;

        glm::vec2 position(x, z);
        bool tooClose = false;

        for (const auto& tree : treePositions)
        {
            if (glm::distance(position, tree) < 0.9f)
            {
                tooClose = true;
                break;
            }
        }

        if (tooClose) continue;

        for (const auto& bush : bushPositions)
        {
            if (glm::distance(position, bush) < 0.7f)
            {
                tooClose = true;
                break;
            }
        }

        if (tooClose) continue;

        bushPositions.push_back(position);

        float angle = static_cast<float>(std::rand() % 360);

        auto transformation = std::make_unique<CompositeTransformation>();

        transformation->addTransformation(std::make_unique<Translation>(glm::vec3(x, -0.5f, z)));
        transformation->addTransformation(std::make_unique<Rotation>(glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f)));
        transformation->addTransformation(std::make_unique<Scale>(glm::vec3(0.4f)));

        forestScene->addObject(std::make_unique<DrawableObject>(
            modelBushes, bushShaderProgram, std::move(transformation)));

        i++;
    }


    // Sun
    auto sunTransformation = std::make_unique<CompositeTransformation>();

    sunTransformation->addTransformation(std::make_unique<Translation>(glm::vec3(0.0f, 5.0f, -8.0f)));
    sunTransformation->addTransformation(std::make_unique<Scale>(glm::vec3(0.8f)));

    forestScene->addObject(std::make_unique<DrawableObject>(
        modelsun, sunShaderProgram, std::move(sunTransformation)));


    // Login
    auto loginTransformation = std::make_unique<CompositeTransformation>();
    loginTransformation->addTransformation(std::make_unique<Translation>(glm::vec3(0.0f, 4.0f, -8.0f)));
    loginTransformation->addTransformation(std::make_unique<Scale>(glm::vec3(0.9f)));

    forestScene->addObject(std::make_unique<DrawableObject>(
        modelLogin, groundShaderProgram, std::move(loginTransformation)));


    // Clock in forest
    ShaderProgram* forestClockShader = forestScene->addShaderProgram(std::make_unique<ShaderProgram>( "../shaders/clock.vert", "../shaders/clock.frag"));

    Model::setVertexFormat(9, 6);

    Model* forestClockModel = forestScene->addModel(std::make_unique<Model>(clockModel, sizeof(clockModel)));

    Model::resetVertexFormat();


    // First clock
    auto forestClockTransformation = std::make_unique<CompositeTransformation>();

    forestClockTransformation->addTransformation(std::make_unique<Translation>(glm::vec3(0.0f, 0.0f, -5.0f)));
    forestClockTransformation->addTransformation(std::make_unique<Scale>(glm::vec3(-1.0f, 1.0f, 1.0f)));
    forestClockTransformation->addTransformation(std::make_unique<Rotation>(glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
    forestClockTransformation->addTransformation(std::make_unique<Scale>(glm::vec3(0.2f)));

    forestScene->addObject(std::make_unique<DrawableObject>(forestClockModel, forestClockShader, std::move(forestClockTransformation)));


    // Second clock
    auto forestClockTransformation2 = std::make_unique<CompositeTransformation>();

    forestClockTransformation2->addTransformation(std::make_unique<Translation>(glm::vec3(5.0f, 0.5f, 0.0f)));
    forestClockTransformation2->addTransformation(std::make_unique<Scale>(glm::vec3(-1.0f, 1.0f, 1.0f)));
    forestClockTransformation2->addTransformation(std::make_unique<Scale>(glm::vec3(0.2f)));

    forestScene->addObject(std::make_unique<DrawableObject>(forestClockModel, forestClockShader, std::move(forestClockTransformation2)));

	scenes.push_back(std::move(forestScene));

    //==Solar systam==

    auto galaxyScene = std::make_unique<Scene>();
    Model::setVertexFormat(9, 3); //set format

    //Sun
    ShaderProgram* galaxySunShaderProgram = galaxyScene->addShaderProgram(std::make_unique<ShaderProgram>(
        "../shaders/planets.vert", 
        "../shaders/sun.frag"
    ));

    Model* modelOrb = galaxyScene->addModel(std::make_unique<Model>(sun, sizeof(sun)));

    transformation = std::make_unique<CompositeTransformation>();

    transformation->addTransformation(std::make_unique<DynRotation>(0.5f, glm::vec3(0.2f, 0.0f, 0.0f)));
    transformation->addTransformation(std::make_unique<Scale>(glm::vec3(0.18f)));

    galaxyScene->addObject(std::make_unique<DrawableObject>(modelOrb, galaxySunShaderProgram, std::move(transformation)));

    //Earth
    ShaderProgram* earthShaderProgram = galaxyScene->addShaderProgram(std::make_unique<ShaderProgram>(
        "../shaders/planets.vert",
        "../shaders/earth.frag"
    ));

    Model* modelEarth = galaxyScene->addModel(std::make_unique<Model>(earth, sizeof(earth)));

    transformation = std::make_unique<CompositeTransformation>();

    transformation->addTransformation(std::make_unique<DynRotation>(0.5f, glm::vec3(0.2f, 0.0f, 0.0f)));
    transformation->addTransformation(std::make_unique<Translation>(glm::vec3(0.0f, 0.5f, 0.0f)));
    transformation->addTransformation(std::make_unique<Scale>(glm::vec3(0.1f)));

    galaxyScene->addObject(std::make_unique<DrawableObject>(modelEarth, earthShaderProgram, std::move(transformation)));

    //Moon
    ShaderProgram* moonShaderProgram = galaxyScene->addShaderProgram(std::make_unique<ShaderProgram>(
        "../shaders/planets.vert",
        "../shaders/moon.frag"
    ));

    Model* modelMoon = galaxyScene->addModel(std::make_unique<Model>(moon, sizeof(moon)));

    transformation = std::make_unique<CompositeTransformation>();

    transformation->addTransformation(std::make_unique<DynRotation>(0.5f, glm::vec3(0.2f, 0.0f, 0.0f)));    //earth rotation
    transformation->addTransformation(std::make_unique<Translation>(glm::vec3(0.0f, 0.5f, 0.0f)));          //earth position
    transformation->addTransformation(std::make_unique<DynRotation>(0.5f, glm::vec3(0.0f, 0.0f, 0.4f)));
    transformation->addTransformation(std::make_unique<Translation>(glm::vec3(0.2f, 0.0f, 0.0f)));
    transformation->addTransformation(std::make_unique<Scale>(glm::vec3(0.05)));

    galaxyScene->addObject(std::make_unique<DrawableObject>(modelMoon, moonShaderProgram, std::move(transformation)));


    //Axe

    Model* modelAxe = galaxyScene->addModel(std::make_unique<Model>(axe, sizeof(axe)));

    transformation = std::make_unique<CompositeTransformation>();

    transformation->addTransformation(std::make_unique<DynRotation>(0.8f, glm::vec3(0.0f, 0.2f, 0.0f)));
    transformation->addTransformation(std::make_unique<Translation>(glm::vec3(0.9f, 0.0f, 0.0f)));
    transformation->addTransformation(std::make_unique<DynRotation>(0.5f, glm::vec3(0.0f, 0.0f, 0.5f)));
    transformation->addTransformation(std::make_unique<Scale>(glm::vec3(0.1)));
    transformation->addTransformation(std::make_unique<Rotation>(90.0f, glm::vec3(1.0f, 0.0f, 0.0f)));

    galaxyScene->addObject(std::make_unique<DrawableObject>(modelAxe, earthShaderProgram, std::move(transformation)));

    Model::resetVertexFormat();

    // login
    Model* modelgLogin = galaxyScene->addModel(std::make_unique<Model>(login, sizeof(login)));

    auto galaxyLoginTransformation = std::make_unique<CompositeTransformation>();
    galaxyLoginTransformation->addTransformation(std::make_unique<Translation>(glm::vec3(0.0f, 4.0f, -8.0f)));
    galaxyLoginTransformation->addTransformation(std::make_unique<Scale>(glm::vec3(0.9f)));

    galaxyScene->addObject(std::make_unique<DrawableObject>(modelgLogin, galaxySunShaderProgram, std::move(galaxyLoginTransformation)));


    scenes.push_back(std::move(galaxyScene));

    scenes[2]->moveCamera(-3.0f, 0.0f);

    // == Clock Scene ==

    auto clockScene = std::make_unique<Scene>();

    ShaderProgram* clockShaderProgram = clockScene->addShaderProgram(
        std::make_unique<ShaderProgram>(
            "../shaders/clock.vert",
            "../shaders/clock.frag"
        )
    );

    Model::setVertexFormat(9, 6);

    Model* modelClock = clockScene->addModel(std::make_unique<Model>(clockModel,sizeof(clockModel)) );

    auto clockTransformation =std::make_unique<CompositeTransformation>();



    clockTransformation->addTransformation( std::make_unique<Scale>( glm::vec3(-1.0f, 1.0f, 1.0f)) );

    clockTransformation->addTransformation(std::make_unique<Rotation>( glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f)));

    clockScene->addObject(std::make_unique<DrawableObject>(modelClock, clockShaderProgram, std::move(clockTransformation) )
);

    scenes.push_back(std::move(clockScene));

    scenes[3]->moveCamera(-3.0f, 0.0f);

    // == Spheres Light Scene ==

    auto spheresScene = std::make_unique<Scene>();

    //for different shaders change to constant.frag, lambert.frag, phong.frag or blinn_phong.frag
    ShaderProgram* sphereShaderProgram1 = spheresScene->addShaderProgram(
        std::make_unique<ShaderProgram>(
            "../shaders/light.vert",
            "../shaders/constant.frag"
        )
    );
    ShaderProgram* sphereShaderProgram2 = spheresScene->addShaderProgram(
        std::make_unique<ShaderProgram>(
            "../shaders/light.vert",
            "../shaders/lambert.frag"
        )
    );
    ShaderProgram* sphereShaderProgram3 = spheresScene->addShaderProgram(
        std::make_unique<ShaderProgram>(
            "../shaders/light.vert",
            "../shaders/phong.frag"
        )
    );
    ShaderProgram* sphereShaderProgram4 = spheresScene->addShaderProgram(
        std::make_unique<ShaderProgram>(
            "../shaders/light.vert",
            "../shaders/blinn_phong.frag"
        )
    );

    Model::setVertexFormat(6, 3);

    Model* modelSphere = spheresScene->addModel(std::make_unique<Model>(sphere, sizeof(sphere)));

    //Sphere 1
    auto sphereTransformation = std::make_unique<CompositeTransformation>();

    sphereTransformation->addTransformation(std::make_unique<Translation>(glm::vec3(3.0f, 0.0f, 0.0f)));

    spheresScene->addObject(std::make_unique<DrawableObject>(modelSphere, sphereShaderProgram1, std::move(sphereTransformation)));
    
    //Sphere 2
    auto sphereTransformation2 = std::make_unique<CompositeTransformation>();

    sphereTransformation2->addTransformation(std::make_unique<Translation>(glm::vec3(-3.0f, 0.0f, 0.0f)));

    spheresScene->addObject(std::make_unique<DrawableObject>(modelSphere, sphereShaderProgram2, std::move(sphereTransformation2)));

    //Sphere 3
    auto sphereTransformation3 = std::make_unique<CompositeTransformation>();

    sphereTransformation3->addTransformation(std::make_unique<Translation>(glm::vec3(0.0f, 0.0f, 3.0f)));

    spheresScene->addObject(std::make_unique<DrawableObject>(modelSphere, sphereShaderProgram3, std::move(sphereTransformation3)));

    //Sphere 4
    auto sphereTransformation4 = std::make_unique<CompositeTransformation>();

    sphereTransformation4->addTransformation(std::make_unique<Translation>(glm::vec3(0.0f, 0.0f, -3.0f)));

    spheresScene->addObject(std::make_unique<DrawableObject>(modelSphere, sphereShaderProgram4, std::move(sphereTransformation4)));

    scenes.push_back(std::move(spheresScene));

    //Camera movement
    scenes[4]->rotateCamera(90.0f, 0.0f);
    scenes[4]->moveCamera(-8.0f, 0.0f);




    //==Night Forrest Scene==

    auto nforestScene = std::make_unique<Scene>();

    Model::resetVertexFormat();

    // OPRAVENO: Používáme nforestScene místo forestScene
    nightForestShader = nforestScene->addShaderProgram(std::make_unique<ShaderProgram>("../shaders/night_tree.vert", "../shaders/night_tree.frag"));
    nightBushShader = nforestScene->addShaderProgram(std::make_unique<ShaderProgram>("../shaders/night_bush.vert", "../shaders/night_tree.frag"));
    nightGroundShader = nforestScene->addShaderProgram(std::make_unique<ShaderProgram>("../shaders/night_ground.vert", "../shaders/night_tree.frag"));
    fireflyShader = nforestScene->addShaderProgram(std::make_unique<ShaderProgram>("../shaders/firefly.vert", "../shaders/yellow.frag"));
    auto nightLoginShader = nforestScene->addShaderProgram(std::make_unique<ShaderProgram>("../shaders/light.vert", "../shaders/yellow.frag"));


    Model* nmodelTree = nforestScene->addModel(std::make_unique<Model>(tree, sizeof(tree)));
    Model* nmodelBushes = nforestScene->addModel(std::make_unique<Model>(bushes, sizeof(bushes)));
    Model* nmodelsun = nforestScene->addModel(std::make_unique<Model>(sphere, sizeof(sphere)));
    Model* nmodelLogin = nforestScene->addModel(std::make_unique<Model>(login, sizeof(login)));


    // Ground
    static const float nground[] = {
        -15.0f, -0.51f, -15.0f, 0.2f, 0.4f, 0.1f,
         15.0f, -0.51f,  15.0f, 0.2f, 0.4f, 0.1f,
         15.0f, -0.51f, -15.0f, 0.2f, 0.4f, 0.1f,

        -15.0f, -0.51f, -15.0f, 0.2f, 0.4f, 0.1f,
        -15.0f, -0.51f,  15.0f, 0.2f, 0.4f, 0.1f,
         15.0f, -0.51f,  15.0f, 0.2f, 0.4f, 0.1f
    };

    Model* nmodelGround = nforestScene->addModel(std::make_unique<Model>(nground, sizeof(nground)));

    auto ngroundTransformation = std::make_unique<CompositeTransformation>();

    nforestScene->addObject(std::make_unique<DrawableObject>(
        nmodelGround, nightGroundShader, std::move(ngroundTransformation)));


    // Trees
    std::vector<glm::vec2> ntreePositions;

    for (int i = 0; i < 96;)
    {
        float x = (std::rand() % 2401 - 1200) / 100.0f;
        float z = (std::rand() % 2401 - 1200) / 100.0f;

        glm::vec2 position(x, z);
        bool tooClose = false;

        for (const auto& other : ntreePositions)
        {
            if (glm::distance(position, other) < 2.1f)
            {
                tooClose = true;
                break;
            }
        }

        if (tooClose) continue;

        ntreePositions.push_back(position);

        float angle = static_cast<float>(std::rand() % 360);
        float size = 0.2f + (std::rand() % 71) / 1000.0f;

        auto ntransformation = std::make_unique<CompositeTransformation>();

        ntransformation->addTransformation(std::make_unique<Translation>(glm::vec3(x, -0.5f, z)));
        ntransformation->addTransformation(std::make_unique<Rotation>(glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f)));
        ntransformation->addTransformation(std::make_unique<Scale>(glm::vec3(size)));

        nforestScene->addObject(std::make_unique<DrawableObject>(
            nmodelTree, nightForestShader, std::move(ntransformation)));

        i++;
    }


    // Bushes
    std::vector<glm::vec2> nbushPositions;

    for (int i = 0; i < 380;)
    {
        float x = (std::rand() % 2401 - 1200) / 100.0f;
        float z = (std::rand() % 2401 - 1200) / 100.0f;

        glm::vec2 position(x, z);
        bool tooClose = false;

        for (const auto& tree : ntreePositions)
        {
            if (glm::distance(position, tree) < 0.9f)
            {
                tooClose = true;
                break;
            }
        }

        if (tooClose) continue;

        for (const auto& bush : nbushPositions)
        {
            if (glm::distance(position, bush) < 0.7f)
            {
                tooClose = true;
                break;
            }
        }

        if (tooClose) continue;

        nbushPositions.push_back(position);

        float angle = static_cast<float>(std::rand() % 360);

        auto ntransformation = std::make_unique<CompositeTransformation>();

        ntransformation->addTransformation(std::make_unique<Translation>(glm::vec3(x, -0.5f, z)));
        ntransformation->addTransformation(std::make_unique<Rotation>(glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f)));
        ntransformation->addTransformation(std::make_unique<Scale>(glm::vec3(0.5f)));

        nforestScene->addObject(std::make_unique<DrawableObject>(
            nmodelBushes, nightBushShader, std::move(ntransformation)));

        i++;
    }


    // Firefly
    auto nsunTransformation = std::make_unique<CompositeTransformation>();

    nsunTransformation->addTransformation(std::make_unique<Translation>(glm::vec3(0.0f, 0.1f, 0.0f)));
    nsunTransformation->addTransformation(std::make_unique<Scale>(glm::vec3(0.02f)));

    nforestScene->addObject(std::make_unique<DrawableObject>(
        nmodelsun, fireflyShader, std::move(nsunTransformation)));


    // Login
    auto nloginTransformation = std::make_unique<CompositeTransformation>();
    nloginTransformation->addTransformation(std::make_unique<Translation>(glm::vec3(0.0f, 4.0f, -8.0f)));
    nloginTransformation->addTransformation(std::make_unique<Scale>(glm::vec3(0.9f)));

    nforestScene->addObject(std::make_unique<DrawableObject>(
        nmodelLogin, nightLoginShader, std::move(nloginTransformation)));

    scenes.push_back(std::move(nforestScene));







    // === Texture Scene ===
    auto textureScene = std::make_unique<Scene>();

    const float plain[] = {
        //vrchol,           normála,            uv souřadnice
        1.0f, 0.0f, 1.0f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f,
        1.0f, 0.0f,-1.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
       -1.0f, 0.0f,-1.0f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f,

       -1.0f, 0.0f, 1.0f,   0.0f, 1.0f, 0.0f,   0.0f, 1.0f,
        1.0f, 0.0f, 1.0f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f,
       -1.0f, 0.0f,-1.0f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f
    };

    auto textureShader = textureScene->addShaderProgram(std::make_unique<ShaderProgram>("../shaders/texture.vert", "../shaders/texture.frag"));

    //vertex buffer object (VBO)
    GLuint VBO = 0;
    glGenBuffers(1, &VBO); // generate the VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(plain), plain, GL_STATIC_DRAW);
    //vertex attribute object(VAO)
    GLuint VAO = 0;
    glGenVertexArrays(1, &VAO); //generate the VAO
    glBindVertexArray(VAO); //bind the VAO
    glEnableVertexAttribArray(0); //enable vertex attributes
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (GLvoid*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (GLvoid*)(3 * sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (GLvoid*)(6 * sizeof(float)));

    int text_width, text_height, channels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load("Models/wooden_fence.png", &text_width, &text_height, &channels, 4);
    if (!data) {
        std::cerr << "Error loading texture: wooden_fence.png" << std::endl;
    }
    glActiveTexture(GL_TEXTURE0);
    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, text_width, text_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);

    //Set texture unit to fragment shader
    GLint uniformID = glGetUniformLocation(textureShader->getId(), "../shaders/texture.frag");
    glUniform1i(uniformID, 0);

    scenes.push_back(std::move(textureScene));


}



void Application::processInput(float deltaTime)
{





    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);

    for (int key = GLFW_KEY_1; key <= GLFW_KEY_9; ++key)
    {
        if (glfwGetKey(window, key) == GLFW_PRESS)
        {
            std::size_t sceneIndex = static_cast<std::size_t>(key - GLFW_KEY_1);

            if (sceneIndex < scenes.size())
                currentScene = sceneIndex;
        }
    }

    // Mouse
    double mouseX, mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
    {

       // std::cout << "Right mouse pressed" << std::endl;


        if (firstMouse)
        {
            lastMouseX = mouseX;
            lastMouseY = mouseY;
            firstMouse = false;
        }

        double deltaX = mouseX - lastMouseX;
        double deltaY = mouseY - lastMouseY;

        lastMouseX = mouseX;
        lastMouseY = mouseY;

        float sensitivity = glm::radians(0.1f);

        if (currentScene < scenes.size())
            scenes[currentScene]->rotateCamera(
                static_cast<float>(deltaY) * sensitivity,
                static_cast<float>(deltaX) * sensitivity);
    }
    else
    {
        firstMouse = true;
    }

    // WASD
    glm::vec2 movement(0.0f);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) movement.x += 1.0f;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) movement.x -= 1.0f;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) movement.y -= 1.0f;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) movement.y += 1.0f;

    if (glm::length(movement) > 0.0f && currentScene < scenes.size())
    {
        movement = glm::normalize(movement) * 3.0f * deltaTime;
        scenes[currentScene]->moveCamera(movement.x, movement.y);
    }
}


void Application::beginFrame()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Application::drawCurrentScene()
{
    // Původní kód s lokálním časem pro hodiny necháš jak je...
    std::time_t now = std::time(nullptr);
    std::tm localTime{};
    localtime_s(&localTime, &now);
    float time = localTime.tm_hour * 3600.0f + localTime.tm_min * 60.0f + localTime.tm_sec;

    // === Výpočet pozice světlušky ===
    animationTime = static_cast<float>(glfwGetTime()); // Plynulý čas pro animaci
    float radius = 3.0f;
    float speed = 0.2f;

    glm::vec3 fireflyPos(
        cos(animationTime * speed) * radius,
        0.5f + sin(animationTime * 0.5f) * 0.5f, // Lehké pohupování
        sin(animationTime * speed) * radius
    );

    // === Odeslání do všech shaderů ===
    if (currentScene == 5) // Změň na správný index noční scény
    {
        // 1. Předáme pozici stromům
        nightForestShader->use();
        nightForestShader->setUniform("lightPosition", fireflyPos);

        // 2. Předáme pozici keřům
        nightBushShader->use();
        nightBushShader->setUniform("lightPosition", fireflyPos);

        // 3. Předáme pozici zemi
        nightGroundShader->use();
        nightGroundShader->setUniform("lightPosition", fireflyPos);

        // 4. Předáme pozici samotnému modelu světlušky
        fireflyShader->use();
        fireflyShader->setUniform("lightPosition", fireflyPos);
    }

    // Vykreslení scény
    scenes[currentScene]->draw(time);
}

void Application::endFrame()
{
    glfwSwapBuffers(window);
    glfwPollEvents();
}

void Application::cleanup()
{
    // OpenGL resources must be destroyed while a valid context still exists.
    scenes.clear();

    if (window != nullptr)
    {
        glfwDestroyWindow(window);
        window = nullptr;
    }

    glfwTerminate();
}

void Application::framebufferSizeCallback(
    GLFWwindow* window,
    int width,
    int height)
{
    glViewport(0, 0, width, height);

    if (width <= 0 || height <= 0)
    {
        return;
    }

    // GLFW needs a static callback, so get the owning Application from the window.
    Application* application = static_cast<Application*>(glfwGetWindowUserPointer(window));
    if (application != nullptr)
    {
        const float aspectRatio = static_cast<float>(width) / static_cast<float>(height);
        for (const std::unique_ptr<Scene>& scene : application->scenes)
        {
            scene->setAspectRatio(aspectRatio);
        }
    }
}

int Application::run()
{
    //  okno z 800x600 na 1280x720
    if (!init(1280, 960, "ZPG"))
    {
        return -1;
    }

    createScenes();

    // Initialise projection for the actual framebuffer size (also on HiDPI displays).
    int framebufferWidth = 0;
    int framebufferHeight = 0;
    glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
    framebufferSizeCallback(window, framebufferWidth, framebufferHeight);


    double lastFrame = glfwGetTime();

    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        double currentFrame = glfwGetTime();
        float deltaTime = static_cast<float>(currentFrame - lastFrame);
        lastFrame = currentFrame;

        processInput(deltaTime);

        beginFrame();
        drawCurrentScene();
        endFrame();
    }


    cleanup();
    return 0;
}
