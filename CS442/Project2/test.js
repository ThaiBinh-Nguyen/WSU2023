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
            
const rot_xz2 = Mat4.rotation_xz( .125 );
const rot_xy2 = Mat4.rotation_xy( .125 );
const rot_yz2 = Mat4.rotation_yz( .125 );
const translate2 = Mat4.translation( -1, 9, 3 );
const scale2 = Mat4.scale( 0.2, -0.1, 19 );
const mul2 = scale2.mul( translate2 );
const vec3 = new Vec4( 3, -3, 2, 0 );
            
console.log( '1/8th turn XZ rotation: ', rot_xz2.toString() ); 
console.log( '1/8th turn XY rotation: ', rot_xy2.toString() ); 
console.log( '1/8th turn YZ rotation: ', rot_yz2.toString() ); 
console.log( 'Translate by -1, 9, 3: ', translate2.toString() );
console.log( 'Scale by 0.2, -0.1, 19: ', scale2.toString() );
console.log( 'Translate and then scale: ', mul2.toString() );
console.log( 'Transform a vector with the preceding matrix: ', 
                mul2.transform_vec(vec3).toString() );
            
const vec4 = new Vec4( 3, 6, 9, 1 );
const scaled2 = vec3.scaled( 10 );
const dot2 = vec3.dot( vec4 );
const length2 = vec3.length();
const norm2 = vec3.norm();
const add2 = vec3.add( vec4 );
            
 console.log( 'Vector multiplied by scaler: ' , scaled2.toString() );
 console.log( 'Vector dot vector: ', dot2.toString() );
 console.log( 'Vector length: ', length2 );
 console.log( 'Vector norm: ', norm2.toString() );
 console.log( 'Vector add: ', add2.toString() );