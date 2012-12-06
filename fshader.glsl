varying vec3 L;
varying vec3 E;
varying vec3 N;


uniform vec4 ambient;
uniform vec4 diffuse;
uniform vec4 specular;
uniform float specexp;
uniform float outline;
uniform float sun;

varying vec2 texCoord;
uniform sampler2D texture;


void main()
{
    // Normalize things
    vec3 NN = normalize (N);
    vec3 LL = normalize (L);

    // Compute terms in the illumination equation
    float Kd = max( dot(LL, NN), 0.0 );
    vec4  dif = Kd*diffuse;
    
    vec3 R = normalize (reflect (-LL, NN));
    float Ks = pow( max(dot(R, E), 0.0), specexp );
    vec4  spec = Ks * specular;


    if( dot(L, N) < 0.0 ) 
        spec = vec4(0.0, 0.0, 0.0, 1.0);

	vec4 color = texture2D( texture, texCoord );

	float intensity = dot(L, N);
	
	if (outline != 0.0)
		color = vec4(0.0, 0.0, 0.0, 1.0);
	else if (sun == 1.0)
		color *= 1.0;
	else if (intensity > 0.80)
		color *= 1.0;
	else if (intensity > 0.65)
		color *= .8;
	else if (intensity > 0.40)
		color *= .6;
	else if (intensity > 0.15)
		color *= .4;
	else if (intensity > 0.05)
		color *= .2;
	else
		color *= .1;

	gl_FragColor = color;
}
