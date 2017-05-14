#version 330 core

in vec3 vertex;
in vec3 normal;

in vec3 matamb;
in vec3 matdiff;
in vec3 matspec;
in float matshin;

uniform mat4 PM;
uniform mat4 VM;
uniform mat4 TG;

out vec3 fmatamb;
out vec3 fmatdiff;
out vec3 fmatspec;
out float fmatshin;
out mat4 viewmatrix;
out mat4 tg;
out vec4 vertSCO;
out vec3 normalSCO;

void main()
{
    vertSCO = VM * TG * vec4(vertex,1.0);
    normalSCO = normalize(inverse(transpose(mat3(VM * TG)))*normal);

    //gl_Position = PM * vertSCO;
    gl_Position = PM * VM * TG * vec4 (vertex, 1.0);

    fmatamb = matamb;
    fmatdiff = matdiff;
    fmatspec = matspec;
    fmatshin = matshin;
    viewmatrix = VM;
    tg = TG;

}
