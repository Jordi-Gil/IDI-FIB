#version 330 core

in vec3 vertex;
in vec3 scolor;
out vec3 fcolor;

void main(){
    fcolor = scolor;
    gl_Position = vec4 (vertex, 1.0);
}
