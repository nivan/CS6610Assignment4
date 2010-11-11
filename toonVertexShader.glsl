varying vec3 N;
varying vec3 v;

void main(){

	// transform the vertext normal the same the object is transformed		      
   	N = normalize(gl_NormalMatrix * gl_Normal);   	
    v = vec3(gl_ModelViewMatrix * gl_Vertex);
    
	gl_Position =  gl_ProjectionMatrix * gl_ModelViewMatrix*gl_Vertex;	
}