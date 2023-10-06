
const VERTEX_STRIDE = 28;


function create_and_load_vertex_buffer(gl, vertices, usage) {
    let Buffer = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, Buffer);
    gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(vertices), usage);
    return Buffer;
}

function create_and_load_elements_buffer(gl, vertices, usage) {
    let Buffer = gl.createBuffer();
    gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, Buffer);
    gl.bufferData(gl.ELEMENT_ARRAY_BUFFER, new Int16Array(vertices), usage);
    return Buffer;
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
        // Initialize empty arrays for vertices and indices
        let vertices = [];
        let indices = [];
    
        // Split the text into lines
        let lines = text.split(/\r?\n/);
    
        // Loop through each line to parse vertices and faces
        for(let i = 0; i < lines.length; i++) {
            let line = lines[i].trim();

            // Split the line into segments based on whitespace
            let parts_of_line = line.split(/(\s+)/);

            let filtered_parts = [];
            for(let i = 0; i < parts_of_line.length; i++) {
                let part = parts_of_line[i].trim();
                if(part.length > 0) {
                    filtered_parts.push(part);
                }
            }
            
            parts_of_line = filtered_parts;
            // If the first segment is 'v', it's vertex information
            if(parts_of_line[0] === 'v') {
                let x = parseFloat(parts_of_line[1]);
                let y = parseFloat(parts_of_line[2]);
                let z = parseFloat(parts_of_line[3]);
                vertices.push(x, y, z);
                // Add color to the vertex (here it's green)
                vertices.push(0, 0, 0, 1);
            } 
            // If the first segment is 'f', it's face information
            else if(parts_of_line[0] === 'f') {
                let index1 = parseInt(parts_of_line[1]) - 1;
                let index2 = parseInt(parts_of_line[2]) - 1;
                let index3 = parseInt(parts_of_line[3]) - 1;
                indices.push(index1, index2, index3);
            }
        }
        return new Mesh(gl, program, vertices, indices);
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
