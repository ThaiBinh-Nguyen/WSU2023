
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
function set_vertex_attrib_to_buffer(gl, program, attrib_name, buffer, size, type, normalized, stride, offset) {
    let attrib_location = gl.getAttribLocation(program, attrib_name);
    gl.enableVertexAttribArray(attrib_location);
    gl.bindBuffer(gl.ARRAY_BUFFER, buffer);
    gl.vertexAttribPointer(attrib_location, size, type, normalized, stride, offset);
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

        let vertices = [
            // x, y, z, 
            -1, -1, -1,  // Vertex 0
            1, -1, -1, // Vertex 1
            1,  1, -1, // Vertex 2
            -1,  1, -1,  // Vertex 3
            -1, -1,  1,  // Vertex 4
            1, -1,  1,  // Vertex 5
            1,  1,  1,  // Vertex 6
            -1,  1,  1  // Vertex 7
        ];


        // Indices for the cube
        let indices = [
            0, 1, 2, 2, 3, 0,  // Front face
            4, 5, 6, 6, 7, 4,  // Back face
            4, 5, 1, 1, 0, 4,  // Bottom face
            7, 6, 2, 2, 3, 7,  // Top face
            4, 0, 3, 3, 7, 4,  // Left face
            1, 5, 6, 6, 2, 1   // Right face
        ];

        return new Mesh( gl, program, vertices, indices );
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

        set_vertex_attrib_to_buffer( 
            gl, this.program, 
            "coordinates", 
            this.verts, 3, 
            gl.FLOAT, false, VERTEX_STRIDE, 0 
        );


        set_vertex_attrib_to_buffer( 
            gl, this.program, 
            "color", 
            this.verts, 4, 
            gl.FLOAT, false, VERTEX_STRIDE, 12
        );

        gl.drawElements( gl.TRIANGLES, this.n_indis, gl.UNSIGNED_SHORT, 0 );
    }

    /**
     * Parse the given text as the body of an obj file.
     * @param {WebGLRenderingContext} gl
     * @param {WebGLProgram} program
     * @param {string} text
     */
    static from_obj_text( gl, program, text ) {
        // your code here
        return new Mesh( /*your arguments here*/ );
    }

    /**
     * Asynchronously load the obj file as a mesh.
     * @param {WebGLRenderingContext} gl
     * @param {string} file_name 
     * @param {WebGLProgram} program
     * @param {function} f the function to call and give mesh to when finished.
     */
    static from_obj_file( gl, file_name, program, f ) {
        let request = new XMLHttpRequest();
        
        // the function that will be called when the file is being loaded
        request.onreadystatechange = function() {
            // console.log( request.readyState );

            if( request.readyState != 4 ) { return; }
            if( request.status != 200 ) { 
                throw new Error( 'HTTP error when opening .obj file: ', request.statusText ); 
            }

            // now we know the file exists and is ready
            let loaded_mesh = Mesh.from_obj_text( gl, program, request.responseText );

            console.log( 'loaded ', file_name );
            f( loaded_mesh );
        };

        
        request.open( 'GET', file_name ); // initialize request. 
        request.send();                   // execute request
    }
}
