uniform sampler2D Texture_Normal;
uniform sampler2D sandTexture;

varying vec3 lightDirection; 
varying vec2 TexCoords;
  
varying vec3 HalfVector;
varying vec3 EyeDir;
varying float LightDistance;  
  
vec4 ComputeLightDiffuse( in int lightIdx , in float d)  
{
	float attenuation = 1.0 / ( gl_LightSource[ lightIdx ].constantAttenuation +
								gl_LightSource[ lightIdx ].linearAttenuation * d +
								gl_LightSource[ lightIdx ].quadraticAttenuation * d * d );
	vec4 diffuse = gl_LightSource[ lightIdx ].diffuse * attenuation;
	
	return diffuse;
}    
  
vec3 UnpackNormal( in vec4 packedNormal ){
	return ( vec3( packedNormal ) - 0.5) * 2.0;
}
   
  
void main() 
{
	vec3 	lDir;		// Normalized light direction in tangent space
	vec3 	eDir;		// Normalized eye direction in tangent space
	vec3 	hVec;		// Light half vector
	vec2 	uvCoord;	// Texture coords
	float 	height;		// height offset for parallax mapping
	vec3 	bumpNorm;	// normal to the pixel ( read from the normal map )
	vec4 	decalCol;	// decal color ( read from color map )
	vec4 	diffuse;	// Light diffuse coefficent
	float 	nDotL;		// Dot product between the light direction in tangent space and the pixel normal
	float 	spec;		// Specular factor	
	vec3 	finalCol;	// Final fragment color after the computations	 
		 
		 
	lDir = normalize ( lightDirection );		
	eDir = normalize ( EyeDir );		
	hVec = normalize ( HalfVector );
	
	
	uvCoord = TexCoords;
		
	bumpNorm = UnpackNormal( texture2D( Texture_Normal, uvCoord ) );
	decalCol = texture2D( sandTexture, uvCoord );
	
	// compute distance between the surface and the light position
	diffuse = ComputeLightDiffuse( 0 , LightDistance ); 
	
	//Find the dot product between the light direction and the normal
	nDotL = max( dot( bumpNorm, lDir ), 0.0 );
	
	finalCol = nDotL * decalCol.rgb;
	finalCol = min( finalCol , 1.0 );
	
	gl_FragColor = vec4( finalCol , decalCol.a ) * diffuse;
}