#version 330
// layout (location=1) in vec4 vColor;

uniform mat4 MVP;

float Ns = 250;
vec4 mat_spec = vec4(1);
vec4 light_spec = vec4(1);

out vec4 vFragColor;

in vec4 fcolor;
in vec2 fmassRad;
in vec4 flightDir;


void main()
{
        vec3 N;
        N.xy = gl_PointCoord * 2.0 - vec2(1.0);
        float mag = dot(N.xy, N.xy);
        if (mag > 1.0) discard;
        N.z = sqrt(1.0 - mag);
        vec3 halfVector = vec3(0.0, 0.0, 1.0);//normalize(vec3(0.0, 0.0, 1.0) + vec3(0.0, 0.0, 1.0))
        float spec = max(pow(dot(N, halfVector), Ns), 0.);
        vec4 S = (fcolor * spec) * 2. + spec * vec4(1.0, 1.0, 1.0, 1.0);

        float diffuse = max(0.0, dot(vec3(0.0, 0.0, 1.0), N));
        vFragColor = fcolor * diffuse + S;
        //vFragColor = fcolor * diffuse + S;
}
