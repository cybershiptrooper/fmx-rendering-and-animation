#include "bike.hpp"

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); 

    GLFWwindow* window = glfwCreateWindow(SCR_W,SCR_H,"ASS2",NULL,NULL);
    if(!window)
    {
        std::cout << "ERROR::GLFW_WINDOW_FAIL" << std::endl;
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, cg::framebuffer_size_callback);
    glfwSetErrorCallback(cg::error_callback);
    glfwSetKeyCallback(window, cg::key_callback);
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if(err != GLEW_OK)
    {
        //Problem: glewInit failed, something is seriously wrong.
        std::cerr << "GLEW Init Failed : %s" << std::endl;
    }

    //Print and see what context got enabled
    std::cout << "Vendor: "   << glGetString(GL_VENDOR)   << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "Version: "  << glGetString(GL_VERSION)  << std::endl;
    std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    //Initialize GL state
    menu = 'B';
    cg::initGL();
    initBuffersGL();

    while(!glfwWindowShouldClose(window))
    {
        renderGL();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    shaderProgram.deleteShader();
    glfwTerminate();
    return 0;
}

void renderGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    matrixStack.clear();

    glm::mat4 view = glm::mat4(1.0f);
    // float time = glfwGetTime()/4;
    // float radi = 6.5;
    // view = glm::lookAt(glm::vec3(sin(-time)*radi, 0.0f, cos(time)*radi),glm::vec3(0.0f,0.0f,0.0f),glm::vec3(0.0f,1.0f,0.0f));
    // view = glm::lookAt(glm::vec3(0.0f, 0.0f, radi),glm::vec3(0.0f,0.0f,0.0f),glm::vec3(0.0f,1.0f,0.0f));
    // cameraPos = glm::rotate(cameraPos, time, glm::vec3(0, 1, 0));
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

    glm::mat4 projection = glm::mat4(1.0f);
    if(perspectiveSwitch)
        projection = glm::perspective(glm::radians(45.0f),4.0f/3.0f,0.1f,100.0f);
    else
        projection = glm::ortho(-3.0f, 3.0f, -3.0f, 3.0f, 0.1f, 10.0f);

    ViewProjection = projection * view;
    matrixStack.push_back(ViewProjection);
    bike[0]->renderTree();
}

