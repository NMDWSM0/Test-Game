#version 330 core
precision highp float;

in highp vec3 vFragPos;
in highp vec3 vNormal;
in highp vec2 vtexCoords;
in highp vec3 vKa;
in highp vec3 vKd;
in highp vec3 vKs;
in highp vec3 vMaterial_others;
//r = ns, g = ior, b = dissolve

uniform mat4 uView;
uniform mat4 uProjection;

uniform vec3 uLightPos;
uniform vec3 uLightColor;
uniform float uLightIntensity;
uniform vec3 uCameraPos;
uniform bool uHasTex;

uniform bool uUseMult;
uniform vec4 uMultColor;

uniform sampler2D uTexture;

/**************************************************/
vec4 shade(void)
{
    //vec3 color = pow(vKd, vec3(2.2f));
	vec3 color = pow(vKd, vec3(2.2f));
	highp float alpha = 1.0f;
	if (uHasTex)
	{
		vec4 ori_color = texture2D(uTexture, vtexCoords);
		alpha = ori_color.a;
		color = pow(ori_color.rgb, vec3(2.2f));  //gamma校正
	}

	vec3 finalcolor = color;
	if (uUseMult)
	{
		finalcolor = color * (1.0f - uMultColor.a) + uMultColor.rgb * uMultColor.a;
	}

//	//环境光
//	//vKa
//	vec3 ambient = vKa * color * uLightColor;
//
//	//漫反射
//	vec3 lightDir = normalize(uLightPos - vFragPos);
//	vec3 normal = normalize(vNormal);
//	float diff = max(dot(lightDir, normal), 0.0);
//    float light_atten_coff = uLightIntensity / length(uLightPos - vFragPos);
//    vec3 diffuse =  diff * light_atten_coff * color * uLightColor;
//
//	//高光项
//	vec3 viewDir = normalize(uCameraPos - vFragPos);
//    float spec = 0.0;
//    vec3 halfDir = normalize(viewDir + lightDir);
//	//vMaterial_others.r
//    spec = pow (max(dot(halfDir, normal), 0.0), vMaterial_others.r);
//    vec3 specular = vKs * light_atten_coff * spec * uLightColor;
//
//	return pow((ambient + diffuse + specular), vec3(1.0/2.2));
    return vec4(pow(finalcolor, vec3(1.0f/2.2f)), alpha);
}

void main(void)
{
	gl_FragColor = shade();
}
