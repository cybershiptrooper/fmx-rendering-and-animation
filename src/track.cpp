#include "track.hpp"

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
    std::cout << std::endl;

    //Initialize GL state
    cg::initGL();
    initBuffersGL();

    //Default Setting
    std::cout << "Press C to toggle control between Cameras\n";
    std::cout << "Press Z to toggle control between Models and Camera\n";
    menu = 'R';
    camera = '1';

    while(!glfwWindowShouldClose(window))
    {
        renderGL();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    skyboxShader.deleteShader();
    shaderProgram.deleteShader();
    glfwTerminate();
    return 0;
}

void renderGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    matrixStack.clear();

    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    switch(camera)
    {
        case '1':
            cameraPos = cameraPos1;
            cameraFront = cameraFront1;
            break;

        case '2':
            cameraPos = cameraPos2;
            cameraFront = cameraFront2;
            break;

        case '3':
            cameraPos = cameraPos3;
            cameraFront = cameraFront3;
            break;
    }

    if(animationSwitch && (1/24-deltaTime)<=0)
    {
        applyFrame(keyframeNumber++);
        if(keyframeNumber == allFrames.size())
        {
            keyframeNumber = 0;
            animationSwitch = 0;
        }
    }

    glm::mat4 view = glm::mat4(1.0f);
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f),4.0f/3.0f,0.1f,100.0f);

    shaderProgram.setMat4(view,"view");
    shaderProgram.setMat4(projection,"projection");
    shaderProgram.setVec3(cameraPos,"viewPos");
    shaderProgram.setFloat(trackL1,"trackL1");
    shaderProgram.setFloat(trackL2,"trackL2");
    shaderProgram.setFloat(riderSL,"riderSL");
    shaderProgram.setFloat(bikeHL,"bikeHL");
    shaderProgram.setVec3(slPos,"spotlightPos");
    shaderProgram.setVec3(hlPos,"headlightPos");
    matrixStack.push_back(view);
    shaderProgram.useShader();
    track[0]->renderTree();

    //------------------------------------------------------------------------------------------------
    // draw skybox as last
    glDepthFunc(GL_LEQUAL);
    // skyboxShader.useShader();
    glm::mat4 skyview = glm::mat4(glm::mat3(view));//remove translation
    skyboxShader.setMat4(skyview, "view");
    skyboxShader.setMat4(projection, "projection");
    skyboxShader.setMat4(scalesky, "scalesky");
    // skybox cube
    glBindVertexArray(skyboxVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
    // glDrawArrays(GL_TRIANGLES, 0, 36);
    //glBindVertexArray(0);
    glDepthFunc(GL_LESS);

    //------------------------------------------------------------------------------------------------
}

