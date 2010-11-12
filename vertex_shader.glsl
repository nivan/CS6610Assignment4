uniform int swim;
uniform float phase;
uniform int isFishVertex;
uniform vec3 eyePosition;
uniform float xTrans;
uniform float zTrans;

void main()
{
     gl_FrontColor = gl_Color;

     //already normalized since the cylinder has radius one
     
     if(gl_Vertex.z == 0.0){
	 gl_Vertex.x = 0.05*gl_Vertex.x;
	 gl_Vertex.y = 0.05*gl_Vertex.y;	 	
     }
     else if(0 < gl_Vertex.z && gl_Vertex.z <= 8.7){
	 gl_Vertex.x = 
	     (2*sin(3.14159265*sqrt(gl_Vertex.z)/3.0))*gl_Vertex.x;
	 gl_Vertex.y = 
	     (2*sin(3.14159265*sqrt(gl_Vertex.z)/3.0))*gl_Vertex.y;
     }
     else if(gl_Vertex.z > 8.7 && gl_Vertex.z <=9){
	 gl_Vertex.x = (0.05)*gl_Vertex.x;
	 gl_Vertex.y = (0.05)*gl_Vertex.y;
     }
     else if(gl_Vertex.z > 9.0 && gl_Vertex.z <10.0){
	 gl_Vertex.x = (0.05)*gl_Vertex.x;
	 gl_Vertex.y = 
	  (1-(gl_Vertex.z - 10.0)*(gl_Vertex.z - 10.0))*gl_Vertex.y;
     }
     else if(gl_Vertex.z == 10.0){
	 gl_Vertex.x = 0.05*gl_Vertex.x;
     }

     if(swim == 1){		    	
	 gl_Vertex.x += (0.5)*sin(gl_Vertex.z - 1.5*phase)*
	     (0.04*gl_Vertex.z+0.01);
     }

     gl_Vertex.z = gl_Vertex.z - 5;

     gl_Vertex.x = gl_Vertex.x + xTrans;
     gl_Vertex.z = gl_Vertex.z + zTrans;
     vec3 verPosition = gl_Vertex.xyz;

     gl_Position = ftransform();
	
}