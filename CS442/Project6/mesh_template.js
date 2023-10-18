
const VERTEX_STRIDE = 28;
/**
 * Creates a new index buffer and loads it into video memory.
 * 
 * @param {WebGLRenderingContext} gl  
 * @param {number[]} indices
 * @param {number} usage - Specifies the expected usage pattern of the data store.
 * @return {WebGLBuffer}
 */
function create_and_load_elements_buffer(gl, indices, usage) {
    // Create a buffer
    const indexBuffer = gl.createBuffer();

    // Bind the buffer
    gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, indexBuffer);

    // Load indices into the buffer
    gl.bufferData(gl.ELEMENT_ARRAY_BUFFER, new Uint16Array(indices), usage);

    return indexBuffer;
}

function create_and_load_vertex_buffer(gl, vertices, usage) {
    // Create a buffer
    const vertexBuffer = gl.createBuffer();

    // Bind the buffer
    gl.bindBuffer(gl.ARRAY_BUFFER, vertexBuffer);

    // Load vertices into the buffer
    gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(vertices), usage);

    return vertexBuffer;
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
        let verts = [
            -1, -1, -1,  // Vertex 0
            1, -1, -1, // Vertex 1
            1,  1, -1, // Vertex 2
            -1,  1, -1,  // Vertex 3
            -1, -1,  1,  // Vertex 4
            1, -1,  1,  // Vertex 5
            1,  1,  1,  // Vertex 6
            -1,  1,  1  // Vertex 7
        ];

        let indis = [
            // clockwise winding
            
            0, 1, 2, 2, 3, 0, 
            4, 0, 3, 3, 7, 4, 
            5, 4, 7, 7, 6, 5, 
            1, 5, 6, 6, 2, 1,
            3, 2, 6, 6, 7, 3,
            4, 5, 1, 1, 0, 4,
            
        ];


        return new Mesh( gl, program, verts, indis );
    }


    /**
     * Render the mesh. Does NOT preserve array/index buffer or program bindings! 
     * 
     * @param {WebGLRenderingContext} gl 
     */
    render( gl ) {
        
        gl.useProgram( this.program );
        gl.bindBuffer( gl.ARRAY_BUFFER, this.verts );
        gl.bindBuffer( gl.ELEMENT_ARRAY_BUFFER, this.indis );

        let atr_coord = gl.getAttribLocation(shader_program, "coordinates");
        gl.vertexAttribPointer(atr_coord, 3, gl.FLOAT, false, 12, 0); // 24 bytes per vertex
        gl.enableVertexAttribArray(atr_coord);

        gl.drawElements( gl.TRIANGLES, this.n_indis, gl.UNSIGNED_SHORT, 0 );
    }
}