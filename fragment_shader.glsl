varying vec3 N;
varying vec3 v;
uniform int isFishVertex;
uniform int polygonMode;

void main()
{	
	vec3 tempNorm = N;

	//if(isFishVertex == 0){
		//tempNorm = texture2D(floorNormalMap,gl_TexCoord[0].st).rgb;
		//tempNorm.x = 2*tempNorm.x - 1;
		//tempNorm.y = 2*tempNorm.y - 1;
		//tempNorm.z = 2*tempNorm.z - 1;
		//tempNorm.yz = tempNorm.zy;		
	//}

	if(/*isFishVertex == 1 && */polygonMode == 0){
    	vec3 L = normalize(gl_LightSource[0].position.xyz - v);   
    	vec3 E = normalize(-v); // we are in Eye Coordinates, so EyePos is (0,0,0)  
    	vec3 R = normalize(-reflect(L,N));  

    	//calculate Diffuse Term:  
    	vec4 Idiff = gl_FrontLightProduct[0].diffuse * max(dot(N,L), 0.0);
    	Idiff = clamp(Idiff, 0.0, 1.0);     
   
    	// calculate Specular Term:
    	vec4 Ispec = gl_FrontLightProduct[0].specular 
                * pow(max(dot(R,E),0.0),0.3*gl_FrontMaterial.shininess);
    	Ispec = clamp(Ispec, 0.0, 1.0); 
    	// write Total Color:  
    	vec4 tempColor = gl_FrontLightModelProduct.sceneColor + Idiff + Ispec;    

    	gl_FragColor =	 tempColor*gl_Color;
 	}
 	else{
 		gl_FragColor =	 gl_Color;
 	}
}