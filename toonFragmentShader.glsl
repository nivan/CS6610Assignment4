varying vec3 N;
varying vec3 v;
varying float diffuseTerm;
varying float specularLight;

void main(){	
   	
   	vec3 NN = normalize(N);
   	vec3 vv = normalize(v);
   	
	float isSilhouette = max(dot(-NN, vv), 0);
	   	
    vec4 silhouetteColor = vec4(1.0,1.0,1.0,1.0);
    if(isSilhouette <= 0.15){
    	silhouetteColor = vec4(0.0,0.0,0.0,1.0);
    }
   
   vec4 diffuseColor;
    if(diffuseTerm <= 0.7){
    	diffuseColor = vec4(0.5,0.0,0.0,1.0);
    }
    else{
    	diffuseColor = vec4(1.0,0.0,0.0,1.0);
    }
   	   	   	   	
	vec4 specularLighting = vec4(0.0,0.0,0.0,1.0);
	if(specularLight >= 0.8){
		specularLighting = vec4(1.0,1.0,1.0,1.0);
	}	   	   	   	   	
   	   	   	   	
	vec4 tempColor = (diffuseColor + specularLighting)*silhouetteColor;
	
	gl_FragColor = tempColor;
}