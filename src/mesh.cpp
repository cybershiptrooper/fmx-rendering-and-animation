#include "mesh.hpp"

float GENERAL_CUBE[] = {
    -0.5f, 0.5f, 0.5f,
     0.5f, 0.5f, 0.5f,
     0.5f,-0.5f, 0.5f,
    -0.5f,-0.5f, 0.5f,
    -0.5f,-0.5f,-0.5f,
     0.5f,-0.5f,-0.5f,
     0.5f, 0.5f,-0.5f,
    -0.5f, 0.5f,-0.5f,
};

float GENERAL_CUBE_NORMALS[] = {
     0.0f, 0.0f, 1.0f,
     0.0f,-1.0f, 0.0f,
     0.0f, 0.0f,-1.0f,
     0.0f, 1.0f, 0.0f,
     1.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, 0.0f,
};

float GENERAL_CUBE_TEXTURES[] = {
    0.0f, 1.0f,
    1.0f, 1.0f,
    1.0f, 0.0f,
    1.0f, 0.0f,
    0.0f, 0.0f,
    0.0f, 1.0f,
};

void generateTrackVertices(float* givenPoints, float* givenColours, float* vertices)
{
    float step = TRACK_STEP;
    float length = givenPoints[0]/2;
    float width  = givenPoints[1]/2;
    int stepX = (int)givenPoints[1]/step;
    int stepY = (int)givenPoints[0]/step;
    int offSet;

    offSet = stepX*stepY*6;
    for(int h=0;h<2;h++)
    {
        for(int i=0;i<stepY;i++)
        {
            for(int j=0;j<stepX;j++)
            {
                float helperOne[] = {width-j*step,(-1+h*2)*step,length-i*step};
                float helperTwo[] = {width-(j+1)*step,(-1+h*2)*step,length-i*step};
                float helperTwoStar[] = {width-(j+1)*step,(-1+h*2)*step,length-(i+1)*step};
                float helperOneStar[] = {width-(j+0)*step,(-1+h*2)*step,length-(i+1)*step};

                for(int k=0;k<11;k++)
                {
                    if(k<3)
                    {
                        vertices[(offSet*h+i*stepX*6+j*6+0)*11+k] = vertices[(offSet*h+i*stepX*6+j*6+5)*11+k] = helperOne[k];
                        vertices[(offSet*h+i*stepX*6+j*6+2)*11+k] = vertices[(offSet*h+i*stepX*6+j*6+3)*11+k] = helperTwoStar[k];
                        vertices[(offSet*h+i*stepX*6+j*6+1)*11+k] = helperTwo[k];
                        vertices[(offSet*h+i*stepX*6+j*6+4)*11+k] = helperOneStar[k];
                    }
                    else if(k<6 && k>2)
                    {
                        for(int l=0;l<6;l++)
                        {
                            if(j>1 && j<stepX-2)
                                vertices[(offSet*h+i*stepX*6+j*6+l)*11+k] = givenColours[k%3+rand()%2*3];
                            else
                                vertices[(offSet*h+i*stepX*6+j*6+l)*11+k] = 1;
                        }
                    }
                    else if(k<9 && k>5)
                    {
                        for(int l=0;l<6;l++)
                        {
                            vertices[(offSet*h+i*stepX*6+j*6+l)*11+k] = (k%3 == 1) ? -1+h*2 : 0.0;
                        }
                    }
                    else
                    {
                        
                    }
                }
            }
        }
    }

    for(int h=0;h<2;h++)
    {
        offSet = (stepX*stepY*6)*2+h*stepX*2*6;
        for(int i=0;i<2;i++)
        {
            for(int j=0;j<stepX;j++)
            {
                float helperOne[] = {width-(j+0)*step,-i*step,(-1+h*2)*length};
                float helperTwo[] = {width-(j+1)*step,-i*step,(-1+h*2)*length};
                float helperTwoStar[] = {width-(j+1)*step,(1-i)*step,(-1+h*2)*length};
                float helperOneStar[] = {width-(j+0)*step,(1-i)*step,(-1+h*2)*length};
                for(int k=0;k<11;k++)
                {
                    if(k<3)
                    {
                        vertices[(offSet+i*stepX*6+j*6+0)*11+k] = vertices[(offSet+i*stepX*6+j*6+5)*11+k] = helperOne[k];
                        vertices[(offSet+i*stepX*6+j*6+2)*11+k] = vertices[(offSet+i*stepX*6+j*6+3)*11+k] = helperTwoStar[k];
                        vertices[(offSet+i*stepX*6+j*6+1)*11+k] = helperTwo[k];
                        vertices[(offSet+i*stepX*6+j*6+4)*11+k] = helperOneStar[k];
                    }
                    else if(k<6 && k>2)
                    {
                        for(int l=0;l<6;l++)
                        {
                            if(j>1 && j<stepX-2)
                                vertices[(offSet+i*stepX*6+j*6+l)*11+k] = givenColours[k%3+rand()%2*3];
                            else
                                vertices[(offSet+i*stepX*6+j*6+l)*11+k] = 1;
                        }
                    }
                    else if(k<9 && k>5)
                    {
                        for(int l=0;l<6;l++)
                        {
                            vertices[(offSet+i*stepX*6+j*6+l)*11+k] = (k%3 == 2) ? -1+h*2 : 0.0;
                        }
                    }
                    else
                    {

                    }
                }
            }
        }
    }

    for(int h=0;h<2;h++)
    {
        offSet = (stepX*stepY*6+stepX*2*6)*2+h*stepY*2*6;
        for(int i=0;i<2;i++)
        {
            for(int j=0;j<stepY;j++)
            {
                float helperOne[] = {(-1+h*2)*width,-i*step,length-(j+0)*step};
                float helperTwo[] = {(-1+h*2)*width,-i*step,length-(j+1)*step};
                float helperTwoStar[] = {(-1+h*2)*width,(1-i)*step,length-(j+1)*step};
                float helperOneStar[] = {(-1+h*2)*width,(1-i)*step,length-(j+0)*step};
                for(int k=0;k<11;k++)
                {
                    if(k<3)
                    {
                        vertices[(offSet+i*stepY*6+j*6+0)*11+k] = vertices[(offSet+i*stepY*6+j*6+5)*11+k] = helperOne[k];
                        vertices[(offSet+i*stepY*6+j*6+2)*11+k] = vertices[(offSet+i*stepY*6+j*6+3)*11+k] = helperTwoStar[k];
                        vertices[(offSet+i*stepY*6+j*6+1)*11+k] = helperTwo[k];
                        vertices[(offSet+i*stepY*6+j*6+4)*11+k] = helperOneStar[k];
                    }
                    else if(k<6 && k>2)
                    {
                        for(int l=0;l<6;l++)
                            vertices[(offSet+i*stepY*6+j*6+l)*11+k] = 1;
                    }
                    else if(k<9 && k>5)
                    {
                        for(int l=0;l<6;l++)
                        {
                            vertices[(offSet+i*stepY*6+j*6+l)*11+k] = (k%3 == 0) ? -1+h*2 : 0.0;
                        }
                    }
                    else
                    {

                    }
                }
            }
        }
    }
}

