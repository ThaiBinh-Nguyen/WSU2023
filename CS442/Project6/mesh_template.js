
const VERTEX_STRIDE = 28;
 /** 
  * Creates a new vertex buffer and loads it full of the given data.
  * Preserves bound buffer.
  * 
  * @param {WebGLRenderingContext} gl  
  * @param {number[]} data
  * @param {number} usage
  * 
  * @returns {WebGlBuffer}
 */
 function create_and_load_vertex_buffer(gl, data, usage) {
    let current_array_buf = gl.getParameter( gl.ARRAY_BUFFER_BINDING );

    let buf_id = gl.createBuffer();
    gl.bindBuffer( gl.ARRAY_BUFFER, buf_id );
    gl.bufferData( gl.ARRAY_BUFFER, new Float32Array(data), usage );
    
    gl.bindBuffer( gl.ARRAY_BUFFER, current_array_buf );

    return buf_id;
}

 /** 
  * Creates a new index buffer and loads it full of the given data.
  * Preserves bound buffer.
  * 
  * @param {WebGLRenderingContext} gl  
  * @param {number[]} data
  * @param {number} usage
  * 
  * @returns {WebGlBuffer}
 */
 function create_and_load_elements_buffer(gl, data, usage) {
    let current_buf = gl.getParameter( gl.ELEMENT_ARRAY_BUFFER_BINDING );
    
    let buf_id = gl.createBuffer();
    gl.bindBuffer( gl.ELEMENT_ARRAY_BUFFER, buf_id );
    gl.bufferData( gl.ELEMENT_ARRAY_BUFFER, new Uint16Array(data), usage );

    gl.bindBuffer( gl.ELEMENT_ARRAY_BUFFER, current_buf );
    
    return buf_id;
}

/**
 * Sets the buffer for a given vertex attribute name. 
 * 
 * @param {WebGLRenderingContext} gl 
 * @param {WebGLProgram} program 
 * @param {string} attrib_name 
 * @param {WebGLBuffer} buffer
 * @param {number} n_components 
 * @param {number} gl_type 
 * @param {number} stride 
 * @param {number} offset
 */
function set_vertex_attrib_to_buffer( 
    gl, program, attrib_name, buffer, n_components, gl_type, normalize, stride, offset ) 
{
    let attr_loc = gl.getAttribLocation( program, attrib_name );
    
    if ( attr_loc == - 1 ) { 
        throw new Error( 'either no attribute named "' + attrib_name + 
            '" in program or attribute name is reserved/built-in.' ) 
    } 

    let err = gl.getError()
    if ( err != 0 ) {
        throw new Error( 'invalid program. Error: ' + err );
    }

    let current_array_buf = gl.getParameter( gl.ARRAY_BUFFER_BINDING );

    gl.bindBuffer( gl.ARRAY_BUFFER, buffer );
    gl.enableVertexAttribArray( attr_loc );
    gl.vertexAttribPointer( attr_loc, n_components, gl_type, normalize, stride, offset );
    //gl.enableVertexAttribArray( attr_loc );

    gl.bindBuffer( gl.ARRAY_BUFFER, current_array_buf );
}


class Mesh {
    /** 
     * Creates a new mesh and loads it into video memory.
     * 
     * @param {WebGLRenderingContext} gl  
     * @param {number} program
     * @param {number[]} vertices
     * @param {number[]} indices
    */
    constructor( gl, program, vertices, indices ) {
        this.verts = create_and_load_vertex_buffer( gl, vertices, gl.STATIC_DRAW );
        this.indis = create_and_load_elements_buffer( gl, indices, gl.STATIC_DRAW );

        this.n_verts = vertices.length;
        this.n_indis = indices.length;
        this.program = program;
    }

    /**
     * Create a box mesh with the given dimensions and colors.
     * @param {WebGLRenderingContext} gl 
     * @param {number} width 
     * @param {number} height 
     * @param {number} depth 
     */

    static box( gl, program, width, height, depth ) {
        let hwidth = width / 2.0;
        let hheight = height / 2.0;
        let hdepth = depth / 2.0;

        let verts = [
            hwidth, -hheight, -hdepth,      1.0, 0.0, 0.0, 1.0,
            -hwidth, -hheight, -hdepth,     0.0, 1.0, 0.0, 1.0,
            -hwidth, hheight, -hdepth,      0.0, 0.0, 1.0, 1.0,
            hwidth, hheight, -hdepth,       1.0, 1.0, 0.0, 1.0,

            hwidth, -hheight, hdepth,       1.0, 0.0, 1.0, 1.0,
            -hwidth, -hheight, hdepth,      0.0, 1.0, 1.0, 1.0,
            -hwidth, hheight, hdepth,       0.5, 0.5, 1.0, 1.0,
            hwidth, hheight, hdepth,        1.0, 1.0, 0.5, 1.0,
        ];

        let indis = [
            // clockwise winding
            /*
            0, 1, 2, 2, 3, 0, 
            4, 0, 3, 3, 7, 4, 
            5, 4, 7, 7, 6, 5, 
            1, 5, 6, 6, 2, 1,
            3, 2, 6, 6, 7, 3,
            4, 5, 1, 1, 0, 4,
            */

            // counter-clockwise winding
            0, 3, 2, 2, 1, 0,
            4, 7, 3, 3, 0, 4,
            5, 6, 7, 7, 4, 5,
            1, 2, 6, 6, 5, 1,
            3, 7, 6, 6, 2, 3,
            4, 0, 1, 1, 5, 4,
        ];

        return new Mesh( gl, program, verts, indis );
    }


    /**
     * Render the mesh. Does NOT preserve array/index buffer or program bindings! 
     * 
     * @param {WebGLRenderingContext} gl 
     */
    /**
     * Render the mesh. Does NOT preserve array/index buffer or program bindings! 
     * 
     * @param {WebGLRenderingContext} gl 
     */
    render( gl ) {
        
        gl.cullFace( gl.BACK );
        gl.enable( gl.CULL_FACE );
        
        gl.useProgram( this.program );
        gl.bindBuffer( gl.ARRAY_BUFFER, this.verts );
        gl.bindBuffer( gl.ELEMENT_ARRAY_BUFFER, this.indis );

        set_vertex_attrib_to_buffer( 
            gl, this.program, 
            "coordinates", 
            this.verts, 3, 
            gl.FLOAT, false, VERTEX_STRIDE, 0 
        );

        gl.drawElements( gl.TRIANGLES, this.n_indis, gl.UNSIGNED_SHORT, 0 );
    }
}