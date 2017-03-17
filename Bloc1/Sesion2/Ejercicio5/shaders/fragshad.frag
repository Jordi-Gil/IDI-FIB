#version 330 core

in vec4 gl_FragCoord;
in vec3 fcolor;
out vec4 FragColor;

void main() {
    
    
    if(gl_FragCoord .x < 355. && gl_FragCoord .y > 355.){
        FragColor = vec4((fcolor*vec3(0.0,0.0,1.0)),1);
    }
    else if(gl_FragCoord .x < 355. && gl_FragCoord .y < 355.){
        FragColor = vec4((fcolor*vec3(1.0,0.0,0.0)),1);
    }
    else if(gl_FragCoord .x > 355. && gl_FragCoord .y > 355.){
        FragColor = vec4((fcolor*vec3(0.0,1.0,0.0)),1);
    }
    else{
        FragColor = vec4((fcolor*vec3(1.0,1.0,0.0)),1);
    }
}