void generateRingVertices(float* givenPoints, float* givenColours, float* vertices)
{
    int sectors = (int)givenPoints[0];
    float sectorAngle = 2*M_PI/sectors;
    float innerRX = givenPoints[1];
    float innerRY = givenPoints[2];
    float outerRX = givenPoints[3];
    float outerRY = givenPoints[4];
    std::vector<float> innerCircle,outerCircle;
    for(int i=0;i<sectors;i++)
    {
        innerCircle.push_back(cos(i*sectorAngle)*innerRX);
        innerCircle.push_back(0);
        innerCircle.push_back(sin(i*sectorAngle)*innerRY);
        outerCircle.push_back(cos(i*sectorAngle)*outerRX);
        outerCircle.push_back(0);
        outerCircle.push_back(sin(i*sectorAngle)*outerRY);
    }

    for(int i=0;i<sectors;i++)
    {
        for(int k=0;k<11;k++)
        {
            if(k<3)
            {
                vertices[(i*6+0)*11+k] = vertices[(i*6+5)*11+k] = innerCircle[((i+0)%sectors)*3+k];
                vertices[(i*6+2)*11+k] = vertices[(i*6+3)*11+k] = outerCircle[((i+1)%sectors)*3+k];
                vertices[(i*6+1)*11+k] = innerCircle[((i+1)%sectors)*3+k];
                vertices[(i*6+4)*11+k] = outerCircle[((i+0)%sectors)*3+k];
            }
            else if(k<6 && k>2)
            {
                for(int j=0;j<6;j++)
                    vertices[(i*6+j)*11+k] = givenColours[k%3+rand()%2*3];
            }
            else if(k<9 && k>5)
            {
                for(int j=0;j<6;j++)
                    vertices[(i*6+j)*11+k] = (k%3==1)? 1.0 : 0.0;
            }
            else
            {

            }
        }
    }
}

