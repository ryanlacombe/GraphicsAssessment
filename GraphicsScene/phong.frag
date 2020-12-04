//Classic Phong fragment shader
#version 410

in vec4 vPosition;
in vec3 vNormal;
in vec4 vColor;
in vec2 vTexCoord;

uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;
uniform float specularPower;

uniform vec3 Ia;
uniform vec3 Id;
uniform vec3 Is;
uniform vec3 LightDirection;

uniform vec3 CameraPosition;

out vec4 FragColor;

void main() {
	//Ensure normal and light direction are normalized
	vec3 N = normalize(vNormal);
	vec3 L = normalize(LightDirection);

	//Calculate lambert term (negate light direction)
	float lambertTerm = max(0, min(1, dot(N, -L)));

	//Calculate view vector and reflection vector
	vec3 V = normalize(CameraPosition - vPosition.xyz);
	vec3 R = reflect(L, N);

	//Calculate specular term
	float specularTerm = pow(max(0, dot(R, V)), specularPower);

	//Output color
	vec3 ambient = (Ka + vColor.rgb) * Ia;
	vec3 diffuse = (Kd + vColor.rgb) * Id * lambertTerm;
	vec3 specular = (Ks + vColor.rgb) * Is * specularTerm;
	FragColor = vec4(ambient + diffuse + specular, 1);
}