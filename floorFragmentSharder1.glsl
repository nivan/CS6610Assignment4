uniform sampler2D floorNormalMap;
uniform sampler2D sandTexture;


void main(){

  // Normalizes light vector with normalization cube map
  //vec3 lightTex = normalize(lightDirection);

  //vec3 light = expand(lightTex);

  // Sample and expand the normal map texture

  //vec3 normalTex = texture2D(floorNormalMap, TexCoords).xyz;

  //float3 normal = expand(normalTex);

  // Compute diffuse lighting

  //vec4 decalCol = texture2D( sandTexture, uvCoord );
  
  gl_FragColor = vec4(1.0,0.0,0.0,1.0);//dot(normal, light)*decalCol;
}