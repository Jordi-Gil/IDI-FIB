#version 330 core

in vec4 gl_FragCoord;
in vec3 fcolor;
out vec4 FragColor;

uniform vec2 WindowSize;

void main() {
    
    if(int(gl_FragCoord .y) % 10 >= 5) discard;
    if(int(gl_FragCoord .x) % 10 >= 5) discard;
    else{
        if(gl_FragCoord .x < 355. && gl_FragCoord .y > 355.){
            FragColor = vec4(1.0,0.0,0.0,1.0);
        }
        else if(gl_FragCoord .x < 355. && gl_FragCoord .y < 355.){
            FragColor = vec4(1.0,1.0,0.0,1.0);
        }
        else if(gl_FragCoord .x > 355. && gl_FragCoord .y > 355.){
            FragColor = vec4(0.0,0.0,1.0,1.0);
        }
        else{
            FragColor = vec4(0.0,1.0,0.0,1.0);
        }
    }
}

