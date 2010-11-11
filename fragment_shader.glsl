uniform sampler2D fishNormalMap;
uniform sampler2D fishTexture;
uniform int isFishVertex;
uniform int polygonMode;

varying vec3 N;
varying vec3 v;
//varying vec3 lightDirection;
//varying vec3 eyeDirection;
//varying vec3 halfAngle;

vec3  expand(vec3 v){
  return (v - vec3(0.5)) * 2; // Expand a range-compressed vector
}

void main()
{	
	vec3 tempNorm = N;	
	vec4 fishTexColor = texture2D(fishTexture,gl_TexCoord[0].st);	
	
	if(/*isFishVertex == 1 && */polygonMode == 0){		
	
    	vec3 L = normalize(gl_LightSource[0].position.xyz - v);   
    	vec3 E = normalize(-v); // we are in Eye Coordinates, so EyePos is (0,0,0)  
    	vec3 R = normalize(-reflect(L,N));  

		//calculate Ambient Term:  
    	vec4 Iamb = gl_FrontLightProduct[0].ambient;    


    	//calculate Diffuse Term:  
    	vec4 Idiff = gl_FrontLightProduct[0].diffuse * max(dot(N,L), 0.0);
    	Idiff = clamp(Idiff, 0.0, 1.0);     
   
    	// calculate Specular Term:
    	vec4 Ispec = gl_FrontLightProduct[0].specular 
                * pow(max(dot(R,E),0.0),0.4*gl_FrontMaterial.shininess);
    	Ispec = clamp(Ispec, 0.0, 1.0); 
    	// write Total Color:  
    	vec4 tempColor = gl_FrontLightModelProduct.sceneColor + Idiff + Ispec + Iamb;    

    	gl_FragColor =	 tempColor*gl_Color*fishTexColor;
 	}
 	else{
 		gl_FragColor =	 gl_Color;
 	}
}