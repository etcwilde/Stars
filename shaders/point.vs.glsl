#version 330
layout (location=0) in vec4 vPosition;
layout (location=1) in vec4 vColor;
layout (location=2) in vec2 vMassRad;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

out vec4 fcolor;
out vec2 fmassRad;
out vec4 flightDir;

vec4 lightPos = vec4(10.0, 10.0, 10.0, 1.0);

void main()
{
        fcolor = vColor;
        fmassRad = vMassRad;
        //flightDir = normalize(lightPos - vPosition);
        //flightDir = vec4(1.0, 0.0, 1.0, 1.0);

        //vec4 worldPosition = M * vPosition;
        //flightDir = vec4(normalize((lightPos - vPosition).xyz), 1.0);

        gl_Position= P * V * M * vPosition;
        gl_PointSize =  10 * fmassRad.y;
}
