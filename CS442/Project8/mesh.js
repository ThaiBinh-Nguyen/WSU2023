
const VERTEX_STRIDE = 36;

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

    static make_uv_sphere(gl, program, subdivs, material) {
        let verts = [];
        let normals = [];
        let uvs = [];
        let inds = [];
        const TAU = 2 * Math.PI;

        function calculateNormal(x, y, z) {
            let length = Math.sqrt(x * x + y * y + z * z);
            return [x / length, y / length, z / length];
        }
    
        for (let layer = 0; layer <= subdivs; layer++) {
            let y_turns = layer / subdivs / 2;
            let y = Math.cos(y_turns * TAU) / 2;
            let rs = Math.sin(TAU * y_turns);
    
            for (let subdiv = 0; subdiv < subdivs; subdiv++) {
                let turns = subdiv / subdivs;
                let rads = turns * TAU;
                let x = Math.cos(rads) / 2 * rs;
                let z = Math.sin(rads) / 2 * rs;
    
                let normal = calculateNormal(x, y, z);
                normals.push(...normal);
    
                verts.push(x, y, z);
                
                // Thêm UVs
                let u = subdiv / subdivs;
                let v = layer / subdivs;
                uvs.push(u, v);
            }
        }
    
        for (let lat = 0; lat < subdivs; lat++) {
            for (let lon = 0; lon < subdivs; lon++) {
                let first = (lat * (subdivs + 1)) + lon;
                let second = first + subdivs + 1;
                inds.push(first);
                inds.push(second);
                inds.push(first + 1);
    
                inds.push(second);
                inds.push(second + 1);
                inds.push(first + 1);
            }
        }
        return new Mesh( gl, program, verts, inds );
    }
    /*
    
     * Create a box mesh with the given dimensions and colors.
     * @param {WebGLRenderingContext} gl 
     * @param {number} width 
     * @param {number} height 
     * @param {number} depth 
     

    static box( gl, program, width, height, depth ) {
        let hwidth = width / 2.0;
        let hheight = height / 2.0;
        let hdepth = depth / 2.0;

        let verts = [
            // Front face
            hwidth, -hheight, -hdepth,      1.0, 0.0, 0.0, 1.0, 0.25, 0.5,
            -hwidth, -hheight, -hdepth,     0.0, 1.0, 0.0, 1.0, 0, 0.5, 
            -hwidth, hheight, -hdepth,      0.0, 0.0, 1.0, 1.0, 0, 0.25,
            hwidth, hheight, -hdepth,       1.0, 1.0, 0.0, 1.0, 0.25, 0.25,

            // Right face
            hwidth, -hheight, hdepth,       1.0, 1.0, 1.0, 1.0, 0.5, 0.5,
            hwidth, -hheight, -hdepth,      1.0, 0.0, 0.0, 1.0, 0.25, 0.5, 
            hwidth, hheight, -hdepth,       1.0, 0.5, 0.5, 1.0, 0.25, 0.25,
            hwidth, hheight, hdepth,        1.0, 1.0, 0.0, 1.0, 0.5, 0.25,

            // Back face
            -hwidth, -hheight, hdepth,      1.0, 0.0, 1.0, 1.0, 0.75, 0.5,
            hwidth, -hheight, hdepth,       0.0, 1.0, 1.0, 1.0, 0.5, 0.5, 
            hwidth, hheight, hdepth,        0.5, 0.5, 1.0, 1.0, 0.5, 0.25,
            -hwidth, hheight, hdepth,       1.0, 1.0, 0.5, 1.0, 0.75, 0.25,

            // Left face
            -hwidth, -hheight, -hdepth,     0.5, 1.0, 0.5, 1.0, 1, 0.5,
            -hwidth, -hheight, hdepth,      0.5, 0.5, 0.0, 1.0, 0.75, 0.5,
            -hwidth, hheight, hdepth,       0.0, 1.0, 1.0, 1.0, 0.75, 0.25,
            -hwidth, hheight, -hdepth,      0.0, 0.0, 0.5, 1.0, 1, 0.25,

            // Top face
            -hwidth, hheight, hdepth,       1.0, 0.5, 0.5, 1.0, 0.5, 0.25,
            hwidth, hheight, hdepth,        0.5, 0.5, 0.0, 1.0, 0.5, 0,
            hwidth, hheight, -hdepth,       0.5, 1.0, 0.5, 1.0, 0.75, 0,
            -hwidth, hheight, -hdepth,      1.0, 0.0, 0.5, 1.0, 0.75, 0.25,

            // Bottom face
            -hwidth, -hheight, -hdepth,       0.5, 0.0, 1.0, 1.0, 0.75, 0.75,
            hwidth, -hheight, -hdepth,      0.0, 0.5, 0.5, 1.0, 0.5, 0.75,
            hwidth, -hheight, hdepth,     0.0, 0.5, 1.0, 1.0, 0.5, 0.5,
            -hwidth, -hheight, hdepth,      0.5, 0.0, 0.0, 1.0, 0.75, 0.5,

        ];
        

        let indis = [
            
             // counter-clockwise winding
            0, 3, 2, 2, 1, 0,           //Front
            4, 7, 6, 6, 5, 4,           //Right
            8, 11, 10, 10, 9, 8,        //Back
            12, 15, 14, 14, 13, 12,     //Left
            18, 17, 16, 16, 19, 18,     //Top
            22, 21, 20, 20, 23, 22,     //Bottom
        ];
        
        return new Mesh( gl, program, verts, indis );
    }
*/

    /**
     * Render the mesh. Does NOT preserve array/index buffer or program bindings! 
     * 
     * @param {WebGLRenderingContext} gl 
     */
    render( gl ) {
        //gl.cullFace( gl.BACK );
        //gl.enable( gl.CULL_FACE );
        
        gl.useProgram( this.program );
        gl.bindBuffer( gl.ARRAY_BUFFER, this.verts );
        gl.bindBuffer( gl.ELEMENT_ARRAY_BUFFER, this.indis );

        set_vertex_attrib_to_buffer( 
            gl, this.program, 
            "coordinates", 
            this.verts, 3, 
            gl.FLOAT, false, VERTEX_STRIDE, 0 
        );
/*
        set_vertex_attrib_to_buffer( 
            gl, this.program, 
            "color", 
            this.verts, 4, 
            gl.FLOAT, false, VERTEX_STRIDE, 12
        );*/

        set_vertex_attrib_to_buffer( 
            gl, this.program, 
            "uv", 
            this.verts, 2, 
            gl.FLOAT, false, VERTEX_STRIDE, 28
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
        let lines = text.split( /\r?\n/ );

        let verts = [];
        let indis = [];
        

        for( let line of lines ) {
            let trimmed = line.trim();
            let parts = trimmed.split( /(\s+)/ );

            if( 
                parts === null || parts.length < 2 || 
                parts[0] == '#' || parts[0] === '' ) 
            { 
                continue; 
            }
            else if( parts[0] == 'v' ) {
                
                verts.push( parseFloat( parts[2] ) );
                verts.push( parseFloat( parts[4] ) );
                verts.push( parseFloat( parts[6] ) );
                
                // color data
                verts.push( 1, 1, 1, 1 );
            }
            else if( parts[0] == 'f' ) {
                indis.push( parseInt( parts[2] ) - 1 );
                indis.push( parseInt( parts[4] ) - 1 );
                indis.push( parseInt( parts[6] ) - 1 );
            }
            else {
                console.log( parts) ;
                throw new Error( 'unsupported obj command: ', parts[0], parts );
            }
        }
		
		//console.log( verts.slice(540, 600) )
		// console.log( indis.slice(540, 600) )
        
        return new Mesh( gl, program, verts, indis );
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