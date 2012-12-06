attribute vec4 vPosition;
attribute vec3 vNormal;
attribute vec2 vTexCoord;

uniform vec3 theta;
uniform vec3 trans;
uniform vec3 scale;

uniform vec3 cPosition;
uniform vec3 cLookAt;
uniform vec3 cUp;

uniform float left;
uniform float right;
uniform float top;
uniform float bottom;
uniform float near;
uniform float far;

uniform int doOrtho;

// Light position is given in world space
uniform vec4 LightPosition;

// all vectors will be calculated in eye space
varying vec3 L;  // to light
varying vec3 E;  // to eye
varying vec3 N;  // normal
varying vec2 texCoord;


void main()
{
    // Compute the sines and cosines of each rotation
    // about each axis
    vec3 angles = radians (theta);
    vec3 c = cos (angles);
    vec3 s = sin (angles);
    
    // rotation matricies
    mat4 rx = mat4 (1.0,  0.0,  0.0,  0.0, 
                    0.0,  c.x,  s.x,  0.0,
                    0.0, -s.x,  c.x,  0.0,
                    0.0,  0.0,  0.0,  1.0);
                    
    mat4 ry = mat4 (c.y,  0.0, -s.y,  0.0, 
                    0.0,  1.0,  0.0,  0.0,
                    s.y,  0.0,  c.y,  0.0,
                    0.0,  0.0,  0.0,  1.0);

    mat4 rz = mat4 (c.z, -s.z,  0.0,  0.0, 
                    s.z,  c.z,  0.0,  0.0,
                    0.0,  0.0,  1.0,  0.0,
                    0.0,  0.0,  0.0,  1.0);

    mat4 transMat = mat4 ( 1.0, 0.0, 0.0, 0.0,
			0.0, 1.0, 0.0, 0.0,
			0.0, 0.0, 1.0, 0.0,
			trans.x, trans.y, trans.z, 1.0);
            
    mat4 scaleMat = mat4 (scale.x,  0.0,     0.0,     0.0,
                          0.0,      scale.y, 0.0,     0.0, 
                          0.0,      0.0,     scale.z, 0.0,
                          0.0,      0.0,     0.0,     1.0);
    
    mat4 modelMat =  transMat * rz * ry * rx * scaleMat;
                          
    // create view matrix
    vec3 nVec = normalize (cPosition - cLookAt);
    vec3 uVec = cross (normalize(cUp), nVec);
    vec3 vVec = cross (nVec, uVec);
    mat4 viewMat = mat4 (uVec.x,  vVec.x, nVec.x, 0.0,
                         uVec.y,  vVec.y, nVec.y, 0.0,
                         uVec.z,  vVec.z, nVec.z, 0.0,
                         -1.0*(dot(uVec, cPosition)), -1.0*(dot(vVec, cPosition)),
                         -1.0*(dot(nVec, cPosition)), 1.0);
                         
    // create projection matrix
    mat4 orthoMat = mat4 ((2.0/(right-left)), 0.0, 0.0, 0.0, 
                        0.0, (2.0 /(top-bottom)), 0.0, 0.0, 
                        0.0, 0.0, (-2.0/(far - near)), 0.0, 
                        (-1.0*(right+left)/(right-left)), (-1.0*(top+bottom)/(top-bottom)),
                        (-1.0*(far+near)/(far-near)), 1.0);
                        
    mat4 frustrumMat = mat4 ((2.0*near)/(right-left), 0.0, 0.0, 0.0, 
                             0.0, ((2.0*near)/(top-bottom)), 0.0, 0.0, 
                             ((right+left)/(right-left)), ((top+bottom)/(top-bottom)),
                             ((-1.0*(far+near)) / (far-near)), -1.0,
                             0.0, 0.0, ((-2.0*far*near)/(far-near)), 0.0);
                             
    mat4 projMat;
    
    if (doOrtho == 1) {
        projMat = orthoMat;
    }
    else {
        projMat = frustrumMat;
    }
    
    // Lighting vectors to be passed to fragment shader
    vec4 vertexInEye = viewMat * modelMat * vPosition;
    vec4 lightInEye = viewMat * LightPosition;
    vec3 normalInEye = normalize(viewMat * modelMat * vec4(vNormal, 0.0)).xyz;

    L = normalize((lightInEye - vertexInEye).xyz);
    N = normalInEye;
    E = normalize ((-vertexInEye).xyz);
          
	//gl_TexCoord[0] = gl_MultiTexCoord0;
	texCoord = vTexCoord;
    
    gl_Position =  projMat * viewMat * modelMat * vPosition;
}
