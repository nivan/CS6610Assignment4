uniform vec3 cameraPosition;

uniform int swimT;
uniform float phase;
uniform float xTransToon;
uniform float zTransToon;

varying vec3 N;
varying vec3 v;
varying float diffuseTerm;
varying float specularLight;

void main(){

   gl_FrontColor = gl_Color;

     //already normalized since the cylinder has radius one
     vec3 tangent;
     tangent.x = -gl_Vertex.y; 
     tangent.y = gl_Vertex.x; 
     tangent.z = 0.0; 
     
     vec3 binormal = vec3(0.0,0.0,1.0);
    
     vec3 line1Jacob = vec3(1.0,0.0,0.0);			
     vec3 line2Jacob = vec3(0.0,1.0,0.0);			   
     
     if(gl_Vertex.z == 0.0){
	 gl_Vertex.x = 0.05*gl_Vertex.x;
	 gl_Vertex.y = 0.05*gl_Vertex.y;


	 line1Jacob = vec3(0.05,0.0,0.0);			
	 line2Jacob = vec3(0.0,0.05,0.0);
			
        //set tangent space
	 vec3 tempTangent = tangent;
			
	 tangent.x     = dot(line1Jacob,tempTangent);
	 tangent.y     = dot(line2Jacob,tempTangent);
	 tangent = normalize(tangent);
			
	 vec3 tempBinormal = binormal;
			
	 binormal.x     = dot(line1Jacob,tempBinormal);
	 binormal.y     = dot(line2Jacob,tempBinormal);
	 binormal = normalize(binormal);
						
	 gl_Normal = normalize(cross(tangent,binormal));
	 	
     }
     else if(0 < gl_Vertex.z && gl_Vertex.z <= 8.7){
	 gl_Vertex.x = 
	     (2*sin(3.14159265*sqrt(gl_Vertex.z)/3.0))*gl_Vertex.x;
	 gl_Vertex.y = 
	     (2*sin(3.14159265*sqrt(gl_Vertex.z)/3.0))*gl_Vertex.y;

	 //set tangent space
	 vec3 line1Jacob = vec3(
	     2*sin(3.14159265*sqrt(gl_Vertex.z)/3.0),0.0, 
	     3.14159265*gl_Vertex.x*cos(3.14159265*sqrt(gl_Vertex.z)/3.0)/3*sqrt(gl_Vertex.z));
			
	 vec3 line2Jacob = vec3(0.0,
	     2*sin(3.14159265*sqrt(gl_Vertex.z)/3.0), 
	     3.14159265*gl_Vertex.y*cos(3.14159265*sqrt(gl_Vertex.z)/3.0)/3*sqrt(gl_Vertex.z));
	
	 vec3 tempTangent = tangent;
			
	 tangent.x     = dot(line1Jacob,tempTangent);
	 tangent.y     = dot(line2Jacob,tempTangent);
	 tangent = normalize(tangent);
			
	 vec3 tempBinormal = binormal;
			
	 binormal.x     = dot(line1Jacob,tempBinormal);
	 binormal.y     = dot(line2Jacob,tempBinormal);
	 binormal = normalize(binormal);
						
	 gl_Normal = normalize(cross(tangent,binormal));
     }
     else if(gl_Vertex.z > 8.7 && gl_Vertex.z <=9){
	 gl_Vertex.x = (0.05)*gl_Vertex.x;
	 gl_Vertex.y = (0.05)*gl_Vertex.y;
	 
	 line1Jacob = vec3(0.05,0.0,0.0);			
	 line2Jacob = vec3(0.0,0.05,0.0);
			
        //set tangent space
	 vec3 tempTangent = tangent;
			
	 tangent.x     = dot(line1Jacob,tempTangent);
	 tangent.y     = dot(line2Jacob,tempTangent);
	 tangent = normalize(tangent);
			
	 vec3 tempBinormal = binormal;
			
	 binormal.x     = dot(line1Jacob,tempBinormal);
	 binormal.y     = dot(line2Jacob,tempBinormal);
	 binormal = normalize(binormal);
						
	 gl_Normal = normalize(cross(tangent,binormal));	
     }
     else if(gl_Vertex.z > 9.0 && gl_Vertex.z <=10.0){
	 gl_Vertex.x = (0.05)*gl_Vertex.x;
	 gl_Vertex.y = 
	  (1-(gl_Vertex.z - 10.0)*(gl_Vertex.z - 10.0))*gl_Vertex.y;

	 vec3 line1Jacob = vec3(0.05,0.0,0.0);			
	 vec3 line2Jacob = vec3(0.0,
			  -(gl_Vertex.z-10.0)*(gl_Vertex.z-10.0),
			  -2.0*gl_Vertex.y*(gl_Vertex.z-10.0));
			
	 //set tangent space
	 vec3 tempTangent = tangent;
			
	 tangent.x     = dot(line1Jacob,tempTangent);
	 tangent.y     = dot(line2Jacob,tempTangent);
	 tangent = normalize(tangent);
			
	 vec3 tempBinormal = binormal;
			
	 binormal.x     = dot(line1Jacob,tempBinormal);
	 binormal.y     = dot(line2Jacob,tempBinormal);
	 binormal = normalize(binormal);
						
	 gl_Normal = normalize(cross(tangent,binormal));
	 
     }
     else if(gl_Vertex.z == 10.0){
	 gl_Vertex.x = 0.05*gl_Vertex.x;

	 line1Jacob = vec3(0.05,0.0,0.0);			
	 //set tangent space
	 vec3 tempTangent = tangent;
			
	 tangent.x     = dot(line1Jacob,tempTangent);
	 tangent = normalize(tangent);
			
	 vec3 tempBinormal = binormal;
			
	 binormal.x     = dot(line1Jacob,tempBinormal);
	 binormal = normalize(binormal);
						
	 gl_Normal = normalize(cross(tangent,binormal));
     }

     if(swimT == 1){		    	
	 gl_Vertex.x += (0.5)*sin(gl_Vertex.z - 1.5*phase)*
	     (0.04*gl_Vertex.z+0.01);

	 //set tangent space;
	 float fxDelz = 0.5*(
	     0.04*sin(gl_Vertex.z - 1.5*phase)
	     +
	     (0.04*gl_Vertex.z+0.01)*cos(gl_Vertex.z - 1.5*phase)
	     );

	 line1Jacob = vec3(1.0,0.0,fxDelz);			
	 //set tangent space
	 vec3 tempTangent = tangent;
			
	 tangent.x     = dot(line1Jacob,tempTangent);
	 tangent = normalize(tangent);
			
	 vec3 tempBinormal = binormal;
			
	 binormal.x     = dot(line1Jacob,tempBinormal);
	 binormal = normalize(binormal);
						
	 gl_Normal = normalize(cross(tangent,binormal));
     }

     gl_Vertex.z = gl_Vertex.z - 5;

     gl_Vertex.x = gl_Vertex.x + xTransToon;
     gl_Vertex.z = gl_Vertex.z + zTransToon;
     vec3 verPosition = gl_Vertex.xyz;

     gl_Position = ftransform();
	
     N = gl_Normal;

     vec3 tempLightDirection = 
	 normalize(gl_LightSource[0].position.xyz - verPosition);

     diffuseTerm = max(dot(gl_Normal, tempLightDirection), 0);

     // Calculate specular lighting 			   
     vec3 V = normalize(cameraPosition - verPosition);  
     v = V;
     vec3 H = normalize(tempLightDirection + V); 
     specularLight = pow(max(dot(gl_Normal, H), 0), 10.0);

}
