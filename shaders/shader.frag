#version 450

in vec4 normal;
in vec4 pos;
in vec4 lightPos;
in vec2 tex_coord;
in vec4 color;

// TODO: set these in program
uniform float ambientCoef = 0.5;
uniform float diffuseCoef = 0.3;
uniform float specularCoef = 0.7;
uniform float specularExp = 10.0;

// the various representations of the models surface
uniform vec4 ambientMaterial = vec4(0.0, 1.0, 0.0, 0.0);
uniform vec4 diffuseMaterial = vec4(1.0, 1.0, 0.0, 0.0);
uniform vec4 specularMaterial = vec4(1.0, 0.0, 1.0, 0.0);

// The colors and positions of the two light sources
uniform vec4 lightColor = vec4(1.0);
uniform vec4 ambientLight = vec4(1.0); 

uniform sampler2D tex;

out vec4 fragmentColor;


void main() {
    /*
 	vec4 surfToLight;
 	vec4 viewVec;
 	vec4 reflectionVec;
	
	// calculate vector pointing from the surface to the light source
	surfToLight = normalize(lightPos - pos);
   	surfToLight.w = 0.0; 
	
	// calculate the vector reflecting from the light off the surface
	reflectionVec = normalize(reflect(-surfToLight, normal));
	reflectionVec.w = 0.0;	
	viewVec = normalize(-pos);

	vec4 ambient = ambientLight * ambientCoef * ambientMaterial;

	vec4 diffuse = lightColor * diffuseCoef * diffuseMaterial *
		max(0.0, dot(normal, surfToLight));
   
	vec4 specular = lightColor * specularCoef * specularMaterial *
		pow(max(dot(reflectionVec, viewVec), 0.0), specularExp);
    */

		
    // fragmentColor = texture(tex, tex_coord);// + vec4(0.1, 0.0, 0.0, 0.0);//ambient + diffuse + specular;
    fragmentColor = color;
}

