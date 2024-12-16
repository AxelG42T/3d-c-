#version 330 core
out vec4 FragColor;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D tex;
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform float ambientStrengh;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main(){
	
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);

	float specularStrengh = 1.0f;
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular =specularStrengh * spec * lightColor;
	
	vec3 ambient = ambientStrengh * lightColor;

	vec2 texSize = textureSize(tex, 0);
	if(texSize.x != 1 && texSize.y != 1){
		vec4 textureColor = texture(tex, TexCoord);
		FragColor = vec4(objectColor * textureColor.rgb * (ambient + diffuse + specular), 1.0f);
	} else{
		FragColor = vec4(objectColor * (ambient + diffuse + specular), 1.0f);
	}
}