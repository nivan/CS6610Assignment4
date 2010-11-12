varying vec2 TexCoords;
varying vec3 lightDirection;
varying vec3 normal;

vec3 TransformToTangentSpace( in vec3 t , in vec3 b , in vec3 n , in vec3 vector ){
	vec3 v;
	vec3 temp = normalize( vector );
	v.x = dot ( vector , t);
	v.y = dot ( vector , b);
	v.z = dot ( vector , n);
	return normalize(v);
}

void main(){

	
	vec3 n = vec3(0.0,1.0,0.0);
	vec3 t = vec3(0.0,0.0,1.0);
	vec3 b = vec3(1.0,0.0,0.0); 

	normal = n;
	TexCoords = gl_MultiTexCoord0.xy;
	
	vec3 tempLightDirection = 
	    gl_LightSource[0].position.xyz - gl_Vertex.xyz;	     
	tempLightDirection = 
	    TransformToTangentSpace(t, b,
				    gl_Normal,tempLightDirection);
	lightDirection = tempLightDirection;

	//Put the vertex in the position passed
	gl_Position = ftransform();
 
}
