[[FX]]

// Samplers
sampler2D depthBuf = sampler_state
{
	Address = Clamp;
};

sampler2D gbuf0 = sampler_state
{
	Address = Clamp;
};

sampler2D gbuf1 = sampler_state
{
	Address = Clamp;
};

sampler2D gbuf2 = sampler_state
{
	Address = Clamp;
};

sampler2D gbuf3 = sampler_state
{
	Address = Clamp;
};
sampler2D randomBuf = sampler_state
{
  Address = Wrap;
  Filter = Trilinear;
  Texture = "textures/ssaoRandom.jpg";
};

context SSAO
{
  VertexShader = compile GLSL VS_FSQUAD;
  PixelShader = compile GLSL FS_SSAO;
}

[[VS_FSQUAD]]

uniform mat4 projMat;
attribute vec3 vertPos;
varying vec2 texCoords;
				
void main( void )
{
	texCoords = vertPos.xy; 
	gl_Position = projMat * vec4( vertPos, 1 );
}

[[FS_SSAO]]
//Thanks http://www.gamedev.net/page/resources/_/technical/graphics-programming-and-theory/a-simple-and-practical-approach-to-ssao-r2753
#include "shaders/utilityLib/fragDeferredRead.glsl"

uniform sampler2D randomBuf;
uniform vec2 frameBufSize;
uniform sampler2D depthBuf;
uniform sampler2D normalsBuf;

varying vec2 texCoords;
vec2 getRandom(vec2 coord)
{
  float randBufSize = 64.0;
  return normalize(texture2D(randomBuf, frameBufSize * coord / randBufSize).xy * 2.0 - 1.0);
}

float doAmbientOcclusion(vec2 tcoord, vec2 uv, vec3 p, vec3 cnorm)
{
  float g_scale = 5.5;
  float g_bias = .02;
  float g_intensity = 5.5;
  vec3 diff = getPos(tcoord+uv)-p;
  vec3 v = normalize(diff);
  float d = length(diff)*g_scale;
  
  return max(0.0, dot(cnorm, v)-g_bias)*(1.0/(1.0+d))*g_intensity;
}

void main( void )
{
  vec2 vec[8];
  vec[0] = vec2(1,0);
  vec[1] = vec2(-1,0);
  vec[2] = vec2(0,1);
  vec[3] = vec2(0,-1);
  vec[4] = vec2(.707, .707);
  vec[5] = vec2(-.707, .707);
  vec[6] = vec2(.707, -.707);
  vec[7] = vec2(-.707, -.707);

  vec3 p = getPos(texCoords);
  vec3 n = getNormal(texCoords);
  vec2 rand = getRandom(texCoords);

  float ao = 0.0;
  float rad = 0.25/p.z;

  int iterations = 4;
  for (int j=0; j < iterations; ++j) {
    vec2 coord1 = reflect(vec[j], rand)*rad;
    vec2 coord2 = vec2(coord1.x*.707 - coord1.y*0.707,
                       coord1.x*.707 + coord1.y*.707);
    ao += doAmbientOcclusion(texCoords, coord1*.25, p, n);
    ao += doAmbientOcclusion(texCoords, coord1*.5, p, n);
    ao += doAmbientOcclusion(texCoords, coord1*.75, p, n);
    ao += doAmbientOcclusion(texCoords, coord1*1., p, n);
  }
  ao /= iterations*4.0;
  ao = 1-clamp(ao, 0, 1);
  gl_FragColor = vec4(ao, ao, ao, 1);//texture2D(depthBuf, texCoords);
}
