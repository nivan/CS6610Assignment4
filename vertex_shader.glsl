uniform int swim;
uniform float phase;

uniform int isFishVertex;
uniform vec3 eyePosition;

varying vec3 N;
varying vec3 v;
//varying vec3 lightDirection;
//varying vec3 eyeDirection;
//varying vec3 halfAngle;	

void main()
{
    
    vec4 temp = gl_Vertex;//gl_ModelViewMatrix * gl_Vertex;
    vec3 tempNormal = gl_Normal;    
	vec3 tangent;
	tangent.x = -temp.y;
	tangent.y = temp.x;
	tangent.z = 0.0;
	        	    
	vec3 binormal;
	binormal.x = 0.0;
	binormal.y = 0.0;
	binormal.z = -1.0;
    	
	if(isFishVertex == 1){
		//gl_FrontColor = vec4(1.0,0.0,0.0,1.0);
		if(temp.z == 0){
				temp.x = (2*sin(3.14159265*sqrt(temp.z)/3.0))*temp.x;
				temp.y = (2*sin(3.14159265*sqrt(temp.z)/3.0))*temp.y;
								
				tempNormal  = vec3(0.0,0.0,-1.0);
				tangent     = vec3(-1.0,0.0,0.0);
				binormal    = vec3(0.0,1.0,0.0);
		}
		else if(0 < temp.z && temp.z <= 8.7){
		
			temp.x = (2*sin(3.14159265*sqrt(temp.z)/3.0))*temp.x;
			temp.y = (2*sin(3.14159265*sqrt(temp.z)/3.0))*temp.y;
			
			tempNormal  = vec3(0.0,0.0,-1.0);
			
			vec3 line1Jacob = vec3(
			2*sin(3.14159265*sqrt(temp.z)/3.0),0.0, 
			3.14159265*temp.x*cos(3.14159265*sqrt(temp.z)/3.0)/3*sqrt(temp.z));
			
			vec3 line2Jacob = vec3(0.0,
			2*sin(3.14159265*sqrt(temp.z)/3.0), 
			3.14159265*temp.y*cos(3.14159265*sqrt(temp.z)/3.0)/3*sqrt(temp.z));
			
			vec3 tempTangent = tangent;
			
			tangent.x     = dot(line1Jacob,tempTangent);
			tangent.y     = dot(line2Jacob,tempTangent);
			tangent = normalize(tangent);
			
			vec3 tempBinormal = binormal;
			
			binormal.x     = dot(line1Jacob,tempBinormal);
			binormal.y     = dot(line2Jacob,tempBinormal);
			binormal = normalize(binormal);
						
			tempNormal = normalize(cross(binormal,tangent));										
		}
		else if(temp.z > 8.7 && temp.z <=9){
			temp.x = (0.05)*temp.x;
			temp.y = (0.05)*temp.y;
			
			vec3 line1Jacob = vec3(0.05,0.0,0.0);			
			vec3 line2Jacob = vec3(0.0,0.05,0.0);
			
			vec3 tempTangent = tangent;
			
			tangent.x     = dot(line1Jacob,tempTangent);
			tangent.y     = dot(line2Jacob,tempTangent);
			tangent = normalize(tangent);
			
			vec3 tempBinormal = binormal;
			
			binormal.x     = dot(line1Jacob,tempBinormal);
			binormal.y     = dot(line2Jacob,tempBinormal);
			binormal = normalize(binormal);
						
			tempNormal = normalize(cross(binormal,tangent));						
		}
		else if(temp.z > 9.0 && temp.z < 10.0){
			temp.x = (0.05)*temp.x;
			temp.y = (1-(temp.z - 10.0)*(temp.z - 10.0))*temp.y;
			
			
			vec3 line1Jacob = vec3(0.05,0.0,0.0);			
			vec3 line2Jacob = vec3(0.0,
				-(temp.z-10.0)*(temp.z-10.0),
				-2.0*temp.y*(temp.z-10.0));
			
			vec3 tempTangent = tangent;
			
			tangent.x     = dot(line1Jacob,tempTangent);
			tangent.y     = dot(line2Jacob,tempTangent);
			tangent = normalize(tangent);
			
			vec3 tempBinormal = binormal;
			
			binormal.x     = dot(line1Jacob,tempBinormal);
			binormal.y     = dot(line2Jacob,tempBinormal);
			binormal = normalize(binormal);
						
			tempNormal = normalize(cross(binormal,tangent));						
		}
		else if(temp.z == 10){
			temp.x = 0;
			tempNormal.x = 0.0;
			tempNormal.y = 0.0;
			tempNormal.z = 1.0;
		}
		
		    if(swim == 1){		    	
       			temp.x += (0.5)*sin(temp.z - 1.5*phase)* (0.04*temp.z+0.01);
    		}
				
		temp.z = temp.z - 5;
	}
		              
    gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
    //Phong Shading
	v = vec3(gl_ModelViewMatrix * temp);       
   	N = normalize(gl_NormalMatrix * tempNormal);
 	gl_FrontColor = gl_Color;	    	      
	gl_Position =  gl_ProjectionMatrix * gl_ModelViewMatrix* temp;
	
	
//	lightDirection = gl_LightSource[0].position.xyz - gl_Vertex.xyz;

  	// Add the computation of a per-vertex half-angle vector

//  	eyeDirection = eyePosition - gl_Vertex.xyz;

//  	halfAngle = normalize(normalize(lightDirection) + normalize(eyeDirection));	
}