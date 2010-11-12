uniform sampler2D floorNormalMap;
uniform sampler2D sandTexture;

varying vec3 lightDirection; 
varying vec2 TexCoords; 
varying vec3 normal;
    
vec3 UnpackNormal( in vec4 packedNormal ){
	return ( vec3( packedNormal ) - 0.5) * 2.0;
}
   
void main(){
	vec3 	lDir;		// Normalized light direction in tangent space
	vec3 	bumpNorm;	// normal to the pixel ( read from the normal map )
	vec4 	decalCol;	// decal color ( read from color map )
	vec4 	diffuse;	// Light diffuse coefficent
		 
		 
	lDir = normalize ( lightDirection );		
	
	bumpNorm = UnpackNormal( texture2D( floorNormalMap, TexCoords ) );
	bumpNorm = normalize(bumpNorm);
	decalCol = texture2D( sandTexture, TexCoords );
	
	// compute diffuse lighting
	float lamberFactor= max (dot (lDir, bumpNorm), 0.0) ;
	vec4 diffuseMaterial = vec4(0.0);
	vec4 diffuseLight  = vec4(0.0);

	if (lamberFactor > 0.0)
	{
	    diffuseMaterial = texture2D (sandTexture, TexCoords);
	    diffuseLight  = gl_LightSource[0].diffuse;
	   
	    diffuseLight = 
		diffuseMaterial * diffuseLight * lamberFactor ;
	}

	gl_FragColor = diffuseLight;
}
