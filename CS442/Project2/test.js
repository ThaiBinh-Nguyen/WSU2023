// Nếu các tệp nằm trong cùng một thư mục với test.js
const { Mat4 } = require('./matrix_template.js');
const { Vec4 } = require('./vector_template.js');


const rot_xz = Mat4.rotation_xz( .25 );
const rot_xy = Mat4.rotation_xy( .25 );
const rot_yz = Mat4.rotation_yz( .25 );
const translate = Mat4.translation( 4, 3, 5 );
const scale = Mat4.scale( 2, 3, 4 );
const mul = scale.mul( translate );
const vec = new Vec4( 1, 2, 0.5, 1 );
            
console.log( 'Quarter turn XZ rotation: ', rot_xz.toString() ); 
console.log( 'Quarter turn XY rotation: ', rot_xy.toString() ); 
console.log( 'Quarter turn YZ rotation: ', rot_yz.toString() ); 
console.log( 'Translate by 4, 3, 5: ', translate.toString() );
console.log( 'Scale by 2, 3, 4: ', scale.toString() );
console.log( 'Translate and then scale: ', mul.toString() );
console.log( 'Transform a vector with the preceding matrix: ', 
    mul.transform_vec(vec).toString() );

const vec2 = new Vec4( 3, 6, 9, 1 );
const scaled = vec.scaled( 10 );
const dot = vec.dot( vec2 );
const length = vec2.length();
const norm = vec2.norm();
const add = vec.add( vec2 );
            
console.log( 'Vector multiplied by scaler: ' , scaled.toString() );
console.log( 'Vector dot vector: ', dot.toString() );
console.log( 'Vector length: ', length );
console.log( 'Vector norm: ', norm.toString() );
console.log( 'Vector add: ', add.toString() );