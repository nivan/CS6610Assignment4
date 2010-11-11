uniform int swim;
uniform float phase;

varying vec3 N;
varying vec3 v;
varying float diffuseTerm;
varying float specularLight;

void main(){

    vec4 temp = gl_Vertex;
    vec3 tempNormal = gl_Normal;    
    //gl_FrontColor = gl_Color;	    	
	vec3 tangent;
	tangent.x = -temp.y;
	tangent.y = temp.x;
	tangent.z = 0.0;
	        	    
	vec3 binormal;
	binormal.x = 0.0;
	binormal.y = 0.0;
	binormal.z = -1.0;    	
	
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
       			temp.x += (0.5)*sin(temp.z - phase)* (0.04*temp.z+0.01);
    		}
				
		temp.z = temp.z - 5;
	















	//shininess
	float shininess = 0.2;

	// transform the vertext normal the same the object is transformed		      
   	vec3 tempN = (gl_NormalMatrix * gl_Normal);
   	N = tempN;   	
    v = vec3(gl_ModelViewMatrix * gl_Vertex);
 
 	//diffuse term
 	vec3 aux;
 	vec4 ecPos;
 	ecPos = gl_ModelViewMatrix * gl_Vertex;
 	aux = vec3(gl_LightSource[0].position-ecPos);
	vec3 lightDir = normalize(aux);
	diffuseTerm = max(dot(tempN, lightDir), 0);
    
	vec3 V = normalize(vec3(ecPos));
	vec3 H = normalize(lightDir + V);
	specularLight = pow(max(dot(N, H), 0), shininess);
        
	gl_Position =  gl_ProjectionMatrix * gl_ModelViewMatrix*temp;	
}