void initBuffersGL()
{
    shaderProgram.addShaders("./shaders/vshader.glsl","./shaders/fshader.glsl");
    shaderProgram.useShader();
    ///////////////////////////////////////////////////////////////////////////
    /////////////// DON'T DEFINE SIMILAR MESHES- REUSE THEM ///////////////////
    ///////////////////////////////////////////////////////////////////////////
    generatePrismVertices(enginePoints, enginecol, engine);
    generatePrismVertices(frustumPoints, body, frustum);
    generatePrismVertices(exhaust1Points, white, exhaust1);
    generatePrismVertices(exhaust2Points, white, exhaust2);
    generatePrismVertices(C2Points, white, C2);
    generateCuboidVertices(wheelconnectPoints, body, wheelConnect); //CuW
    generatePrismVertices(wheelcylinderPoints, body, wheelcylinder);
    generateDiscVertices(spokePoints, body, spokes);
    generatePipeVertices(tyrePoints, handle_color, tyre); 
    generatePrismVertices(P1Points, body, P1);
    generateCuboidVertices(cubePoints, body, cube);
    generateCuboidVertices(seat1Points, seat, seat1);
    generatePrismVertices(seatcurvePoints, seat, seatcurve);
    generateCuboidVertices(seat2Points, seat, seat2);
    generatePrismVertices(CH2Points, handle_color, CH2);
    generatePrismVertices(CH1Points, handle_color, CH1);
    generateEllipsoidVertices(dialPoints, seat, dial);
    generatePrismVertices(exhaust3Points, black, exhaust3);
    //generateCuboidVertices(decorPoints, seat, decor);
    generatePrismVertices(decorPoints, yellow, decor);
    //engine
    bike[0] = new cg::Node(NULL,NUM_NODES(NUM_CYLINDER)); 
    bike[9] = new cg::Node(bike[0], NUM_NODES(NUM_PRISM(4))); //F1
    //exhaust
    bike[1] = new cg::Node(bike[0], NUM_NODES(NUM_CYLINDER)); //C4
    bike[2] = new cg::Node(bike[1], NUM_NODES(NUM_CYLINDER)); //C3
    bike[24]= new cg::Node(bike[2], NUM_NODES(NUM_CYLINDER)); //tail
    //back wheel
    bike[3] = new cg::Node(bike[0], NUM_NODES(NUM_CYLINDER));   //C2
    bike[4] = new cg::Node(bike[3], NUM_NODES(NUM_CUBOID));     //CuW1
    bike[5] = new cg::Node(bike[3], NUM_NODES(NUM_CUBOID));     //CuW2
    bike[6] = new cg::Node(bike[4], NUM_NODES(NUM_CYLINDER));   //how to add 2 parents?!
    bike[7] = new cg::Node(bike[6], NUM_NODES(NUM_DISC));       //D1
    bike[8] = new cg::Node(bike[6], NUM_NODES(NUM_PIPE));       //T1
    //body
    bike[10] = new cg::Node(bike[9], NUM_NODES(NUM_PRISM(4))); //P1
    bike[11] = new cg::Node(bike[10],NUM_NODES(NUM_CUBOID));   //Cube
    bike[25] = new cg::Node(bike[11],NUM_NODES(NUM_PRISM(3)));   //decor
    //seat
    bike[12] = new cg::Node(bike[10],NUM_NODES(NUM_CUBOID));   //Seat1
    bike[13] = new cg::Node(bike[12], NUM_NODES(NUM_PRISM(4)));//P3- seatcurve
    bike[14] = new cg::Node(bike[13],NUM_NODES(NUM_CUBOID));   //Seat2
    //handle
    bike[18] = new cg::Node(bike[12], NUM_NODES(NUM_ELLIPSOID));//BIKE DIAL
    bike[15] = new cg::Node(bike[18], NUM_NODES(NUM_CYLINDER)); //CH2
    bike[16] = new cg::Node(bike[18], NUM_NODES(NUM_CYLINDER)); //CH1 left
    bike[17] = new cg::Node(bike[18], NUM_NODES(NUM_CYLINDER)); //CH1 right
    
    //front wheel 
    bike[19] = new cg::Node(bike[15], NUM_NODES(NUM_CUBOID));  //CuW3
    bike[20] = new cg::Node(bike[15], NUM_NODES(NUM_CUBOID));  //CuW4
    bike[21] = new cg::Node(bike[20], NUM_NODES(NUM_CYLINDER));//howto add two parents?!
    bike[22] = new cg::Node(bike[21], NUM_NODES(NUM_DISC));    //D1
    bike[23] = new cg::Node(bike[21], NUM_NODES(NUM_PIPE));    //T1 
    //engine
    bike[0]->addObj(engine, sizeof(engine));
    bike[0]->updateModelParams(0,-0.1,0.,90,0,0);
    bike[9]->addObj(frustum, sizeof(frustum));
    bike[9]->updateModelParams(-0.35,0,0, 0,0,90);
    //front wheel
    bike[19]->addObj(wheelConnect, sizeof(wheelConnect));
    bike[19]->updateModelParams(0.0, 0.98, -0.1, 0,0,0);
    bike[20]->addObj(wheelConnect, sizeof(wheelConnect));
    bike[20]->updateModelParams(0.0, 0.98, 0.1, 0,0,0);
    bike[21]->addObj(wheelcylinder, sizeof(wheelcylinder));
    bike[21]->updateModelParams(0.0, 0.2, -0.1, 90,0,0);
    bike[22]->addObj(spokes, sizeof(spokes));
    bike[22]->updateModelParams(0,0,0,0,0,0); //change ratio
    bike[23]->addObj(tyre, sizeof(tyre));
    bike[23]->updateModelParams(0,0,0,0,0,0);
    //handle
    bike[18]->addObj(dial, sizeof(dial));
    bike[18]->updateModelParams(0.2,0.37,0, 0,0,80);
    bike[18]->updateModelConstraints(60,-60,0,0,0,0);

    bike[15]->addObj(CH2, sizeof(CH2));
    bike[15]->updateModelParams(0,0.8,0, 0,0,0);
    bike[16]->addObj(CH1, sizeof(CH1));
    bike[16]->updateModelParams(0,.05,0.3, 90,0,0);
    bike[17]->addObj(CH1, sizeof(CH1));
    bike[17]->updateModelParams(0,.05,-0.3, 90,0,180);
    
    //body
    bike[10]->addObj(P1, sizeof(P1));
    bike[10]->updateModelParams(0.,0.2,-0.2, 0,45,0);
    bike[11]->addObj(cube, sizeof(cube));
    bike[11]->updateModelParams(0.0,0.5,0, 0,45,0);
    bike[25]->addObj(decor, sizeof(decor));
    bike[25]->updateModelParams(0.35,0.0, 0, 0,0,20);
    //seat
    bike[12]->addObj(seat1,sizeof(seat1));
    bike[12]->updateModelParams(0.2,-0.,-0.2, 0,45,-20);
    bike[13]->addObj(seatcurve,sizeof(seatcurve));
    bike[13]->updateModelParams(-0.02,-0.45,0., 90,25,0);
    bike[14]->addObj(seat2,sizeof(seat2));
    bike[14]->updateModelParams(0.22,0,0.72, 90,0,-15);
    //exhaust
    bike[1]->addObj(exhaust1, sizeof(exhaust1));
    bike[1]->updateModelParams(0.38,0,-0.25,-90,0,100);
    bike[2]->addObj(exhaust2, sizeof(exhaust2));
    bike[2]->updateModelParams(0.0, -0.6, 0.0, 0,0,0);
    bike[24]->addObj(exhaust3, sizeof(exhaust3));
    bike[24]->updateModelParams(0,-0.3,0.0, 0,0,0);
    //back wheel
    bike[3]->addObj(C2, sizeof(C2));
    bike[3]->updateModelParams(0.42, 0, 0.42, -90, 0,-135);
    bike[4]->addObj(wheelConnect, sizeof(wheelConnect));
    bike[4]->updateModelParams(0.0, 0.6, -0.1, 0,0,0);
    bike[5]->addObj(wheelConnect, sizeof(wheelConnect));
    bike[5]->updateModelParams(0.0, 0.6, 0.1, 0,0,0);
    bike[6]->addObj(wheelcylinder, sizeof(wheelcylinder));
    bike[6]->updateModelParams(0.0, 0.2, 0.1, 90,0,0);
    bike[7]->addObj(spokes, sizeof(spokes));
    bike[7]->updateModelParams(0,0,0,0,0,0); //change ratio
    bike[8]->addObj(tyre, sizeof(tyre));
    bike[8]->updateModelParams(0,0,0,0,0,0);
}
