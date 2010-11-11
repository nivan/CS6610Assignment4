varying vec3 lightDirection;
varying vec2 TexCoords;
varying float LightDistance; 
varying vec3 EyeDir;
varying vec3 HalfVector;



attribute vec3 Tangent; 

vec3 TransformToTangentSpace( in vec3 t , in vec3 b , in vec3 n , in vec3 vector ){
	vec3 v;
	vec3 temp = normalize( vector );
	v.x = dot ( vector , t);
	v.y = dot ( vector , b);
	v.z = dot ( vector , n);
	return normalize(v);
}

void main(){

	//gl_Position = ftransform(); 

	vec3 lPos;		// Light position ( eye space )
	vec3 halfVec;	// Light half vector
	vec3 vertPos;	// Vertex position ( eye space )
	
	vec3 eyePosition  = vec3( 0.0 , 0.0 , 0.0  );
	vec3 eyeDirection = vec3( 0.0 , 0.0 , -1.0 );
	
	//Put the vertex in the position passed
	gl_Position = ftransform();
	
	vec3 n = normalize ( gl_NormalMatrix * gl_Normal);
	vec3 t = normalize ( gl_NormalMatrix * Tangent );
	vec3 b = cross (n, t); 

	vertPos = vec3( gl_ModelViewMatrix * gl_Vertex );	

	lPos 		      = gl_LightSource[0].position.xyz - vertPos;
	//lPos            = normalize( lPos );	
	LightDistance     = length( lPos );
	lightDirection 	  = TransformToTangentSpace( t , b, n , lPos );

	halfVec 	= gl_LightSource[0].halfVector.xyz;
	HalfVector 	= TransformToTangentSpace( t , b, n , halfVec );
	
	EyeDir = TransformToTangentSpace( t , b, n , eyeDirection );

	TexCoords = gl_MultiTexCoord0.xy;
	
}