void generatePipeVertices(float* givenPoints, float* givenColours, float* vertices)
{
    int sectors = (int)givenPoints[0];
    float sectorAngle = 2*M_PI/sectors;
    float Down_innerRX = givenPoints[1];float Top_innerRX = givenPoints[5];
    float Down_innerRY = givenPoints[2];float Top_innerRY = givenPoints[6];
    float Down_outerRX = givenPoints[3];float Top_outerRX = givenPoints[7];
    float Down_outerRY = givenPoints[4];float Top_outerRY = givenPoints[8];
    float height = givenPoints[9];
    std::vector<float> Down_innerCircle,Down_outerCircle,Top_innerCircle,Top_outerCircle;
    for(int i=0;i<sectors;i++)
    {
        Down_innerCircle.push_back(cos(i*sectorAngle)*Down_innerRX);
        Down_innerCircle.push_back(-height/2);
        Down_innerCircle.push_back(sin(i*sectorAngle)*Down_innerRY);
        Down_outerCircle.push_back(cos(i*sectorAngle)*Down_outerRX);
        Down_outerCircle.push_back(-height/2);
        Down_outerCircle.push_back(sin(i*sectorAngle)*Down_outerRY);
        Top_innerCircle.push_back(cos(i*sectorAngle)*Top_innerRX);
        Top_innerCircle.push_back(height/2);
        Top_innerCircle.push_back(sin(i*sectorAngle)*Top_innerRY);
        Top_outerCircle.push_back(cos(i*sectorAngle)*Top_outerRX);
        Top_outerCircle.push_back(height/2);
        Top_outerCircle.push_back(sin(i*sectorAngle)*Top_outerRY);
    }

    std::vector<float> mapping[] = {
        Down_innerCircle,Down_outerCircle,Down_innerCircle,Down_outerCircle,
        Top_innerCircle,Top_outerCircle,Top_innerCircle,Top_outerCircle,
        Down_innerCircle,Top_innerCircle,Down_innerCircle,Top_innerCircle,
        Down_outerCircle,Top_outerCircle,Down_outerCircle,Top_outerCircle
    };

    for(int s=0;s<=18;s+=6)
    {
        for(int i=0;i<sectors;i++)
        {
            for(int k=0;k<11;k++)
            {
                if(k<3)
                {
                    vertices[(s*sectors+i*6+0)*11+k] = vertices[(s*sectors+i*6+5)*11+k] = mapping[s/6*4 +0][((i+0)%sectors)*3+k];
                    vertices[(s*sectors+i*6+2)*11+k] = vertices[(s*sectors+i*6+3)*11+k] = mapping[s/6*4 +1][((i+1)%sectors)*3+k];
                    vertices[(s*sectors+i*6+1)*11+k] = mapping[s/6*4 +2][((i+1)%sectors)*3+k];
                    vertices[(s*sectors+i*6+4)*11+k] = mapping[s/6*4 +3][((i+0)%sectors)*3+k];
                }
                else if(k<6 && k>2)
                {
                    for(int j=0;j<6;j++)
                        vertices[(s*sectors+i*6+j)*11+k] = givenColours[k%3+rand()%2*3];
                }
                else if(k<9 && k>5)
                {
                    //bottom and top face
                    if(s<12)
                    {
                        for(int j=0;j<6;j++)
                            vertices[(s*sectors+i*6+j)*11+k] = (k%3==1)? -1+s/6*2 : 0.0;
                    }
                    //innner surface
                    else if(s==12)
                    {
                        vertices[(s*sectors+i*6+0)*11+k] = vertices[(s*sectors+i*6+5)*11+k] = (k%3==1)? 0.0 : -mapping[s/6*4 +0][((i+0)%sectors)*3+k%3];
                        vertices[(s*sectors+i*6+2)*11+k] = vertices[(s*sectors+i*6+3)*11+k] = (k%3==1)? 0.0 : -mapping[s/6*4 +1][((i+1)%sectors)*3+k%3];
                        vertices[(s*sectors+i*6+1)*11+k] = (k%3==1)? 0.0 : -mapping[s/6*4 +2][((i+1)%sectors)*3+k%3];
                        vertices[(s*sectors+i*6+4)*11+k] = (k%3==1)? 0.0 : -mapping[s/6*4 +3][((i+0)%sectors)*3+k%3];
                    }
                    //innner surface
                    else if(s==18)
                    {
                        vertices[(s*sectors+i*6+0)*11+k] = vertices[(s*sectors+i*6+5)*11+k] = (k%3==1)? 0.0 : mapping[s/6*4 +0][((i+0)%sectors)*3+k%3];
                        vertices[(s*sectors+i*6+2)*11+k] = vertices[(s*sectors+i*6+3)*11+k] = (k%3==1)? 0.0 : mapping[s/6*4 +1][((i+1)%sectors)*3+k%3];
                        vertices[(s*sectors+i*6+1)*11+k] = (k%3==1)? 0.0 : mapping[s/6*4 +2][((i+1)%sectors)*3+k%3];
                        vertices[(s*sectors+i*6+4)*11+k] = (k%3==1)? 0.0 : mapping[s/6*4 +3][((i+0)%sectors)*3+k%3];
                    }
                }
                else
                {

                }
            }
        }
    }
}

