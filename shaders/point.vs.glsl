#version 330
layout (location=0) in vec4 vPosition;
layout (location=1) in vec4 vColor;
layout (location=2) in vec2 vMassRad;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

out vec4 fcolor;
out vec2 fmassRad;
//out vec4 flightDir;
//vec4 lightPos = vec4(10.0, 10.0, 10.0, 0.0);

void main()
{
        fcolor = vColor;
        fmassRad = vMassRad;
        mat4 viewInv = inverse(V);
        vec4 eyePos = vec4(viewInv[3][0], viewInv[3][1], viewInv[3][2], 0) / viewInv[3].w;
        gl_Position= P * V * M * vPosition;
        gl_PointSize =  (1000 * fmassRad.y) / length(vPosition-eyePos);
}
