/*
The most basic implementation of DFAA Antialiasing Algorithm.
More info is in the blog post http://alexpolt.github.io/dfaa.html

Alexandr Poltavsky
*/

//pixel to display and packed DFAA in alpha
sampler tex0 : register(s0); 

static const float pi2 = 2*3.1415926;

float4 main( float2 screenxy : TEXCOORD0 ) : COLOR0 {
  
  float4 color0 = tex2D( tex0, screenxy );
  float dfaa = color0.w;
  
  //unpack
  float dir_angle = floor(255*dfaa/16)/15;
  float2 dir = float2( cos( dir_angle * pi2 ), sin( dir_angle * pi2 ) );
  float area = frac(255*dfaa/16)*16/15;
         
  float2 sdx = ddx( screenxy );
  float2 sdy = ddy( screenxy );
  
  float4 color1 = color0;
  
  //only do fetch if necessary (edge pixels)
  if( area > 0 && area < 1 ) 
    color1 = tex2D(tex0, screenxy + sdx*dir.x + sdy*dir.y );
  
  float3 color = lerp( color1, color0, area ).rgb;
  
  return float4( color, 1 );
}