void generateDiscVertices(float* givenPoints, float* givenColours, float* vertices)
{
    int   sectors  = (int)givenPoints[0];
    float secAngle = M_PI * 2 / sectors;
    std::vector<float> unitCircle;
    for(int i=0;i<sectors;i++)
    {	
        unitCircle.push_back(cos(i*secAngle));
        unitCircle.push_back(0.0);
        unitCircle.push_back(sin(i*secAngle));
    }
    for(int i=0;i<3*sectors;i+=3)
    {
        for(int j=0;j<3;j++)
        {   
            for(int k=0;k<11;k++)
            {	
                int helperOne = (k==2)? 2 : 1;
                if(k<3)
                {
                    vertices[(i+j)*11+k] = (j==1)? 0.0 : unitCircle[((i/3+j/2)*3+k)%(3*sectors)] * givenPoints[helperOne];
                }
                else if(k<6 && k>2)
                {
                    vertices[(i+j)*11+k] = givenColours[k%3+rand()%2*3];
                }
                else if(k<9 && k>5)
                {
                    vertices[(i+j)*11+k] = (k%3==1)? 1.0 : 0.0;
                }
                else
                {
                    
                }
            }
        }
    }
}

void generatePrismVertices(float* givenPoints, float* givenColours, float* vertices)
{
    int   sectors  = (int)givenPoints[0];
    float height   = givenPoints[5];
    float secAngle = M_PI * 2 / sectors;
    std::vector<float> unitCircle;
    for(int i=0;i<sectors;i++)
    {
        unitCircle.push_back(cos(i*secAngle));
        unitCircle.push_back(0.0);
        unitCircle.push_back(sin(i*secAngle));
    }

    // top and bottom disc
    for(int h=0;h<2;h++)
    {
        for(int i=h*3*sectors;i<3*sectors*(h+1);i+=3)
        {
            for(int j=0;j<3;j++)
            {   
                for(int k=0;k<11;k++)
                {
                    if(k<3)
                    {
                        vertices[(i+j)*11+k]  = (j==1)? 0.0 : unitCircle[((i/3+j/2)*3+k)%(3*sectors)];
                        vertices[(i+j)*11+k] *= (k==2)? givenPoints[h*2 + 2] : givenPoints[h*2 + 1];
                        if(h)
                            vertices[(i+j)*11+k] += (k==1)? height/2 : 0.0;
                        else
                            vertices[(i+j)*11+k] -= (k==1)? height/2 : 0.0;
                    }
                    else if(k<6 && k>2)
                    {
                        vertices[(i+j)*11+k] = givenColours[k%3+rand()%2*3];
                    }
                    else if(k<9 && k>5)
                    {
                        vertices[(i+j)*11+k] = (k%3==1)? -1+h*2 : 0.0;
                    }
                    else
                    {

                    }
                }
            }
        }
    }

    // curve sigh
    for(int i=0;i<sectors;i++)
    {
        for(int k=0;k<11;k++)
        {
            int helperOne = (k%3==2)? 2 : 1;
            int helperTwo = (k%3==2)? 4 : 3;
            if(k<3)
            {
                vertices[(6*sectors+i*6)*11+k] = vertices[(6*sectors+i*6+5)*11+k] = unitCircle[i*3+k] * givenPoints[helperOne];
                vertices[(6*sectors+i*6)*11+k] = vertices[(6*sectors+i*6+5)*11+k] -= (k==1)? height/2 : 0.0;
                vertices[(6*sectors+i*6+2)*11+k] = vertices[(6*sectors+i*6+3)*11+k] = unitCircle[((i+1)%sectors)*3+k] * givenPoints[helperTwo];
                vertices[(6*sectors+i*6+2)*11+k] = vertices[(6*sectors+i*6+3)*11+k] += (k==1)? height/2 : 0.0;
                vertices[(6*sectors+i*6+4)*11+k] = unitCircle[i*3+k] * givenPoints[helperTwo];
                vertices[(6*sectors+i*6+4)*11+k] += (k==1)? height/2 : 0.0;
                vertices[(6*sectors+i*6+1)*11+k] = unitCircle[((i+1)%sectors)*3+k] * givenPoints[helperOne];
                vertices[(6*sectors+i*6+1)*11+k] -= (k==1)? height/2 : 0.0;
            }
            else if(k<6 && k>2)
            {
                for(int j=0;j<6;j++)
                    vertices[(6*sectors+i*6+j)*11+k] = givenColours[k%3+rand()%2*3];
            }
            else if(k<9 && k>5)
            {
                vertices[(6*sectors+i*6)*11+k] = vertices[(6*sectors+i*6+5)*11+k] = unitCircle[i*3+k%3] * givenPoints[helperOne];
                vertices[(6*sectors+i*6+2)*11+k] = vertices[(6*sectors+i*6+3)*11+k] = unitCircle[((i+1)%sectors)*3+k%3] * givenPoints[helperTwo];
                vertices[(6*sectors+i*6+4)*11+k] = unitCircle[i*3+k%3] * givenPoints[helperTwo];
                vertices[(6*sectors+i*6+1)*11+k] = unitCircle[((i+1)%sectors)*3+k%3] * givenPoints[helperOne];
            }
            else
            {

            }
        }
    }
}

