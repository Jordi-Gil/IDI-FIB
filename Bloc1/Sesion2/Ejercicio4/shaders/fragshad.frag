#version 330 core

in vec4 gl_FragCoord;
in vec3 fcolor;
out vec4 FragColor;

void main() {
    
    
    if(gl_FragCoord .x < 355. && gl_FragCoord .y > 355.){
        FragColor = vec4(fcolor,1);
    }
    else if(gl_FragCoord .x < 355. && gl_FragCoord .y < 355.){
        FragColor = vec4(fcolor,1);
    }
    else if(gl_FragCoord .x > 355. && gl_FragCoord .y > 355.){
        FragColor = vec4(fcolor,1);
    }
    else{
        FragColor = vec4(fcolor,1);
    }
}

