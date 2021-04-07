#include "rider.hpp"
#include <iostream>
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
    menu = 'R';
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
    // float radi = 5;
    // view = glm::lookAt(glm::vec3(sin(time)*radi, 0.0f, cos(time)*radi),glm::vec3(0.0f,0.0f,0.0f),glm::vec3(0.0f,1.0f,0.0f));
    // view = glm::lookAt(glm::vec3(0.0f, 0.0f, radi),glm::vec3(0.0f,0.0f,0.0f),glm::vec3(0.0f,1.0f,0.0f));
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

    glm::mat4 projection = glm::mat4(1.0f);
    if(perspectiveSwitch)
        projection = glm::perspective(glm::radians(45.0f),4.0f/3.0f,0.1f,100.0f);
    else
        projection = glm::ortho(-3.0f, 3.0f, -3.0f, 3.0f, 0.1f, 10.0f);

    ViewProjection = projection * view;
    matrixStack.push_back(ViewProjection);
    rider[0]->renderTree();
}

void initBuffersGL()
{
    shaderProgram.addShaders("./shaders/vshader.glsl","./shaders/fshader.glsl");
    shaderProgram.useShader();

    generatePrismVertices(hipPoints,pants,hip);
    generateEllipsoidVertices(bendjointPoints, pants, bendjoint);
    generatePrismVertices(f2Points,shirt,f2);
    generatePrismVertices(f1Points,shirt,f1);
    generateEllipsoidVertices(mainbuttPoints, pants, main_butt);
    generateEllipsoidVertices(buttPoints, pants, butt);
    generatePrismVertices(thighPoints,pants,thigh);
    generateEllipsoidVertices(S3Points,shirt,S3);
    generatePrismVertices(shinPoints, pants, shin);
    generateEllipsoidVertices(C1Points, skin, C1);
    generateEllipsoidVertices(E1Points, skin, E1);
    generateEllipsoidVertices(S1Points, pants, S1);
    generateEllipsoidVertices(S2Points, pants, S2);
    generatePrismVertices(A1Points, shirt, A1);
    generatePrismVertices(A2Points, shirt, A2);
    generateEllipsoidVertices(handPoints, skin, hand);
    generateEllipsoidVertices(anklePoints, pants, ankle);
    generatePrismVertices(feetPoints, pants, feet);
    generatePipeVertices(pipePoints, hat, pipe);
    generatePrismVertices(hPoints, hat, h);
    generateCuboidVertices(nosePoints, pants, nose);
    generateCuboidVertices(eyePoints, pants, eyes);
    //root
    rider[0] = new cg::Node(NULL,NUM_NODES(NUM_CYLINDER)); //hip
    //lower body 
    rider[3] = new cg::Node(rider[0],NUM_NODES(NUM_ELLIPSOID));//main butt
    //lower left
    rider[20]= new cg::Node(rider[0],NUM_NODES(NUM_ELLIPSOID)); //left butt
    rider[4] = new cg::Node(rider[20],NUM_NODES(NUM_CYLINDER));  //L1L
    rider[6] = new cg::Node(rider[4],NUM_NODES(NUM_ELLIPSOID)); //S3L
    rider[8] = new cg::Node(rider[6],NUM_NODES(NUM_CYLINDER));  //L2L`
    rider[24]= new cg::Node(rider[8],NUM_NODES(NUM_ELLIPSOID)); //left ankle
    rider[26]= new cg::Node(rider[24],NUM_NODES(NUM_PRISM(4))); //left feet
    //lower right
    rider[21]= new cg::Node(rider[0],NUM_NODES(NUM_ELLIPSOID)); //right butt
    rider[5] = new cg::Node(rider[21],NUM_NODES(NUM_CYLINDER));  //L1R
    rider[7] = new cg::Node(rider[5],NUM_NODES(NUM_ELLIPSOID)); //S3R
    rider[9] = new cg::Node(rider[7],NUM_NODES(NUM_CYLINDER));  //L2L
    rider[25]= new cg::Node(rider[9],NUM_NODES(NUM_ELLIPSOID)); //right ankle
    rider[27]= new cg::Node(rider[25],NUM_NODES(NUM_PRISM(4))); //right feet
    //upper body
    rider[28]= new cg::Node(rider[0], NUM_NODES(NUM_ELLIPSOID));//bending joint
    rider[1] = new cg::Node(rider[28],NUM_NODES(NUM_CYLINDER)); //F2
    rider[2] = new cg::Node(rider[1],NUM_NODES(NUM_CYLINDER));  //F1  
    rider[10]= new cg::Node(rider[2],NUM_NODES(NUM_ELLIPSOID)); //C1
    rider[11]= new cg::Node(rider[10],NUM_NODES(NUM_ELLIPSOID));//E1
    rider[31]= new cg::Node(rider[11],NUM_NODES(NUM_CUBOID));   //NOSE
    rider[32]= new cg::Node(rider[11],NUM_NODES(NUM_CUBOID));
    rider[33]= new cg::Node(rider[11],NUM_NODES(NUM_CUBOID));
    //hat
    rider[29]= new cg::Node(rider[11], NUM_NODES(NUM_PIPE/10));
    rider[30]= new cg::Node(rider[29], NUM_NODES(NUM_PRISM(4)));
    //upper left
    rider[12]= new cg::Node(rider[1],NUM_NODES(NUM_ELLIPSOID)); //S1L
    rider[14]= new cg::Node(rider[12],NUM_NODES(NUM_CYLINDER)); //A1L
    rider[16]= new cg::Node(rider[14],NUM_NODES(NUM_ELLIPSOID));//S2L
    rider[18]= new cg::Node(rider[16],NUM_NODES(NUM_CYLINDER)); //A2L
    rider[22]= new cg::Node(rider[18],NUM_NODES(NUM_ELLIPSOID)); //HL
    //upper right
    rider[13]= new cg::Node(rider[1],NUM_NODES(NUM_ELLIPSOID)); //S1R
    rider[15]= new cg::Node(rider[13],NUM_NODES(NUM_CYLINDER)); //A1L
    rider[17]= new cg::Node(rider[15],NUM_NODES(NUM_ELLIPSOID));//S2R
    rider[19]= new cg::Node(rider[17],NUM_NODES(NUM_CYLINDER)); //A2R
    rider[23]= new cg::Node(rider[19],NUM_NODES(NUM_ELLIPSOID));//HR
    //root
    rider[0]->addObj(hip,sizeof(hip));
    rider[0]->updateModelParams(0,-0.8,0,0,0,0);
    //upper body
    rider[28]->addObj(bendjoint, sizeof(bendjoint));
    rider[28]->updateModelParams(0,0.1,0,0,0,0);
    rider[28]->updateModelConstraints(0,0,10, -10, 30, -30);
    rider[1]->addObj(f2,sizeof(f2));
    rider[1]->updateModelParams(0,0.4,0,0,0,0);
    rider[2]->addObj(f1,sizeof(f1));
    rider[2]->updateModelParams(0,0.5,0,0,0,0);
    rider[10]->addObj(C1,sizeof(C1));
    rider[10]->updateModelParams(0,0.15,0, 0,0,0);
    // rider[10]->updateModelConstraints(70,-70,70,-70, 70,-70);
    rider[11]->addObj(E1,sizeof(E1));
    rider[11]->updateModelParams(0,0.3,0, 0,0,0);
    //hat
    rider[29]->addObj(pipe, sizeof(pipe));
    rider[29]->updateModelParams(0, 0.1, 0, 10,0,0);
    rider[30]->addObj(h,sizeof(h));
    rider[30]->updateModelParams(0,0.25/2,0,5,45,0);
    rider[31]->addObj(nose,sizeof(nose));
    rider[31]->updateModelParams(0,-0.2,0.15,0,0,90);
    rider[32]->addObj(eyes, sizeof(eyes));
    rider[32]->updateModelParams(0.1,0,0.18,0,0,0);
    rider[33]->addObj(eyes, sizeof(eyes));
    rider[33]->updateModelParams(-0.1,0,0.18,0,0,0);
    //upper left
    rider[12]->addObj(S1, sizeof(S1));
    rider[12]->updateModelParams(-0.36, 0.35, 0 ,0,0,-10);
    rider[14]->addObj(A1, sizeof(A1));
    rider[14]->updateModelParams(0.0, -0.2, 0, 0,0,0);
    rider[16]->addObj(S2, sizeof(S2));
    rider[16]->updateModelParams(0.0, -0.2, 0, 0,0,10);
    rider[18]->addObj(A2, sizeof(A2));
    rider[18]->updateModelParams(0.0, -0.15, 0, 0,0,0);
    rider[22]->addObj(hand,sizeof(hand));
    rider[22]->updateModelParams(0.0,-0.22,0, 45,0,0);
    //upper right
    rider[13]->addObj(S1, sizeof(S1));
    rider[13]->updateModelParams(0.36, 0.35, 0 ,0,0,10);
    rider[15]->addObj(A1, sizeof(A1));
    rider[15]->updateModelParams(0.0, -0.2, 0, 0,0,0);
    rider[17]->addObj(S2, sizeof(S2));
    rider[17]->updateModelParams(0.0, -0.2, 0, 0,0,-10);
    rider[19]->addObj(A2, sizeof(A2));
    rider[19]->updateModelParams(0.0, -0.15, 0, 0,0,0);
    rider[23]->addObj(hand,sizeof(hand));
    rider[23]->updateModelParams(0.0,-0.22,0, 0,0,0);
    //lower body
    rider[3]->addObj(main_butt,sizeof(main_butt));
    rider[3]->updateModelParams(0.0,-0.1,0,0,0,0);
    //lower left
    rider[20]->addObj(butt,sizeof(butt));
    rider[20]->updateModelParams(-0.05,-0.10,0,0,0,0);
    rider[4]->addObj(thigh,sizeof(thigh));
    rider[4]->updateModelParams(-0.05,-0.2,0,0,0,0);
    rider[6]->addObj(S3,sizeof(S3));
    rider[6]->updateModelParams(0,-0.25,0, 0,0,0);
    // rider[7]->updateModelConstraints(0,0,0,0,3,-30);
    rider[8]->addObj(shin,sizeof(shin));
    rider[8]->updateModelParams(0.0,-0.18,0,0,0,0);
    rider[24]->addObj(ankle,sizeof(ankle));
    rider[24]->updateModelParams(0,-0.25,0,0,90,90);
    rider[26]->addObj(feet, sizeof(feet));
    rider[26]->updateModelParams(-0.025,0.08,0, 0,45,0);
    //lower right
    rider[21]->addObj(butt,sizeof(butt));
    rider[21]->updateModelParams(0.05,-0.1,0,0,0,0);
    rider[5]->addObj(thigh,sizeof(thigh));
    rider[5]->updateModelParams(0.05,-0.2,0,0,0,0);
    rider[7]->addObj(S3,sizeof(S3));
    rider[7]->updateModelParams(0,-0.25,0, 0,0,0);
    // rider[7]->updateModelConstraints(0,0,0,0,3,-30);
    rider[9]->addObj(shin,sizeof(shin));
    rider[9]->updateModelParams(0.0,-0.18,0,0,0,0);
    rider[25]->addObj(ankle,sizeof(ankle));
    rider[25]->updateModelParams(0,-0.25,0,0,90,90);
    rider[27]->addObj(feet, sizeof(feet));
    rider[27]->updateModelParams(-0.025,0.08,0, 0,45,0);
}