void generateCuboidVertices(float* givenPoints, float* givenColours, float* vertices)
{   
    std::string faces[6] = {"1234", "3456", "5678", "1278", "2367", "1458"};
    for(int i=0;i<6;i++)
    {
        int index[] = {faces[i][0]-'0',faces[i][1]-'0',faces[i][2]-'0',faces[i][3]-'0'};
        for(int k=0;k<11;k++)
        {
            if(k<3)
            {
                vertices[(i*6+0)*11+k] = vertices[(i*6+5)*11+k] = GENERAL_CUBE[(index[0]-1)*3+k]*givenPoints[k];
                vertices[(i*6+2)*11+k] = vertices[(i*6+3)*11+k] = GENERAL_CUBE[(index[2]-1)*3+k]*givenPoints[k];
                vertices[(i*6+1)*11+k] = GENERAL_CUBE[(index[1]-1)*3+k]*givenPoints[k];
                vertices[(i*6+4)*11+k] = GENERAL_CUBE[(index[3]-1)*3+k]*givenPoints[k];
            }
            else if(k<6 && k>2)
            {
                for(int j=0;j<6;j++)
                    vertices[(i*6+j)*11+k] = givenColours[k%3+rand()%2*3];
            }
            else if(k<9 && k>5)
            {
                for(int j=0;j<6;j++)
                    vertices[(i*6+j)*11+k] = GENERAL_CUBE_NORMALS[i*3+k%3];
            }
            else
            {
                for(int j=0;j<6;j++)
                    vertices[(i*6+j)*11+k] = GENERAL_CUBE_TEXTURES[j*2+k%3];
            }
        }
    }

    
}

