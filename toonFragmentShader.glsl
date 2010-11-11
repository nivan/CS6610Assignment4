varying vec3 N;
varying vec3 v;

void main(){	
   	
	float diffuseLight = max(dot(N, v), 0);
	   	
    vec4 silhouetteColor = vec4(1.0,1.0,1.0,1.0);
    if(diffuseLight <= 0.3){
    	silhouetteColor = vec4(0.0,0.0,0.0,1.0);
    }
    
   	   	   	   	
	vec4 tempColor = vec4(1.0,0.0,0.0,1.0)*silhouetteColor;
	
	
	gl_FragColor = tempColor;
}