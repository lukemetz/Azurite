[[FX]]
sampler2D buf0 = sampler_state
{
  Address = Clamp;
  Filter = None;
};

sampler2D buf1 = sampler_state
{
  Address = Clamp;
  Filter = None;
};
float thickness = 1.0;

sampler2D buf2 = sampler_state
{
  Address = Clamp;
  Filter = None;
};

sampler2D buf3 = sampler_state
{
  Address = Clamp;
  Filter = Trilinear;
};

float4 color = {0, 0, 0, 1};

// Uniforms
float hdrExposure = 2.0;       // Exposure (higher values make scene brighter)
float hdrBrightThres = 0.6;    // Brightpass threshold (intensity where blooming begins)
float hdrBrightOffset = 0.06;  // Brightpass offset (smaller values produce stronger blooming)

context OUTLINE
{
  VertexShader = compile GLSL VS_FSQUAD;
  PixelShader = compile GLSL FS_OUTLINE;

  ZWriteEnable = false;
}

context FINALPASS
{
	VertexShader = compile GLSL VS_FSQUAD;
	PixelShader = compile GLSL FS_FINALPASS;
	
	ZWriteEnable = false;
}

context FXAA
{
  VertexShader = compile GLSL VS_FSQUAD;
  PixelShader = compile GLSL FS_FXAA;
}

[[FS_OUTLINE]]
uniform sampler2D depthBuf;
varying vec2 texCoords;
uniform vec2 frameBufSize;

void main( void )
{
  //Using a 3x3 laplacian kernel
  float width = 2.5;
  vec4 color = 8 * texture2D(depthBuf, texCoords);
  color -= texture2D(depthBuf, texCoords+vec2(-width, 0.0)/frameBufSize);
  color -= texture2D(depthBuf, texCoords+vec2(width, 0.0)/frameBufSize);
  color -= texture2D(depthBuf, texCoords+vec2(0.0, -width)/frameBufSize);
  color -= texture2D(depthBuf, texCoords+vec2(0.0, width)/frameBufSize);
  color -= texture2D(depthBuf, texCoords+vec2(-width, -width)/frameBufSize);
  color -= texture2D(depthBuf, texCoords+vec2(-width, width)/frameBufSize);
  color -= texture2D(depthBuf, texCoords+vec2(width, -width)/frameBufSize);
  color -= texture2D(depthBuf, texCoords+vec2(width, width)/frameBufSize);
  
  color = 1-clamp(-1*color*400, 0, 1);
  gl_FragColor = color;
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


[[FS_FINALPASS]]
uniform sampler2D buf0, buf1, buf2, buf3;
uniform vec2 frameBufSize;
uniform float hdrExposure;
varying vec2 texCoords;

void main( void )
{
	vec4 col0 = texture2D( buf0, texCoords );	// HDR color
	vec4 col1 = texture2D( buf1, texCoords );	// Bloom
  vec4 col2 = texture2D( buf2, texCoords ); // Outline
  
  vec4 col3 = texture2D( buf3, texCoords);
	vec4 col = 1.0 - exp2( -hdrExposure * col0 );
	gl_FragColor = col3*col2*(col+col1);
}

[[FS_FXAA]]
//Thanks http://horde3d.org/wiki/index.php5?title=Shading_Technique_-_FXAA
uniform sampler2D buf0, buf1;
uniform vec2 frameBufSize;
uniform float hdrExposure;
varying vec2 texCoords;

void main( void )
{
	float FXAA_SPAN_MAX = 8.0;
	float FXAA_REDUCE_MUL = 1.0/8.0;
	float FXAA_REDUCE_MIN = 1.0/128.0;

	vec3 rgbNW=texture2D(buf0,texCoords+(vec2(-1.0,-1.0)/frameBufSize)).xyz;
	vec3 rgbNE=texture2D(buf0,texCoords+(vec2(1.0,-1.0)/frameBufSize)).xyz;
	vec3 rgbSW=texture2D(buf0,texCoords+(vec2(-1.0,1.0)/frameBufSize)).xyz;
	vec3 rgbSE=texture2D(buf0,texCoords+(vec2(1.0,1.0)/frameBufSize)).xyz;
	vec3 rgbM=texture2D(buf0,texCoords).xyz;
	
	vec3 luma=vec3(0.299, 0.587, 0.114);
	float lumaNW = dot(rgbNW, luma);
	float lumaNE = dot(rgbNE, luma);
	float lumaSW = dot(rgbSW, luma);
	float lumaSE = dot(rgbSE, luma);
	float lumaM  = dot(rgbM,  luma);
	
	float lumaMin = min(lumaM, min(min(lumaNW, lumaNE), min(lumaSW, lumaSE)));
	float lumaMax = max(lumaM, max(max(lumaNW, lumaNE), max(lumaSW, lumaSE)));
	
	vec2 dir;
	dir.x = -((lumaNW + lumaNE) - (lumaSW + lumaSE));
	dir.y =  ((lumaNW + lumaSW) - (lumaNE + lumaSE));
	
	float dirReduce = max(
		(lumaNW + lumaNE + lumaSW + lumaSE) * (0.25 * FXAA_REDUCE_MUL),
		FXAA_REDUCE_MIN);
	  
	float rcpDirMin = 1.0/(min(abs(dir.x), abs(dir.y)) + dirReduce);
	
	dir = min(vec2( FXAA_SPAN_MAX,  FXAA_SPAN_MAX),
		  max(vec2(-FXAA_SPAN_MAX, -FXAA_SPAN_MAX),
		  dir * rcpDirMin)) / frameBufSize;
		
	vec3 rgbA = (1.0/2.0) * (
		texture2D(buf0, texCoords.xy + dir * (1.0/3.0 - 0.5)).xyz +
		texture2D(buf0, texCoords.xy + dir * (2.0/3.0 - 0.5)).xyz);
	vec3 rgbB = rgbA * (1.0/2.0) + (1.0/4.0) * (
		texture2D(buf0, texCoords.xy + dir * (0.0/3.0 - 0.5)).xyz +
		texture2D(buf0, texCoords.xy + dir * (3.0/3.0 - 0.5)).xyz);
	float lumaB = dot(rgbB, luma);
  
  vec4 outputCol;
	if((lumaB < lumaMin) || (lumaB > lumaMax)){
		outputCol.xyz=rgbA;
	}else{
		outputCol.xyz=rgbB;
	}
  //Gamma correct
  gl_FragColor = pow(outputCol, 1.0/1.2);
}
