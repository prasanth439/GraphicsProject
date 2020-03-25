#include<glm/glm.hpp>
#include<glm/gtc/quaternion.hpp>
#include<iostream>
#include<vector>
#include<stdio.h>
#include<time.h> 

#define pixelsx 640
#define pixelsy 480
#define pixelsxw (1.0/640)
#define pixelsyw (1.0/480)
#define numCellsPerAxis 5
// #define  max_z  1.0
// #define  max_distance  1.0
// // #define sampler2D worley_points;
// #define  number_of_points 25

#define PI 3.141592654
#define TWOPI 6.283185308

using namespace std;



vector<glm::vec3> points;
vector<glm::vec3> newpoints;
vector<double> minidistance(640*480);
vector<glm::vec3> colors;

// float dis(glm::vec3 a,glm::vec3)
void CreateWorleyPointsBuffer () {
         
        points.resize(numCellsPerAxis * numCellsPerAxis );
        newpoints.resize((numCellsPerAxis+2) *(numCellsPerAxis+2));
        float cellSize = 1.0 / numCellsPerAxis;
        for (int x = 0; x < numCellsPerAxis; x++) {
            for (int y = 0; y < numCellsPerAxis; y++) {
                
                    double randomX = static_cast <double> (rand()) / static_cast <double> (RAND_MAX);
                    double randomY = static_cast <double> (rand()) / static_cast <double> (RAND_MAX);
                    // cout<<randomX<<" "<<randomY<<" "<<endl;
                    // float randomX = (float) rand();
                    // float randomY = (float) rand();
                    // float randomZ = (float) rand();
                    glm::vec3 randomOffset =cellSize *glm::vec3(randomX, randomY,0) ;
                    glm::vec3 cellCorner =  cellSize *glm::vec3 (x, y, 0) ;

                    int index = x + numCellsPerAxis * (y);
                    // int index1 = x + numCellsPerAxis * (y + z * numCellsPerAxis);

                    points[index] = cellCorner + randomOffset;
                
            }
        }

        for(int x=0;x<numCellsPerAxis+2;x++)
        {
            for (int  y = 0; y < numCellsPerAxis+2; y++)

            {
                int index = x + numCellsPerAxis * (y);
                int index1 = x-1 + numCellsPerAxis * (y-1);

               if(x==0)
               {
                   if(y==0)
                   {
                       index1= numCellsPerAxis-1 + numCellsPerAxis * (numCellsPerAxis-1);
                       newpoints[index]=points[index1]-glm::vec3(1.0,1.0,0.0);  
                   }
                   else if(y==numCellsPerAxis+1)
                   {
                    index1= numCellsPerAxis-1 ;
                    newpoints[index]=points[index1]-glm::vec3(1.0,-1.0,0.0);;   
                   }
                   else
                   {
                    index1= numCellsPerAxis-1 + numCellsPerAxis * (y);
                    newpoints[index]=points[index1]-glm::vec3(1.0,0.0,0.0);;   
                   }
                   
                   
               }
               else if(x==numCellsPerAxis+1)
                {

                    if(y==0)
                   {
                       index1=  numCellsPerAxis * (numCellsPerAxis-1);
                       newpoints[index]=points[index1]-glm::vec3(-1.0,1.0,0.0);  
                   }
                   else if(y==numCellsPerAxis+1)
                   {
                    index1= 0 ;
                    newpoints[index]=points[index1]-glm::vec3(-1.0,-1.0,0.0);;   
                   }
                   else
                   {
                    index1= 0 + numCellsPerAxis * (y);
                    newpoints[index]=points[index1]-glm::vec3(-1.0,0.0,0.0);;   
                   }
                  
                }
                else if(y==0)
                {

                   
                    index1= x + numCellsPerAxis * (numCellsPerAxis-1);
                    newpoints[index]=points[index1]-glm::vec3(0.0,1.0,0.0);;   
                
                  
                }
                else if(y==numCellsPerAxis+1)
                {

                   
                    index1= x + numCellsPerAxis * (0);
                    newpoints[index]=points[index1]-glm::vec3(0.0,-1.0,0.0);;   
                
                  
                }
                
                else
                {
                
                    newpoints[index]=points[index1];
                }
                

            }
            

        }


    }




