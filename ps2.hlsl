/*
The most basic implementation of DFAA Antialiasing Algorithm.
More info is in the blog post http://alexpolt.github.io/dfaa.html

Alexandr Poltavsky
*/


sampler tex0 : register(s0);
sampler tex1 : register(s1);

static const float pi2 = 2*3.1415926;

float4 main( float2 screenxy : TEXCOORD0 ) : COLOR0 {

  float4 color0 = tex2D( tex0, screenxy );
  float2 dfaa = tex2D( tex1, screenxy ).xy;
  float area = dfaa.y;
  //decode direction, angle of 0 is valid, so we need this area > 0 check
  float2 dir = area > 0 ? float2( cos( dfaa.x * pi2 ), sin( dfaa.x * pi2 ) ) : 0;
  float2 sdx = ddx( screenxy );
  float2 sdy = ddy( screenxy );

  if( noaa ) return float4( color0.rgb, 1 );

  float4 color1 = tex2D(tex0, screenxy + sdx*dir.x + sdy*dir.y );

  float4 color = lerp( color1, color0, area );

  return float4( color.rgb, 1 );
}

