/*
The most basic implementation of DFAA Antialiasing Algorithm.
More info is in the blog post http://alexpolt.github.io/dfaa.html

Alexandr Poltavsky
*/

static const float pi2 = 2*3.1415926;

float2 dfaa( float2 uv01 ) {

  float2 uvdx = ddx( uv01 );
  float2 uvdy = ddy( uv01 );

  //area - coverage, (steps+1)^2 - total number of samples, rad - radius of sampling

  float area=0, steps=4, rad=0.5;

  for(float y=0; y<=steps; y++) {
    for(float x=0; x<=steps; x++) {
      float2 dxdy = float2( 2*rad*x/steps - rad, 2*rad*y/steps - rad );
      float2 edge = uv01 + uvdx * dxdy.x  + uvdy * dxdy.y;
      // if we are out of the triangle - increase by one the non-coverage
      if( edge.x < 0 || edge.y < 0 || ( edge.x + edge.y ) > 1 ) area++;
    } 
  }

  //get coverage
  area = 1 - area / pow(steps+1, 2);

  //the next big step is to compute the direction of sampling
  //we get the inverse matrix and compute the edge vectors of the polygon
  //then we break the polygon into three independent parts by medians
  //and get the right edge, then rotate it by 90 degrees

  float2 dir;
  float2 uvdx2 = normalize( uvdx ), uvdy2 = normalize( uvdy );

  float det = 1 / ( uvdx2.x * uvdy2.y - uvdx2.y * uvdy2.x );
  float2 xydu = det * float2( uvdy2.y, -uvdx2.y );
  float2 xydv = det * float2( -uvdy2.x, uvdx2.x );

  float2 z = float2( xydv - xydu );
  if( uv01.y > uv01.x && (uv01.y+2*uv01.x) < 1 ) dir = float2( -xydv.y, xydv.x );
  else if( uv01.x > uv01.y && (uv01.x+2*uv01.y) < 1 ) dir = float2( xydu.y, -xydu.x );
  else dir = float2( z.y, -z.x );

  dir = cross( float3(xydu, 0), float3(xydv, 0) ).z > 0 ? dir : -dir;

  //encode direction as an angle
  float dir_atan = atan2( dir.y, dir.x );
  float dir_angle = ( dir_atan < 0 ? dir_atan + pi2 : dir_atan ) / pi2;

  return float2( dir_angle, area );

}