void generateEllipsoidVertices(float* givenPoints, float* givenColours, float* vertices)
{
    int sectors = (int)givenPoints[0];
    int stacks  = (int)givenPoints[1];
    float sectorAngle = M_PI * 2 / sectors;
    float stackAngle  = M_PI / stacks;
    std::vector<float> Sphere;

    //Top Point
    Sphere.push_back(0);
    Sphere.push_back(givenPoints[4]);
    Sphere.push_back(0);

    float x,y,z,xy;
    for(int i=1;i<stacks;i++)
    { 
        z  = sin(M_PI/2 - i*stackAngle) * givenPoints[4];
        xy = cos(M_PI/2 - i*stackAngle);
        for(int j=0;j<sectors;j++)
        {
            x = xy * cos(j*sectorAngle) * givenPoints[2];
            y = xy * sin(j*sectorAngle) * givenPoints[3];
            Sphere.push_back(x);
            Sphere.push_back(z);
            Sphere.push_back(y);
        }
    }

    //Bottom Point
    Sphere.push_back(0);
    Sphere.push_back(-givenPoints[4]);
    Sphere.push_back(0);

    //Fill vertices sigh
    //Top triangles
    for(int i=0;i<sectors;i++)
    {
        for(int k=0;k<11;k++)
        {
            if(k<3)
            {
                vertices[(i*3+0)*11 +k] = Sphere[((i+0)%sectors+1)*3 +k];
                vertices[(i*3+1)*11 +k] = Sphere[((i+0)%sectors+1)*0 +k];
                vertices[(i*3+2)*11 +k] = Sphere[((i+1)%sectors+1)*3 +k];
            }
            else if(k<6 && k>2)
            {
                for(int j=0;j<3;j++)
                    vertices[(i*3+j)*11 +k] = givenColours[k%3+rand()%2*3];
            }
            else if(k<9 && k>5)
            {
                vertices[(i*3+0)*11 +k] = Sphere[((i+0)%sectors+1)*3 +k%3];
                vertices[(i*3+1)*11 +k] = Sphere[((i+0)%sectors+1)*0 +k%3];
                vertices[(i*3+2)*11 +k] = Sphere[((i+1)%sectors+1)*3 +k%3];
            }
            else
            {
                
            }
        }
    }

    //Bottom triangles
    int offSet = Sphere.size()-(sectors+1)*3;
    for(int i=sectors;i<sectors*2;i++)
    {
        for(int k=0;k<11;k++)
        {
            if(k<3)
            {
                vertices[(i*3+0)*11 +k] = Sphere[offSet+((i+0)%sectors)*3 +k];
                vertices[(i*3+1)*11 +k] = Sphere[Sphere.size()-3 +k];
                vertices[(i*3+2)*11 +k] = Sphere[offSet+((i+1)%sectors)*3 +k];
            }
            else if(k<6 && k>2)
            {
                for(int j=0;j<3;j++)
                    vertices[(i*3+j)*11 +k] = givenColours[k%3+rand()%2*3];
            }
            else if(k<9 && k>5)
            {
                vertices[(i*3+0)*11 +k] = Sphere[offSet+((i+0)%sectors)*3 +k%3];
                vertices[(i*3+1)*11 +k] = Sphere[Sphere.size()-3 +k%3];
                vertices[(i*3+2)*11 +k] = Sphere[offSet+((i+1)%sectors)*3 +k%3];
            }
            else
            {
                
            }
        }
    }

    //Sheets
    for(int s=1;s<stacks-1;s++)
    {
        for(int i=0;i<sectors;i++)
        {
            for(int k=0;k<11;k++)
            {
                if(k<3)
                {
                    vertices[(s*6*sectors+i*6+0)*11+k] = vertices[(s*6*sectors+i*6+5)*11+k] = Sphere[((i+0)%sectors+1 +(s-1)*sectors)*3 +k];
                    vertices[(s*6*sectors+i*6+2)*11+k] = vertices[(s*6*sectors+i*6+3)*11+k] = Sphere[((i+1)%sectors+1 + s   *sectors)*3 +k];
                    vertices[(s*6*sectors+i*6+1)*11+k] = Sphere[((i+1)%sectors+1 +(s-1)*sectors)*3 +k];
                    vertices[(s*6*sectors+i*6+4)*11+k] = Sphere[((i+0)%sectors+1 + s   *sectors)*3 +k];
                }
                else if(k<6 && k>2)
                {
                    for(int j=0;j<6;j++)
                        vertices[(s*6*sectors+i*6+j)*11+k] = givenColours[k%3+rand()%2*3];
                }
                else if(k<9 && k>5)
                {
                    vertices[(s*6*sectors+i*6+0)*11+k] = vertices[(s*6*sectors+i*6+5)*11+k] = Sphere[((i+0)%sectors+1 +(s-1)*sectors)*3 +k%3];
                    vertices[(s*6*sectors+i*6+2)*11+k] = vertices[(s*6*sectors+i*6+3)*11+k] = Sphere[((i+1)%sectors+1 + s   *sectors)*3 +k%3];
                    vertices[(s*6*sectors+i*6+1)*11+k] = Sphere[((i+1)%sectors+1 +(s-1)*sectors)*3 +k%3];
                    vertices[(s*6*sectors+i*6+4)*11+k] = Sphere[((i+0)%sectors+1 + s   *sectors)*3 +k%3];
                }
                else
                {
                    
                }
            }
        }
    }
}