
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

  //get the memory for the resulting vid buffer, delete with delete[]
  R* vids = new R[ vertex_count ];

  //init it with 3 (3 means vacant)
  for( int i = 0; i < vertex_count; i++ ) vids[ i ] = 3;

  int bad_tri = 0; //number of tris for which no good 0,1,2 mapping exists

  //loop through the index buffer
  for( int idx = 0; idx < index_count; idx+=3 ) {

    byte m = mask( vids[ index_buffer[idx+0] ], 
                   vids[ index_buffer[idx+1] ], 
                   vids[ index_buffer[idx+2] ] ); //helper mask for searching in mv array

    //loop through the mv array and find the mapping
    int i = 0; const int mv_size = sizeof(mv)/sizeof(mv[0]);
    for(; i < mv_size; i++ ) {

        if( m == mv[i].mask ) {
            if( 3 == mv[i].value[0] ) break; //proper vids are already in place, early out
            //assign
            vids[idx+0] = mv[i].value[0];
            vids[idx+1] = mv[i].value[1];
            vids[idx+2] = mv[i].value[2];
            break;
        } 
    }

    if( i == mv_size) //no mask was found, means we've got a tri with no proper mapping, gets zeros
      bad_tri++; //print it to get the number of tris with wrong vertex ids, no or partial AA on them

  }

  return vids;
}

