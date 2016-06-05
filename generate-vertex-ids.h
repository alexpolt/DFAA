
template<typename R, typename T>
R* generate_vertex_ids( T* index_buffer, int index_count, int vertex_count = 0 ) {

  //check preconditions
  if( ! index_buffer ) return nullptr;
  if( ! index_count ) return nullptr;
  if( ! vertex_count ) {
    for( int i = 0; i < index_count; i++ ) 
        vertex_count = vertex_count < index_buffer[i] ? index_buffer[i] : vertex_count;
    ++vertex_count;
  }

  int vid = 1; //vertex id
  int bad_tri = 0; //number of tris for which no vertex id could be generated

  #define mask(a,b,c) ((a) << 4) | ((b) << 2) | (c)
  typedef unsigned char byte;

  //a helper data structure for generating proper vertex ids
  struct mask_to_values {
    byte mask;
    byte value[3];
  };

  mask_to_values mv[] = {

    { mask(3,3,3), {0,1,2} },
    { mask(3,3,0), {1,2,0} },
    { mask(3,3,1), {0,2,1} },
    { mask(3,3,2), {0,1,2} },

    { mask(3,0,3), {1,0,2} },
    { mask(3,0,1), {2,0,1} },
    { mask(3,0,2), {1,0,2} },

    { mask(3,1,3), {0,1,2} },
    { mask(3,1,0), {2,1,0} },
    { mask(3,1,2), {0,1,2} },

    { mask(3,2,3), {0,2,1} },
    { mask(3,2,0), {1,2,0} },
    { mask(3,2,1), {0,2,1} },

    { mask(0,3,3), {0,1,2} },
    { mask(0,3,1), {0,2,1} },
    { mask(0,3,2), {0,1,2} },
    { mask(0,1,3), {0,1,2} },
    { mask(0,1,2), {3,3,3} },
    { mask(0,2,3), {0,2,1} },
    { mask(0,2,1), {3,3,3} },

    { mask(1,3,3), {1,2,0} },
    { mask(1,3,0), {1,2,0} },
    { mask(1,3,2), {1,0,2} },
    { mask(1,0,3), {1,0,2} },
    { mask(1,0,2), {3,3,3} },
    { mask(1,2,3), {1,2,0} },
    { mask(1,2,0), {3,3,3} },

    { mask(2,3,3), {2,1,0} },
    { mask(2,3,0), {2,1,0} },
    { mask(2,3,1), {2,0,1} },
    { mask(2,0,3), {2,0,1} },
    { mask(2,0,1), {3,3,3} },
    { mask(2,1,3), {2,1,0} },
    { mask(2,1,0), {3,3,3} },

  };

  //get the memory for the resulting vid buffer
  R* vids = (R*) operator new( vertex_count * sizeof( R ) );

  //init it with zeros
  for( int i = 0; i < vertex_count; i++ ) vids[ i ] = 0; 

  //loop through the index buffer
  for( int idx = 0; idx < index_count; idx+=3 ) {

    unsigned idx0 = index_buffer[idx], idx1 = index_buffer[idx+1], idx2 = index_buffer[idx+2]; 
    unsigned vid0 = vids[ idx0 ], vid1 = vids[ idx1 ], vid2 = vids[ idx2 ]; //existing vertex ids
    byte m = mask( !vid0 ? 3 : vid0%3, !vid1 ? 3 : vid1%3, !vid2 ? 3 : vid2%3 ); //helper mask, value 3 means vacant vertex id

    //loop through the mv array and find the mapping
    int i = 0; const int mv_size = sizeof(mv)/sizeof(mv[0]);
    for(; i < mv_size; i++ ) {

        if( m == mv[i].mask ) {
            if( 3 == mv[i].value[0] ) break; //proper vids are already in place

            if(!vid0) { 
              do { ++vid; } while( vid%3 != mv[i].value[0] );
              vids[ idx0 ] = vid;
            }

            if(!vid1) { 
              do { ++vid; } while( vid%3 != mv[i].value[1] );
              vids[ idx1 ] = vid;
            }

            if(!vid2) { 
              do { ++vid; } while( vid%3 != mv[i].value[2] );
              vids[ idx2 ] = vid;
            }

            break;
        } 
    }

    if( i == mv_size) //no mask was found, means we've got a bad tri
      bad_tri++; //print it to get the number of tris with wrong vertex ids, no or partial AA on them

  }

  return vids;
}

