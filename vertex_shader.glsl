uniform int swim;
uniform float phase;

uniform int isFishVertex;

varying vec3 N;
varying vec3 v;


attribute vec3 tangent;

void main()
{
    
    vec4 temp = gl_Vertex;//gl_ModelViewMatrix * gl_Vertex;
    vec3 tempNormal = gl_Normal;    
    gl_FrontColor = gl_Color;	    	
    	
	if(isFishVertex == 1){
		//gl_FrontColor = vec4(1.0,0.0,0.0,1.0);
		if(temp.z <= 8.7){
		
			temp.x = (2*sin(3.14159265*sqrt(temp.z)/3.0))*temp.x;
			temp.y = (2*sin(3.14159265*sqrt(temp.z)/3.0))*temp.y;		
		}
		else if(temp.z > 8.7 && temp.z <=9){
			temp.x = (0.05)*temp.x;
			temp.y = (0.05)*temp.y;
		}
		else if(temp.z > 9 && temp.z < 10){
			temp.x = (0.05)*temp.x;
			temp.y = (1-(temp.z - 10)*(temp.z - 10))*temp.y;
			tempNormal.x = 0.05*tempNormal.x;
			tempNormal.y = (1-(temp.z - 10)*(temp.z - 10))*tempNormal.y;			
		}
		else if(temp.z == 10){
			temp.x = 0;
			tempNormal.x = tempNormal.x;
			tempNormal.y = 0.0;
			tempNormal.z = 0.0;
		}
		
		    if(swim == 1){		    	
       			temp.x += (0.5)*sin(temp.z - phase)* (0.04*temp.z+0.01);
    		}
				
		temp.z = temp.z - 5;
	}
	else if(isFishVertex == 0){
	//floor
	//tempNormal = tempNormal + gl_TextureMatrix[0] * gl_MultiTexCoord0;
		gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
	}	              
    
    //Phong Shading
	v = vec3(gl_ModelViewMatrix * temp);       
   	N = normalize(gl_NormalMatrix * tempNormal);
    
    
    
    
    
    
    
    
    
    
	gl_Position =  gl_ProjectionMatrix * gl_ModelViewMatrix* temp;	
}