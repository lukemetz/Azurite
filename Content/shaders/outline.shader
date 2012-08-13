[[FX]]

sampler2D buf0 = sampler_state
{
  Address = Clamp;
};

float thickness = 1.0;
float4 color = {0, 0, 0, 1};

context OUTLINE
{
  VertexShader = compile GLSL VS_FSQUAD;
  PixelShader = compile GLSL FS_OUTLINE;

  ZWriteEnable = false;
}


[[VS_FSQUAD]]
// =================================================================================================

uniform mat4 projMat;
attribute vec3 vertPos;
varying vec2 texCoords;
				
void main( void )
{
	texCoords = vertPos.xy; 
	gl_Position = projMat * vec4( vertPos, 1 );
}

[[FS_OUTLINE]]
uniform sampler2D buf0;
uniform vec4 color;
uniform float thickness;
varying vec2 texCoords;

void main ( void )
{
  vec4 color = {1,1,1,.3};
  gl_FragColor = color;
}

