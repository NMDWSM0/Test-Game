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

uniform vec2 uTexOffset1;
uniform vec2 uTexOffset2;

uniform bool uHasTex;
uniform bool uHasTex2;
uniform bool uUseEdgeCut;

uniform bool uUseMult;
uniform vec4 uMultColor;

uniform sampler2D uTexture;
uniform sampler2D uTexture2;

/**************************************************/
vec4 shade(void)
{
    //vec3 color = pow(vKd, vec3(2.2f));
	vec3 color = pow(vKd, vec3(2.2f));
	highp float alpha = 1.0f;
	if (uHasTex)
	{
		vec4 ori_color = texture2D(uTexture, vtexCoords + uTexOffset1);
		alpha = ori_color.a;
		if (uHasTex2)
		{
			vec4 ori_color2 = texture2D(uTexture2, vtexCoords + uTexOffset2);
			ori_color += ori_color2;
			alpha += ori_color2.a;
			alpha *= 0.5f;
		}
		color = pow(ori_color.rgb, vec3(2.2f));  //gammaÐ£Õý
	}

	vec3 finalcolor = color;
	if (uUseMult)
	{
		finalcolor = color * (1.0f - uMultColor.a) + uMultColor.rgb * uMultColor.a;
	}

    return vec4(pow(finalcolor, vec3(1.0f/2.2f)), alpha);
}

void main(void)
{
    vec4 shadecolor = shade();

	if (!uUseEdgeCut)
	{
		gl_FragColor = shadecolor;
		return;
	}

	float x = 2.0f * vtexCoords.x - 1.0f;
	float y = 2.0f * vtexCoords.y - 1.0f;

	float a_x = 1.0f - abs(pow(x, 2));
	float a_y = 1.0f - abs(pow(y, 2));

	gl_FragColor = vec4(shadecolor.rgb, shadecolor.a * min(a_x, a_y));
}