// float worley_distance(glm::vec3 point, glm::vec3 worley_point, float p_max) {
// 	float value = p_max;
	
// 	for (float z = -1.0; z < 1.1; z += 1.0) {
// 		for (float y = -1.0; y < 1.1; y += 1.0) {
// 			for (float x = -1.0; x < 1.1; x += 1.0) {
// 				glm::vec3 delta = worley_point - point + glm::vec3(x,y,z);
// 				float distance = glm::length(delta);
// 				if (distance < value) {
// 					value = distance;
// 				}
// 			}
// 		}
// 	}
	
// 	return value;
// }

// float worley_value(glm::vec3 pos) {
// 	float value = max_distance;
// 	glm::vec3 fpos=pos;
	
// 	if (number_of_points == 0) {
// 		// just some test data
// 		value = worley_distance(fpos, glm::vec3(0.2, 0.5, 0.1), value);
// 		value = worley_distance(fpos, glm::vec3(0.8, 0.3, 0.4), value);
// 		value = worley_distance(fpos, glm::vec3(0.4, 0.7, 0.3), value);
// 		value = worley_distance(fpos, glm::vec3(0.6, 0.4, 0.2), value);
// 		value = worley_distance(fpos, glm::vec3(0.1, 0.9, 0.5), value);
// 		value = worley_distance(fpos, glm::vec3(0.8, 0.2, 0.4), value);
// 	} else {
// 		for (int p = 0; p < number_of_points; p++) {
			
// 			glm::vec3 wp = points[p];
// 			value = worley_distance(fpos, wp, value);
// 		}
		
// 	}
	
	
// 	return value / max_distance;
// }























void neighbour(vector<int>& neig,int x,int y)
{
   int index=x+numCellsPerAxis*y;
   neig[0] =index;
   index=x+1+numCellsPerAxis*y;
   neig[1] =index;
   index=x-1+numCellsPerAxis*y;
   neig[2] =index;
   index=x+numCellsPerAxis*(y+1);
   neig[3] =index;
   index=x+numCellsPerAxis*(y-1);
   neig[4] =index;
   index=x+1+numCellsPerAxis*(y+1);
   neig[5] =index;
   index=x+1+numCellsPerAxis*(y-1);
   neig[6] =index;
   index=x-1+numCellsPerAxis*(y+1);
   neig[7] =index;
   index=x-1+numCellsPerAxis*(y-1);
   neig[8] =index;

}


double dis(glm::vec3 a,glm::vec3 b)
{
    return glm::distance(a,b);

}

void noiseforeachpixel()
{
     int x,y;
     int index,index1;
     vector<int> neigh;
     double mindistance=100.0,curdis,maxdistance=0.0;
     glm::vec3 pixel;
     double pixex,pixey;
     neigh.resize(9);

    for (int  py = 0; py < 480; py++)
    {
        for (int px = 0; px < 640; px++)
        {
            

            x=px/128;
            y=py/96;
            index= x + numCellsPerAxis * (y);
            neighbour(neigh,x,y);
            pixel=glm::vec3 (px*pixelsxw, y*pixelsyw, 0.0) +glm::vec3(pixelsxw/2,pixelsyw/2,0.0);
            for (int i = 0; i < 9; i++)
            {
                curdis=dis(newpoints[neigh[i]],pixel);
                if(curdis<mindistance)
                {
                    mindistance=curdis;
                }
                if(curdis>maxdistance)
                {
                    maxdistance=curdis;
                }
            }

            minidistance[px+py*640] = mindistance;

        }
        mindistance=100.0;
        
    }
    for (int  i = 0; i < 640*480; i++)
    {
        minidistance[i]=minidistance[i]/maxdistance;
    }
    colors.resize(640*480);
    for (int  i = 0; i < 640*480; i++)
    {
        colors[i]=(float)minidistance[i]*glm::vec3(1.0,1.0,1.0);
    }
    
    
}

int main(int argc,const char*argv[])
{
    srand(time(0));
    // cout<<"suresh";
    // cout<<glm::distance(glm::vec3(0.0,0.0,0.0),glm::vec3(0.5,0.0,0.0));

    CreateWorleyPointsBuffer();

    return 0;
}