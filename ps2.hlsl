/*
The most basic implementation of DFAA Antialiasing Algorithm.
More info is in the blog post http://alexpolt.github.io/dfaa.html

Alexandr Poltavsky
*/

sampler tex0 : register(s0);
sampler tex1 : register(s1);

float4 main( float2 screenxy : TEXCOORD0 ) : COLOR0 {

	float4 color0 = tex2D( tex0, screenxy );
	float2 dir = normalize( tex2D( tex1, screenxy ).xy );
	float area = abs( color0.a );
	float2 sdx = ddx( screenxy );
	float2 sdy = ddy( screenxy );

	float4 color1 = tex2D(tex0, screenxy + sdx*dir.x + sdy*dir.y );

	float4 color = lerp( color1, color0, area );
 
	return float4( color.rgb, 1 );
}