void initBuffersGL()
{
    shaderProgram.addShaders("./shaders/vshader.glsl","./shaders/fshader.glsl");
    skyboxShader.addShaders("./shaders/skybox.vs", "./shaders/skybox.fs");

    //------------------------------------------------------------------------------------------------
    //bind skybox
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    scalesky = glm::mat4(1.0f);
    float f = 100;
    scalesky = glm::scale(scalesky,glm::vec3(f,f,f));
    //bind textures
    skyboxTexture = loadCubemap(skybox);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
    //bind shader

    //------------------------------------------------------------------------------------------------
    unsigned int wood = loadTexture("./data/container.jpg");

    generateTrackVertices(PathPoints,pathColour,Path);
    generatePipeVertices(Pipe1Points,Colour,Pipe1);
    generateCuboidVertices(jumpPlatformPoints,Colour,jumpPlatform);
    generateRingVertices(jumpRing1Points,Colour,jumpRing1);
    generateCuboidVertices(cuboidStopperPoints,Colour,CuboidStopper);
    generatePrismVertices(FrustumSlopePoints,Colour,FrustumSlope);
    generatePipeVertices(FlyingPipePoints,Colour,FlyingPipe);
    generatePrismVertices(TriangularBreakersPoints,Colour,TriangularBreakers);

    track[0] = new cg::Node(NULL,NUM_NODES(NUM_TRACK(5,2)));
    track[1] = new cg::Node(track[0],NUM_NODES(NUM_TRACK(5,2)));
    track[2] = new cg::Node(track[0],NUM_NODES(NUM_TRACK(5,2)));
    track[3] = new cg::Node(track[0],NUM_NODES(NUM_TRACK(5,2)));
    track[4] = new cg::Node(track[0],NUM_NODES(NUM_TRACK(5,2)));
    track[5] = new cg::Node(track[0],NUM_NODES(NUM_TRACK(5,2)));
    track[6] = new cg::Node(track[0],NUM_NODES(NUM_TRACK(5,2)));
    track[7] = new cg::Node(track[0],NUM_NODES(NUM_TRACK(5,2)));
    track[8] = new cg::Node(track[0],NUM_NODES(NUM_TRACK(5,2)));
    track[9] = new cg::Node(track[0],NUM_NODES(NUM_TRACK(5,2)));

    track[10] = new cg::Node(track[0],NUM_NODES(NUM_PIPED_PRISM(6)));
    track[11] = new cg::Node(track[0],NUM_NODES(NUM_CUBOID));
    track[12] = new cg::Node(track[0],NUM_NODES(NUM_RING));

    track[13] = new cg::Node(track[1],NUM_NODES(NUM_CUBOID),wood,1);
    track[14] = new cg::Node(track[1],NUM_NODES(NUM_CUBOID),wood,1);
    track[15] = new cg::Node(track[1],NUM_NODES(NUM_CUBOID));

    track[16] = new cg::Node(track[2],NUM_NODES(NUM_CYLINDER));
    track[17] = new cg::Node(track[2],NUM_NODES(NUM_PIPE));

    track[18] = new cg::Node(track[3],NUM_NODES(NUM_CUBOID));
    track[19] = new cg::Node(track[3],NUM_NODES(NUM_CUBOID));
    track[20] = new cg::Node(track[3],NUM_NODES(NUM_CUBOID));

    track[21] = new cg::Node(track[4],NUM_NODES(NUM_PRISM(3)));
    track[22] = new cg::Node(track[4],NUM_NODES(NUM_PRISM(3)));
    track[23] = new cg::Node(track[4],NUM_NODES(NUM_PRISM(3)));
    track[24] = new cg::Node(track[4],NUM_NODES(NUM_PRISM(3)));
    track[25] = new cg::Node(track[4],NUM_NODES(NUM_PRISM(3)));
    track[26] = new cg::Node(track[4],NUM_NODES(NUM_CUBOID));

    track[27] = new cg::Node(track[5],NUM_NODES(NUM_CUBOID));
    track[28] = new cg::Node(track[5],NUM_NODES(NUM_CUBOID));
    track[29] = new cg::Node(track[5],NUM_NODES(NUM_CUBOID));

    track[30] = new cg::Node(track[6],NUM_NODES(NUM_CYLINDER));
    track[31] = new cg::Node(track[6],NUM_NODES(NUM_PIPE));

    track[32] = new cg::Node(track[7],NUM_NODES(NUM_CUBOID));
    track[33] = new cg::Node(track[7],NUM_NODES(NUM_CUBOID));
    track[34] = new cg::Node(track[7],NUM_NODES(NUM_CUBOID));

    track[35] = new cg::Node(track[8],NUM_NODES(NUM_PIPED_PRISM(6)));
    track[36] = new cg::Node(track[8],NUM_NODES(NUM_CUBOID));
    track[37] = new cg::Node(track[8],NUM_NODES(NUM_RING));

    track[0]->addObj(Path,sizeof(Path));
    track[0]->updateModelParams(0,-0.3,0,0,0,0);

    track[1]->addObj(Path,sizeof(Path));
    track[1]->updateModelParams(0.5,0,-10,0,-5,10);

    track[2]->addObj(Path,sizeof(Path));
    track[2]->updateModelParams(1,0,-20,0,0,30);

    track[3]->addObj(Path,sizeof(Path));
    track[3]->updateModelParams(1,0,-30,0,0,45);

    track[4]->addObj(Path,sizeof(Path));
    track[4]->updateModelParams(-2.5,0,-40,-45,90,90);

    track[5]->addObj(Path,sizeof(Path));
    track[5]->updateModelParams(-6,0,-30,0,180,45);

    track[6]->addObj(Path,sizeof(Path));
    track[6]->updateModelParams(-6,0,-20,0,180,30);

    track[7]->addObj(Path,sizeof(Path));
    track[7]->updateModelParams(-5.5,0,-10,0,185,10);

    track[8]->addObj(Path,sizeof(Path));
    track[8]->updateModelParams(-5,0,0,0,180,0);

    track[9]->addObj(Path,sizeof(Path));
    track[9]->updateModelParams(-2.5,0,6,0,-90,0);

    track[10]->addObj(Pipe1,sizeof(Pipe1));
    track[10]->updateModelParams(-0.5,0.15+2*TRACK_STEP,3.8,90,10,15);

    track[11]->addObj(jumpPlatform,sizeof(jumpPlatform));
    track[11]->updateModelParams(0,0.1+TRACK_STEP,-2.6+TRACK_STEP,-55,0,0);

    track[12]->addObj(jumpRing1,sizeof(jumpRing1));
    track[12]->updateModelParams(0,1,-5,90,0,0);

    track[13]->addObj(CuboidStopper,sizeof(CuboidStopper));
    track[13]->updateModelParams(-0.3,0.2+2*TRACK_STEP,1.5,0,30,0);

    track[14]->addObj(CuboidStopper,sizeof(CuboidStopper));
    track[14]->updateModelParams(0.5,0.2+2*TRACK_STEP,-0.5,0,-30,0);

    track[15]->addObj(jumpPlatform,sizeof(jumpPlatform));
    track[15]->updateModelParams(0,0.1+TRACK_STEP,-2.6+TRACK_STEP,-55,0,0);

    track[16]->addObj(FrustumSlope,sizeof(FrustumSlope));
    track[16]->updateModelParams(0,0.2+2*TRACK_STEP,-1,0,0,0);

    track[17]->addObj(FlyingPipe,sizeof(FlyingPipe));
    track[17]->updateModelParams(0,2,-4.5,90,0,0);

    track[18]->addObj(CuboidStopper,sizeof(CuboidStopper));
    track[18]->updateModelParams(-0.3,0.2+2*TRACK_STEP,1.5,0,30,0);

    track[19]->addObj(CuboidStopper,sizeof(CuboidStopper));
    track[19]->updateModelParams(0.5,0.2+2*TRACK_STEP,-0.5,0,-30,0);

    track[20]->addObj(jumpPlatform,sizeof(jumpPlatform));
    track[20]->updateModelParams(0,0.1+TRACK_STEP,-2.6+TRACK_STEP,-55,0,0);

    track[21]->addObj(TriangularBreakers,sizeof(TriangularBreakers));
    track[21]->updateModelParams(0,0.12,-1,0,0,90);

    track[22]->addObj(TriangularBreakers,sizeof(TriangularBreakers));
    track[22]->updateModelParams(0,0.12,-0.5,0,0,90);

    track[23]->addObj(TriangularBreakers,sizeof(TriangularBreakers));
    track[23]->updateModelParams(0,0.12,0,0,0,90);

    track[24]->addObj(TriangularBreakers,sizeof(TriangularBreakers));
    track[24]->updateModelParams(0,0.12,0.5,0,0,90);

    track[25]->addObj(TriangularBreakers,sizeof(TriangularBreakers));
    track[25]->updateModelParams(0,0.12,1,0,0,90);

    track[26]->addObj(jumpPlatform,sizeof(jumpPlatform));
    track[26]->updateModelParams(0,0.1+TRACK_STEP,-2.6+TRACK_STEP,-30,0,0);

    track[27]->addObj(CuboidStopper,sizeof(CuboidStopper));
    track[27]->updateModelParams(-0.3,0.2+2*TRACK_STEP,1.5,0,30,0);

    track[28]->addObj(CuboidStopper,sizeof(CuboidStopper));
    track[28]->updateModelParams(0.5,0.2+2*TRACK_STEP,-0.5,0,-30,0);

    track[29]->addObj(jumpPlatform,sizeof(jumpPlatform));
    track[29]->updateModelParams(0,0.1+TRACK_STEP,-2.6+TRACK_STEP,-55,0,0);

    track[30]->addObj(FrustumSlope,sizeof(FrustumSlope));
    track[30]->updateModelParams(0,0.2+2*TRACK_STEP,-1,0,0,0);

    track[31]->addObj(FlyingPipe,sizeof(FlyingPipe));
    track[31]->updateModelParams(0,2,-4.5,90,0,0);

    track[32]->addObj(CuboidStopper,sizeof(CuboidStopper));
    track[32]->updateModelParams(-0.3,0.2+2*TRACK_STEP,1.5,0,30,0);

    track[33]->addObj(CuboidStopper,sizeof(CuboidStopper));
    track[33]->updateModelParams(0.5,0.2+2*TRACK_STEP,-0.5,0,-30,0);

    track[34]->addObj(jumpPlatform,sizeof(jumpPlatform));
    track[34]->updateModelParams(0,0.1+TRACK_STEP,-2.6+TRACK_STEP,-55,0,0);

    track[35]->addObj(Pipe1,sizeof(Pipe1));
    track[35]->updateModelParams(0.2,0.65+2*TRACK_STEP,3.8,90,0,0);

    track[36]->addObj(jumpPlatform,sizeof(jumpPlatform));
    track[36]->updateModelParams(0,0.1+TRACK_STEP,-2.6+TRACK_STEP,-55,0,0);

    track[37]->addObj(jumpRing1,sizeof(jumpRing1));
    track[37]->updateModelParams(-0.5,0.5,-4,90,210,0);


    ///////////

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
    generateCuboidVertices(seat1Points, seatOne, seat1);
    generatePrismVertices(seatcurvePoints, seatOne, seatcurve);
    generateCuboidVertices(seat2Points, seatOne, seat2);
    generatePrismVertices(CH2Points, handle_color, CH2);
    generatePrismVertices(CH1Points, handle_color, CH1);
    generateEllipsoidVertices(dialPoints, seatOne, dial);
    generatePrismVertices(exhaust3Points, black, exhaust3);
    //generateCuboidVertices(decorPoints, seat, decor);
    generatePrismVertices(decorPoints, yellow, decor);
    //engine
    bike[0] = new cg::Node(track[9],NUM_NODES(NUM_CYLINDER)); 
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
    // bike[0]->updateModelParams(0,-0.1,0.,90,0,0);
    bike[0]->updateModelParams(0,0.58,-1,90,0,90,0.4,0.4,0.4);
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


    ////////////
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
    //root
    rider[0] = new cg::Node(track[9],NUM_NODES(NUM_CYLINDER)); //hip
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
    rider[0]->updateModelParams(0,0.6,1,0,-90,0,0.5,0.5,0.5);
    //upper body
    rider[28]->addObj(bendjoint, sizeof(bendjoint));
    rider[28]->updateModelParams(0,0.1,0,0,0,0);
    rider[1]->addObj(f2,sizeof(f2));
    rider[1]->updateModelParams(0,0.4,0,0,0,0);
    rider[2]->addObj(f1,sizeof(f1));
    rider[2]->updateModelParams(0,0.5,0,0,0,0);
    rider[10]->addObj(C1,sizeof(C1));
    rider[10]->updateModelParams(0,0.15,0, 0,0,0);
    rider[11]->addObj(E1,sizeof(E1));
    rider[11]->updateModelParams(0,0.3,0, 0,0,0);
    //hat
    rider[29]->addObj(pipe, sizeof(pipe));
    rider[29]->updateModelParams(0, 0.1, 0, 10,0,0);
    rider[30]->addObj(h,sizeof(h));
    rider[30]->updateModelParams(0,0.25/2,0,5,45,0);
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
    rider[9]->addObj(shin,sizeof(shin));
    rider[9]->updateModelParams(0.0,-0.18,0,0,0,0);
    rider[25]->addObj(ankle,sizeof(ankle));
    rider[25]->updateModelParams(0,-0.25,0,0,90,90);
    rider[27]->addObj(feet, sizeof(feet));
    rider[27]->updateModelParams(-0.025,0.08,0, 0,45,0);
}

