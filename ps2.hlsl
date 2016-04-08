/*
The most basic implementation of DFAA Antialiasing Algorithm.
More info is in the blog post http://alexpolt.github.io/dfaa.html

Alexandr Poltavsky
*/

//forward declare
float3 DFAA( float2 screenxy );

//pixel to display and packed DFAA in alpha
sampler tex0: register(s0);

static const float pi2 = 2 * 3.1415926;


/* full screen pixel shader main  */
float4 main( float2 screenxy : TEXCOORD0 ) : COLOR0 {

  return float4( DFAA( screenxy ), 1 );

}

//defines used by DFAA
#define dfaa_screen_texture tex0
#define dfaa_tex2D tex2D

//DFAA. Performs fetch, unpack and lerp.
float3 DFAA( float2 screenxy ) {
  
  float4 color0 = dfaa_tex2D( dfaa_screen_texture, screenxy );
  float dfaa_packed = color0.a;
  
  //unpack
  float dir_angle = floor(255*dfaa_packed/16)/15;
  float2 dir = float2( cos( dir_angle * pi2 ), sin( dir_angle * pi2 ) );
  float area = frac(255*dfaa_packed/16)*16/15;
  
  float2 sdx = ddx( screenxy );
  float2 sdy = ddy( screenxy );
  
  float4 color1 = color0;
  
  //only do fetch if necessary (edge pixels)
  if( area > 0 && area < 1 ) 
    color1 = dfaa_tex2D( dfaa_screen_texture, screenxy + sdx*dir.x + sdy*dir.y );
  
  return lerp( color1, color0, area ).rgb;
}