std::string itos(int k) 
{
    return std::to_string(k) + " ";
}

std::string itos(float k) 
{
    return std::to_string(k) + " ";
}

std::string itos(bool k) 
{
    return std::to_string(k) + " ";
}

void storeFrame()
{
    std::ofstream values;
    values.open("./data/keyframes.txt", std::ios::app);
    values << itos(trackL1) + itos(trackL2) + itos(riderSL) + itos(bikeHL);
    values << itos(cameraPos.x) + itos(cameraPos.y) + itos(cameraPos.z);
    values << itos(cameraFront.x) + itos(cameraFront.y) + itos(cameraFront.z);
    track[0]->storeNodeParams(values);
    values << "\n";
    values.close();
}

void interpolateFrames()
{
    for(int i=0;i<keyFrames.size()-1;i++)
    {
        std::vector<std::vector<float> > frames = interpolateFramesHelper(i);
        allFrames.insert(allFrames.end(),frames.begin(),frames.end());
    }
    allFrames.push_back(keyFrames.back());
}

std::vector<std::vector<float>> interpolateFramesHelper(int n)
{
    std::vector<std::vector<float> > intFrames;

    for(int i=0;i<24;i++)
    {
        std::vector<float> frame;
        for(int j=0;j<4;j++)
        {
            frame.push_back(keyFrames[n][j]);
        }
        for(int j=4;j<keyFrames[n].size();j++)
        {
            frame.push_back((keyFrames[n][j]*(24-i) + keyFrames[n+1][j]*i)/24);
        }

        intFrames.push_back(frame);
        frame.clear();
    }
    return intFrames;
}

std::vector<float> parseFrame(std::string s)
{
    std::vector<float> frame;
    std::istringstream input(s);
    std::string param;
    while(getline(input, param, ' ')) 
    {
        frame.push_back(std::stof(param));
    }
    return frame;
}

void readkeyFrame()
{
    keyFrames.clear();
    std::string frame;
    std::ifstream values("./data/keyframes.txt");
    if(values.is_open())
    {
        while(getline(values,frame))
        {
            keyFrames.push_back(parseFrame(frame));
        }
        values.close();
        std::cout << "Number of Keyframes in the file : " << keyFrames.size() << std::endl;
        interpolateFrames();
    }
    else
    {
        std::cout << "Error opening keyframes text file" << std::endl;
    } 
}

void applyFrame(int n)
{
    std::vector<float> frame = allFrames[n];
    trackL1 = frame[0];
    trackL2 = frame[1];
    riderSL = frame[2];
    bikeHL  = frame[3];
    cameraPos = glm::vec3(frame[4],frame[5],frame[6]);
    cameraFront = glm::vec3(frame[7],frame[8],frame[9]);
    track[0]->updateFrameParams(frame);
}