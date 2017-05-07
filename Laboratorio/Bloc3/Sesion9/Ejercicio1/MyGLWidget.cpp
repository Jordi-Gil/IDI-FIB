#include "MyGLWidget.h"

#include <iostream>

MyGLWidget::MyGLWidget (QWidget* parent) : QOpenGLWidget(parent)
{
    setFocusPolicy(Qt::ClickFocus);  // per rebre events de teclat
}

MyGLWidget::~MyGLWidget ()
{
    if (program != NULL)
        delete program;
}

void MyGLWidget::initializeGL ()
{
    

    x = glm::vec3(1,0,0);
    y = glm::vec3(0,1,0);
    z = glm::vec3(0,0,1);

    degreesX = 0.0;
    
    gir = false;
    song = false;
    ortogonal = true;

    scale = 1.0f;


    red = green = blue = 1.0;

    model = "Patricio";
    index = 1;

    origen = glm::vec3(0,0,0);

    one_degree = (float)M_PI/180;

    v = glm::vec3(1/sqrt(3),1/sqrt(3),1/sqrt(3));

    radiModel = 4;
    
    // Cal inicialitzar l'ús de les funcions d'OpenGL
    initializeOpenGLFunctions();
    
    glClearColor(0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
    glEnable(GL_DEPTH_TEST);
    
    carregaShaders();
    createBuffers();

    ini_camera_3a_persona();

    posFocus = glm::vec3(2,0,2);
    colFocus = glm::vec3(0.8,0.8,0.8);
    glUniform3fv(posFocusLoc, 1, &posFocus[0]);
    glUniform3fv(colorFocusLoc, 1, &colFocus[0]);
    factor = 1;
    sun = false;
    cambia = false;
    
    fvant = 85-(int)((FOV-15*(float)M_PI/180)*((float)180/M_PI));
    //Signals
    emit updateslfov(85-(int)((FOV-15*(float)M_PI/180)*((float)180/M_PI)));
    emit updateslort(-(raux/radiModel)*100 + 150);
    emit updatePers();
    emit updateOrt();
    emit updatepsi(psi*((float)180/M_PI));
    emit updatetheta(theta*((float)180/M_PI));
    emit updateEscalat((double)scale);
}

void MyGLWidget::paintGL ()
{
    // Esborrem el frame-buffer
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Carreguem la transformació de model
    if(model == "Patricio"){
        modelTransformPat1 ();
        pinta_model();

        modelTransformPat2();
        pinta_model();
    }
    else if(model == "Homer"){
        modelTransformHomer1();
        pinta_model();

        modelTransformHomer2();
        pinta_model();
    }
    else if(model == "Legoman"){
        modelTransformLego1();
        pinta_model();

        modelTransformLego2();
        pinta_model();
    }
    else if(model == "Vaca"){
        modelTransformVaca1();
        pinta_model();

        modelTransformVaca2();
        pinta_model();
    }
    else if(model == "Delfin"){
        modelTransformDelfin1();
        pinta_model();

        modelTransformDelfin2();
        pinta_model();
    }
    else if(model == "F16"){
        modelTransformF161();
        pinta_model();

        modelTransformF162();
        pinta_model();
    }
    else if(model == "Porsche"){
        modelTransformPorsche1();
        pinta_model();

        modelTransformPorsche2();
        pinta_model();
    }

    modelTransformFloor ();
    pinta_floor();

    glBindVertexArray (0);

    if(gir) gira();
    if(sun) sunshine();

}

void MyGLWidget::resizeGL (int w, int h)
{
    evitaDeformacions(w,h);
    glViewport(0, 0, w, h);
}

void MyGLWidget::createBuffers ()
{

    carregaModels();
    carregaFloor();

    calculaCapsaPatricio();
    calculaCapsaHomer();
    calculaCapsaLegoman();
    calculaCapsaVaca();
    calculaCapsaDelfin();
    calculaCapsaF16();
    calculaCapsaPorsche();

    glBindVertexArray (0);

}

void MyGLWidget::carregaShaders()
{
    // Creem els shaders per al fragment shader i el vertex shader
    QOpenGLShader fs (QOpenGLShader::Fragment, this);
    QOpenGLShader vs (QOpenGLShader::Vertex, this);
    // Carreguem el codi dels fitxers i els compilem
    fs.compileSourceFile("shaders/fragshad.frag");
    vs.compileSourceFile("shaders/vertshad.vert");
    // Creem el program
    program = new QOpenGLShaderProgram(this);
    // Li afegim els shaders corresponents
    program->addShader(&fs);
    program->addShader(&vs);
    // Linkem el program
    program->link();
    // Indiquem que aquest és el program que volem usar
    program->bind();

    // Obtenim identificador per a l'atribut “vertex” del vertex shader
    vertexLoc = glGetAttribLocation (program->programId(), "vertex");
    // Obtenim identificador per a l'atribut “normal” del vertex shader
    normalLoc = glGetAttribLocation (program->programId(), "normal");
    // Obtenim identificador per a l'atribut “matamb” del vertex shader
    matambLoc = glGetAttribLocation (program->programId(), "matamb");
    // Obtenim identificador per a l'atribut “matdiff” del vertex shader
    matdiffLoc = glGetAttribLocation (program->programId(), "matdiff");
    // Obtenim identificador per a l'atribut “matspec” del vertex shader
    matspecLoc = glGetAttribLocation (program->programId(), "matspec");
    // Obtenim identificador per a l'atribut “matshin” del vertex shader
    matshinLoc = glGetAttribLocation (program->programId(), "matshin");

    // Uniform locations
    transLoc = glGetUniformLocation(program->programId(), "TG");
    projLoc = glGetUniformLocation(program->programId(), "PM");
    viewLoc = glGetUniformLocation(program->programId(), "VM");

    posFocusLoc = glGetUniformLocation(program->programId(), "posFocus");
    colorFocusLoc = glGetUniformLocation(program->programId(), "colFocus");
}

/*
        ***********************
        *                     *
        *    MIS FUNCIONES    *
        *                     *
        ***********************

*/

/*
 ************************************
 *                                  *
 *   FUNCIONES DE CARREGA DE VAOs   *
 *                                  *
 ************************************
*/


void MyGLWidget::carregaModels(){


    //carrega el model
    models[0].load("../../models/Patricio.obj");
    models[1].load("../../models/homer.obj");
    models[2].load("../../models/legoman.obj");
    models[3].load("../../models/cow.obj");
    models[4].load("../../models/dolphin.obj");
    models[5].load("../../models/f-16.obj");
    models[6].load("../../models/porsche.obj");

     for(int i = 1; i < 8; ++i){
    // Creació del Vertex Array Object per pintar
        glGenVertexArrays(1, &Vs[i][0]);
        glBindVertexArray(Vs[i][0]);

        //Vertex
        glGenBuffers(1, &Vs[i][1]);
        glBindBuffer(GL_ARRAY_BUFFER, Vs[i][1]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*models[i-1].faces().size()*3*3, models[i-1].VBO_vertices(), GL_STATIC_DRAW);

        // Activem l'atribut vertexLoc
        glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(vertexLoc);

        //Normals
        glGenBuffers(1, &Vs[i][2]);
        glBindBuffer(GL_ARRAY_BUFFER, Vs[i][2]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*models[i-1].faces().size()*3*3, models[i-1].VBO_normals(), GL_STATIC_DRAW);

        // Activem l'atribut normalLoc
        glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(normalLoc);

        //Normals
        glGenBuffers(1, &Vs[i][3]);
        glBindBuffer(GL_ARRAY_BUFFER, Vs[i][3]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*models[i-1].faces().size()*3*3, models[i-1].VBO_matamb(), GL_STATIC_DRAW);

        // Activem l'atribut matambLoc
        glVertexAttribPointer(matambLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(matambLoc);

        glGenBuffers(1, &Vs[i][4]);
        glBindBuffer(GL_ARRAY_BUFFER, Vs[i][4]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*models[i-1].faces().size()*3*3, models[i-1].VBO_matdiff(), GL_STATIC_DRAW);

        // Activem l'atribut matdiffLoc
        glVertexAttribPointer(matdiffLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(matdiffLoc);

         // Buffer de component especular
        glGenBuffers(1, &Vs[i][5]);
        glBindBuffer(GL_ARRAY_BUFFER, Vs[i][5]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*models[i-1].faces().size()*3*3, models[i-1].VBO_matspec(), GL_STATIC_DRAW);

        glVertexAttribPointer(matspecLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(matspecLoc);

        // Buffer de component shininness
        glGenBuffers(1, &Vs[i][6]);
        glBindBuffer(GL_ARRAY_BUFFER, Vs[i][6]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*models[i-1].faces().size()*3, models[i-1].VBO_matshin(), GL_STATIC_DRAW);

        glVertexAttribPointer(matshinLoc, 1, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(matshinLoc);
    }


}

//Carrega el terra
void MyGLWidget::carregaFloor(){

    // Dades del terra
    // VBO amb la posició dels vèrtexs
    glm::vec3 posterra[12] = {
        glm::vec3(-2.0, -2.0, 2.0),
        glm::vec3(2.0, -2.0, 2.0),
        glm::vec3(-2.0, -2.0, -2.0),
        glm::vec3(-2.0, -2.0, -2.0),
        glm::vec3(2.0, -2.0, 2.0),
        glm::vec3(2.0, -2.0, -2.0),
        glm::vec3(-2.0, -2.0, -2.0),
        glm::vec3(2.0, -2.0, -2.0),
        glm::vec3(-2.0, 2.0, -2.0),
        glm::vec3(-2.0, 2.0, -2.0),
        glm::vec3(2.0, -2.0, -2.0),
        glm::vec3(2.0, 2.0, -2.0)
    };

    // VBO amb la normal de cada vèrtex
    glm::vec3 norm1 (0,1,0);
    glm::vec3 norm2 (0,0,1);
    glm::vec3 normterra[12] = {
        norm1, norm1, norm1, norm1, norm1, norm1, // la normal (0,1,0) per als primers dos triangles
        norm2, norm2, norm2, norm2, norm2, norm2  // la normal (0,0,1) per als dos últims triangles
    };

    // Definim el material del terra
    glm::vec3 amb(0,0,0.2);
    glm::vec3 diff(0,0,0.8);
    glm::vec3 spec(0,0,1);
    float shin = 100;

    // Fem que aquest material afecti a tots els vèrtexs per igual
    glm::vec3 matambterra[12] = {
        amb, amb, amb, amb, amb, amb, amb, amb, amb, amb, amb, amb
    };
    glm::vec3 matdiffterra[12] = {
        diff, diff, diff, diff, diff, diff, diff, diff, diff, diff, diff, diff
    };
    glm::vec3 matspecterra[12] = {
        spec, spec, spec, spec, spec, spec, spec, spec, spec, spec, spec, spec
    };
    float matshinterra[12] = {
        shin, shin, shin, shin, shin, shin, shin, shin, shin, shin, shin, shin
    };

    // Creació del Vertex Array Object del terra
    glGenVertexArrays(1, &Vs[0][0]);
    glBindVertexArray(Vs[0][0]);

    glGenBuffers(1, &Vs[0][1]);
    glBindBuffer(GL_ARRAY_BUFFER, Vs[0][1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(posterra), posterra, GL_STATIC_DRAW);

    // Activem l'atribut vertexLoc
    glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexLoc);

    glGenBuffers(1, &Vs[0][2]);
    glBindBuffer(GL_ARRAY_BUFFER, Vs[0][2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(normterra), normterra, GL_STATIC_DRAW);

    // Activem l'atribut normalLoc
    glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(normalLoc);

    // En lloc del color, ara passem tots els paràmetres dels materials
    // Buffer de component ambient
    glGenBuffers(1, &Vs[0][3]);
    glBindBuffer(GL_ARRAY_BUFFER, Vs[0][3]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(matambterra), matambterra, GL_STATIC_DRAW);

    glVertexAttribPointer(matambLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(matambLoc);

    // Buffer de component difusa
    glGenBuffers(1, &Vs[0][4]);
    glBindBuffer(GL_ARRAY_BUFFER, Vs[0][4]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(matdiffterra), matdiffterra, GL_STATIC_DRAW);

    glVertexAttribPointer(matdiffLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(matdiffLoc);

    // Buffer de component especular
    glGenBuffers(1, &Vs[0][5]);
    glBindBuffer(GL_ARRAY_BUFFER, Vs[0][5]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(matspecterra), matspecterra, GL_STATIC_DRAW);

    glVertexAttribPointer(matspecLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(matspecLoc);

    // Buffer de component shininness
    glGenBuffers(1, &Vs[0][6]);
    glBindBuffer(GL_ARRAY_BUFFER, Vs[0][6]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(matshinterra), matshinterra, GL_STATIC_DRAW);

    glVertexAttribPointer(matshinLoc, 1, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(matshinLoc);

}

/*
 ********************************
 *                              *
 *   FUNCIONES DE PINTAR VAOs   *
 *                              *
 ********************************
*/

void MyGLWidget::pinta_model()
{
    // Activem el VAO per a pintar el homer
    glBindVertexArray (Vs[index][0]);
    // pintem
    glDrawArrays(GL_TRIANGLES, 0, models[index-1].faces().size()*3);
}

void MyGLWidget::pinta_floor()
{
    // Activem el VAO per a pintar el floor
    glBindVertexArray(Vs[0][0]);

    //pintem
    glDrawArrays(GL_TRIANGLES, 0, 12);
}

/*
 *****************************
 *                           *
 *   FUNCIONES DE CALCULO    *
 *                           *
 *****************************
*/

//Patricio
void MyGLWidget::calculaCapsaPatricio()
{
    EsferaMinimaPat.x = EsferaMaximaPat.x = models[0].vertices()[0];
    EsferaMinimaPat.y = EsferaMaximaPat.y = models[0].vertices()[1];
    EsferaMinimaPat.z = EsferaMaximaPat.z = models[0].vertices()[2];

    for(unsigned int i = 3; i < models[0].vertices().size(); i+=3){
        //Comprovamos la componente 'x'
        EsferaMinimaPat.x = std::min(EsferaMinimaPat.x,(float) models[0].vertices()[i]);
        EsferaMaximaPat.x = std::max(EsferaMaximaPat.x,(float) models[0].vertices()[i]);

        //Comprovamos la componente 'y'
        EsferaMinimaPat.y = std::min(EsferaMinimaPat.y,(float) models[0].vertices()[i+1]);
        EsferaMaximaPat.y = std::max(EsferaMaximaPat.y,(float) models[0].vertices()[i+1]);

        //Comprovamos la componente 'z'
        EsferaMinimaPat.z = std::min(EsferaMinimaPat.z,(float) models[0].vertices()[i+2]);
        EsferaMaximaPat.z = std::max(EsferaMaximaPat.z,(float) models[0].vertices()[i+2]);

    }
    //Abansa de recalcular els vertex de l'esfera amb el terra, calculem l'escalat amb la capsa del patricio
    scalePat1 = 2/(EsferaMaximaPat.y - EsferaMinimaPat.y);
    CBMPat = glm::vec3((EsferaMaximaPat.x + EsferaMinimaPat.x)/2, EsferaMinimaPat.y, (EsferaMaximaPat.z + EsferaMinimaPat.z)/2 );


    calculaCentreModelPat();

}

void MyGLWidget::calculaCentreModelPat()
{
    centreModelPat = glm::vec3((EsferaMaximaPat.x + EsferaMinimaPat.x)/2, (EsferaMaximaPat.y + EsferaMinimaPat.y)/2, (EsferaMaximaPat.z + EsferaMinimaPat.z)/2 );

    // r = √((x2 - x1)^2 + (y2 - y1)^2 + (z2 - z1)^2)/2
    float auxX = pow((EsferaMaximaPat.x - EsferaMinimaPat.x),2);
    float auxY = pow((EsferaMaximaPat.y - EsferaMinimaPat.y),2);
    float auxZ = pow((EsferaMaximaPat.z - EsferaMinimaPat.z),2);

    radiModelPat = sqrt(auxX + auxY + auxZ);

}

//Homer
void MyGLWidget::calculaCapsaHomer()
{
    EsferaMinimaHomer.x = EsferaMaximaHomer.x = models[1].vertices()[0];
    EsferaMinimaHomer.y = EsferaMaximaHomer.y = models[1].vertices()[1];
    EsferaMinimaHomer.z = EsferaMaximaHomer.z = models[1].vertices()[2];

    for(unsigned int i = 3; i < models[1].vertices().size(); i+=3){
        //Comprovamos la componente 'x'
        EsferaMinimaHomer.x = std::min(EsferaMinimaHomer.x,(float) models[1].vertices()[i]);
        EsferaMaximaHomer.x = std::max(EsferaMaximaHomer.x,(float) models[1].vertices()[i]);

        //Comprovamos la componente 'y'
        EsferaMinimaHomer.y = std::min(EsferaMinimaHomer.y,(float) models[1].vertices()[i+1]);
        EsferaMaximaHomer.y = std::max(EsferaMaximaHomer.y,(float) models[1].vertices()[i+1]);

        //Comprovamos la componente 'z'
        EsferaMinimaHomer.z = std::min(EsferaMinimaHomer.z,(float) models[1].vertices()[i+2]);
        EsferaMaximaHomer.z = std::max(EsferaMaximaHomer.z,(float) models[1].vertices()[i+2]);

    }
    //Abansa de recalcular els vertex de l'esfera amb el terra, calculem l'escalat amb la capsa del homer
    scaleHomer = 2/(EsferaMaximaHomer.y - EsferaMinimaHomer.y);
    CBMHomer = glm::vec3((EsferaMaximaHomer.x + EsferaMinimaHomer.x)/2, EsferaMinimaHomer.y, (EsferaMaximaHomer.z + EsferaMinimaHomer.z)/2 );

    calculaCentreModelHomer();

}

void MyGLWidget::calculaCentreModelHomer()
{
    centreModelHomer = glm::vec3((EsferaMaximaHomer.x + EsferaMinimaHomer.x)/2, (EsferaMaximaHomer.y + EsferaMinimaHomer.y)/2, (EsferaMaximaHomer.z + EsferaMinimaHomer.z)/2 );

    // r = √((x2 - x1)^2 + (y2 - y1)^2 + (z2 - z1)^2)/2
    float auxX = pow((EsferaMaximaHomer.x - EsferaMinimaHomer.x),2);
    float auxY = pow((EsferaMaximaHomer.y - EsferaMinimaHomer.y),2);
    float auxZ = pow((EsferaMaximaHomer.z - EsferaMinimaHomer.z),2);

    radiModelHomer = sqrt(auxX + auxY + auxZ);

}

//Legoman
void MyGLWidget::calculaCapsaLegoman()
{

    EsferaMinimaLego.x = EsferaMaximaLego.x = models[2].vertices()[0];
    EsferaMinimaLego.y = EsferaMaximaLego.y = models[2].vertices()[1];
    EsferaMinimaLego.z = EsferaMaximaLego.z = models[2].vertices()[2];


    for(unsigned int i = 3; i < models[2].vertices().size(); i+=3){
        //Comprovamos la componente 'x'
        EsferaMinimaLego.x = std::min(EsferaMinimaLego.x,(float) models[2].vertices()[i]);
        EsferaMaximaLego.x = std::max(EsferaMaximaLego.x,(float) models[2].vertices()[i]);

        //Comprovamos la componente 'y'
        EsferaMinimaLego.y = std::min(EsferaMinimaLego.y,(float) models[2].vertices()[i+1]);
        EsferaMaximaLego.y = std::max(EsferaMaximaLego.y,(float) models[2].vertices()[i+1]);

        //Comprovamos la componente 'z'
        EsferaMinimaLego.z = std::min(EsferaMinimaLego.z,(float) models[2].vertices()[i+2]);
        EsferaMaximaLego.z = std::max(EsferaMaximaLego.z,(float) models[2].vertices()[i+2]);

    }

    //Abansa de recalcular els vertex de l'esfera amb el terra, calculem l'escalat amb la capsa del legomans
    scaleLego = 2/(EsferaMaximaLego.y - EsferaMinimaLego.y);
    CBMLego = glm::vec3((EsferaMaximaLego.x + EsferaMinimaLego.x)/2, EsferaMinimaLego.y, (EsferaMaximaLego.z + EsferaMinimaLego.z)/2 );

    calculaCentreModelLego();

}

void MyGLWidget::calculaCentreModelLego()
{
    centreModelLego = glm::vec3((EsferaMaximaLego.x + EsferaMinimaLego.x)/2, (EsferaMaximaLego.y + EsferaMinimaLego.y)/2, (EsferaMaximaLego.z + EsferaMinimaLego.z)/2 );

    // r = √((x2 - x1)^2 + (y2 - y1)^2 + (z2 - z1)^2)/2
    float auxX = pow((EsferaMaximaLego.x - EsferaMinimaLego.x),2);
    float auxY = pow((EsferaMaximaLego.y - EsferaMinimaLego.y),2);
    float auxZ = pow((EsferaMaximaLego.z - EsferaMinimaLego.z),2);

    radiModelLego = sqrt(auxX + auxY + auxZ);

}

//Vaca
void MyGLWidget::calculaCapsaVaca()
{

    EsferaMinimaVaca.x = EsferaMaximaVaca.x = models[3].vertices()[0];
    EsferaMinimaVaca.y = EsferaMaximaVaca.y = models[3].vertices()[1];
    EsferaMinimaVaca.z = EsferaMaximaVaca.z = models[3].vertices()[2];


    for(unsigned int i = 3; i < models[3].vertices().size(); i+=3){
        //Comprovamos la componente 'x'
        EsferaMinimaVaca.x = std::min(EsferaMinimaVaca.x,(float) models[3].vertices()[i]);
        EsferaMaximaVaca.x = std::max(EsferaMaximaVaca.x,(float) models[3].vertices()[i]);

        //Comprovamos la componente 'y'
        EsferaMinimaVaca.y = std::min(EsferaMinimaVaca.y,(float) models[3].vertices()[i+1]);
        EsferaMaximaVaca.y = std::max(EsferaMaximaVaca.y,(float) models[3].vertices()[i+1]);

        //Comprovamos la componente 'z'
        EsferaMinimaVaca.z = std::min(EsferaMinimaVaca.z,(float) models[3].vertices()[i+2]);
        EsferaMaximaVaca.z = std::max(EsferaMaximaVaca.z,(float) models[3].vertices()[i+2]);

    }

    //Abansa de recalcular els vertex de l'esfera amb el terra, calculem l'escalat amb la capsa del vacas
    scaleVaca = 2/(EsferaMaximaVaca.y - EsferaMinimaVaca.y);
    CBMVaca = glm::vec3((EsferaMaximaVaca.x + EsferaMinimaVaca.x)/2, EsferaMinimaVaca.y, (EsferaMaximaVaca.z + EsferaMinimaVaca.z)/2 );

    calculaCentreModelVaca();

}

void MyGLWidget::calculaCentreModelVaca()
{
    centreModelVaca = glm::vec3((EsferaMaximaVaca.x + EsferaMinimaVaca.x)/2, (EsferaMaximaVaca.y + EsferaMinimaVaca.y)/2, (EsferaMaximaVaca.z + EsferaMinimaVaca.z)/2 );

    // r = √((x2 - x1)^2 + (y2 - y1)^2 + (z2 - z1)^2)/2
    float auxX = pow((EsferaMaximaVaca.x - EsferaMinimaVaca.x),2);
    float auxY = pow((EsferaMaximaVaca.y - EsferaMinimaVaca.y),2);
    float auxZ = pow((EsferaMaximaVaca.z - EsferaMinimaVaca.z),2);

    radiModelVaca = sqrt(auxX + auxY + auxZ);

}

void MyGLWidget::calculaCapsaDelfin()
{

    EsferaMinimaDelfin.x = EsferaMaximaDelfin.x = models[4].vertices()[0];
    EsferaMinimaDelfin.y = EsferaMaximaDelfin.y = models[4].vertices()[1];
    EsferaMinimaDelfin.z = EsferaMaximaDelfin.z = models[4].vertices()[2];


    for(unsigned int i = 3; i < models[4].vertices().size(); i+=3){
        //Comprovamos la componente 'x'
        EsferaMinimaDelfin.x = std::min(EsferaMinimaDelfin.x,(float) models[4].vertices()[i]);
        EsferaMaximaDelfin.x = std::max(EsferaMaximaDelfin.x,(float) models[4].vertices()[i]);

        //Comprovamos la componente 'y'
        EsferaMinimaDelfin.y = std::min(EsferaMinimaDelfin.y,(float) models[4].vertices()[i+1]);
        EsferaMaximaDelfin.y = std::max(EsferaMaximaDelfin.y,(float) models[4].vertices()[i+1]);

        //Comprovamos la componente 'z'
        EsferaMinimaDelfin.z = std::min(EsferaMinimaDelfin.z,(float) models[4].vertices()[i+2]);
        EsferaMaximaDelfin.z = std::max(EsferaMaximaDelfin.z,(float) models[4].vertices()[i+2]);

    }

    //Abansa de recalcular els vertex de l'esfera amb el terra, calculem l'escalat amb la capsa del vacas
    scaleDelfin = 2/(EsferaMaximaDelfin.y - EsferaMinimaDelfin.y);
    CBMDelfin = glm::vec3((EsferaMaximaDelfin.x + EsferaMinimaDelfin.x)/2, EsferaMinimaDelfin.y, (EsferaMaximaDelfin.z + EsferaMinimaDelfin.z)/2 );

    calculaCentreModelDelfin();
}

void MyGLWidget::calculaCentreModelDelfin()
{
    centreModelDelfin = glm::vec3((EsferaMaximaDelfin.x + EsferaMinimaDelfin.x)/2, (EsferaMaximaDelfin.y + EsferaMinimaDelfin.y)/2, (EsferaMaximaDelfin.z + EsferaMinimaDelfin.z)/2 );

    // r = √((x2 - x1)^2 + (y2 - y1)^2 + (z2 - z1)^2)/2
    float auxX = pow((EsferaMaximaDelfin.x - EsferaMinimaDelfin.x),2);
    float auxY = pow((EsferaMaximaDelfin.y - EsferaMinimaDelfin.y),2);
    float auxZ = pow((EsferaMaximaDelfin.z - EsferaMinimaDelfin.z),2);

    radiModelDelfin = sqrt(auxX + auxY + auxZ);

}

//F16
void MyGLWidget::calculaCapsaF16()
{

    EsferaMinimaF16.x = EsferaMaximaF16.x = models[5].vertices()[0];
    EsferaMinimaF16.y = EsferaMaximaF16.y = models[5].vertices()[1];
    EsferaMinimaF16.z = EsferaMaximaF16.z = models[5].vertices()[2];


    for(unsigned int i = 3; i < models[5].vertices().size(); i+=3){
        //Comprovamos la componente 'x'
        EsferaMinimaF16.x = std::min(EsferaMinimaF16.x,(float) models[5].vertices()[i]);
        EsferaMaximaF16.x = std::max(EsferaMaximaF16.x,(float) models[5].vertices()[i]);

        //Comprovamos la componente 'y'
        EsferaMinimaF16.y = std::min(EsferaMinimaF16.y,(float) models[5].vertices()[i+1]);
        EsferaMaximaF16.y = std::max(EsferaMaximaF16.y,(float) models[5].vertices()[i+1]);

        //Comprovamos la componente 'z'
        EsferaMinimaF16.z = std::min(EsferaMinimaF16.z,(float) models[5].vertices()[i+2]);
        EsferaMaximaF16.z = std::max(EsferaMaximaF16.z,(float) models[5].vertices()[i+2]);

    }

    //Abansa de recalcular els vertex de l'esfera amb el terra, calculem l'escalat amb la capsa del vacas
    scaleF16 = 2/(EsferaMaximaF16.y - EsferaMinimaF16.y);
    CBMF16 = glm::vec3((EsferaMaximaF16.x + EsferaMinimaF16.x)/2, EsferaMinimaF16.y, (EsferaMaximaF16.z + EsferaMinimaF16.z)/2 );

    calculaCentreModelF16();

}

void MyGLWidget::calculaCentreModelF16()
{
    centreModelF16 = glm::vec3((EsferaMaximaF16.x + EsferaMinimaF16.x)/2, (EsferaMaximaF16.y + EsferaMinimaF16.y)/2, (EsferaMaximaF16.z + EsferaMinimaF16.z)/2 );

    // r = √((x2 - x1)^2 + (y2 - y1)^2 + (z2 - z1)^2)/2
    float auxX = pow((EsferaMaximaF16.x - EsferaMinimaF16.x),2);
    float auxY = pow((EsferaMaximaF16.y - EsferaMinimaF16.y),2);
    float auxZ = pow((EsferaMaximaF16.z - EsferaMinimaF16.z),2);

    radiModelF16 = sqrt(auxX + auxY + auxZ);

}

//Porsche
void MyGLWidget::calculaCapsaPorsche()
{

    EsferaMinimaPorsche.x = EsferaMaximaPorsche.x = models[6].vertices()[0];
    EsferaMinimaPorsche.y = EsferaMaximaPorsche.y = models[6].vertices()[1];
    EsferaMinimaPorsche.z = EsferaMaximaPorsche.z = models[6].vertices()[2];


    for(unsigned int i = 3; i < models[6].vertices().size(); i+=3){
        //Comprovamos la componente 'x'
        EsferaMinimaPorsche.x = std::min(EsferaMinimaPorsche.x,(float) models[6].vertices()[i]);
        EsferaMaximaPorsche.x = std::max(EsferaMaximaPorsche.x,(float) models[6].vertices()[i]);

        //Comprovamos la componente 'y'
        EsferaMinimaPorsche.y = std::min(EsferaMinimaPorsche.y,(float) models[6].vertices()[i+1]);
        EsferaMaximaPorsche.y = std::max(EsferaMaximaPorsche.y,(float) models[6].vertices()[i+1]);

        //Comprovamos la componente 'z'
        EsferaMinimaPorsche.z = std::min(EsferaMinimaPorsche.z,(float) models[6].vertices()[i+2]);
        EsferaMaximaPorsche.z = std::max(EsferaMaximaPorsche.z,(float) models[6].vertices()[i+2]);

    }

    //Abansa de recalcular els vertex de l'esfera amb el terra, calculem l'escalat amb la capsa del vacas
    scalePorsche = 2/(EsferaMaximaPorsche.y - EsferaMinimaPorsche.y);
    CBMPorsche = glm::vec3((EsferaMaximaPorsche.x + EsferaMinimaPorsche.x)/2, EsferaMinimaPorsche.y, (EsferaMaximaPorsche.z + EsferaMinimaPorsche.z)/2 );

    calculaCentreModelPorsche();

}

void MyGLWidget::calculaCentreModelPorsche()
{
    centreModelPorsche = glm::vec3((EsferaMaximaPorsche.x + EsferaMinimaPorsche.x)/2, (EsferaMaximaPorsche.y + EsferaMinimaPorsche.y)/2, (EsferaMaximaPorsche.z + EsferaMinimaPorsche.z)/2 );

    // r = √((x2 - x1)^2 + (y2 - y1)^2 + (z2 - z1)^2)/2
    float auxX = pow((EsferaMaximaPorsche.x - EsferaMinimaPorsche.x),2);
    float auxY = pow((EsferaMaximaPorsche.y - EsferaMinimaPorsche.y),2);
    float auxZ = pow((EsferaMaximaPorsche.z - EsferaMinimaPorsche.z),2);

    radiModelPorsche = sqrt(auxX + auxY + auxZ);

}

/*
 ********************************
 *                              *
 *   FUNCIONES DE INTERACCIÓN   *
 *                              *
 ********************************
*/

void MyGLWidget::wheelEvent (QWheelEvent * e)
{
    makeCurrent();


    if(e->delta() > 0.f){

        FOVini -= one_degree;
        FOV -= one_degree;
        
        laux += radiModel/100;
        raux -= radiModel/100;
        baux += radiModel/100;
        taux -= radiModel/100;

    }
    else
    {
        FOVini += one_degree;
        FOV += one_degree;
        
        raux += radiModel/100;
        laux -= radiModel/100;
        baux -= radiModel/100;
        taux += radiModel/100;
    }
    
    if (laux > -radiModel/2) laux = -radiModel/2;
    else if (laux < -1.5*radiModel) laux = -1.5*radiModel;
    if (raux < radiModel/2) raux = radiModel/2;
    else if (raux > 1.5*radiModel) raux = 1.5*radiModel;
    if (taux < radiModel/2) taux = radiModel/2;
    else if (taux > 1.5*radiModel) taux = 1.5*radiModel;
    if (baux > -radiModel/2) baux = -radiModel/2;
    else if (baux < -1.5*radiModel) baux = -1.5*radiModel;
    
    resizeOrto();

    if(FOVini < 15*one_degree) FOVini = 15*one_degree;
    else if(FOVini > 100*one_degree) FOVini = 100*one_degree;

    if(FOV < 15*one_degree) FOV = 15*one_degree;
    else if(FOV > 100*one_degree) FOV = 100*one_degree;
    
    emit updateslfov(85-(int)((FOV-15*(float)M_PI/180)*((float)180/M_PI)));
    emit updateslort(-(raux/radiModel)*100 + 150);
    if(!ortogonal) projectTransformPerspectiva();
    else projectTransformOrtogonal();

    update();
}

void MyGLWidget::mousePressEvent(QMouseEvent *e)
{
    if(e->buttons() & Qt::LeftButton && ! ( e->modifiers() & ( Qt::ShiftModifier | Qt::AltModifier | Qt::ControlModifier ))){
        position = e->pos();
    }
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *e)
{
    makeCurrent();
    if(e->buttons() & Qt::LeftButton && ! ( e->modifiers() & ( Qt::ShiftModifier | Qt::AltModifier | Qt::ControlModifier ))) {
        
        if(e->x() > position.x()) psi += 2*(float)(M_PI/180);
        else if(e->x() < position.x()) psi -= 2*(float)(M_PI/180);
        
        if(e->y() > position.y()) theta += 2*(float)(M_PI/180);
        else if(e->y() < position.y()) theta -= 2*(float)(M_PI/180);
        position = e->pos();
        
        emit updatetheta((int)(theta*((float)180/M_PI))%360);
        emit updatepsi((int)(psi*((float)180/M_PI))%360);
        
        viewTransform();
        update();
    }
}

void MyGLWidget::keyPressEvent(QKeyEvent* event)
{
    makeCurrent();
    switch (event->key()) {
        case Qt::Key_S: { // escalar a més gran
            scale += 0.05;
            emit updateEscalat(scale);
            break;
        }
        case Qt::Key_D: { // escalar a més petit
            scale -= 0.05;
            emit updateEscalat(scale);
            break;
        }
        case Qt::Key_G: {
            gir = !gir;
            break;
        }
        case Qt::Key_M: {
            if(gir) {
                if(!song){
                    player.play();
                    song = true;
                }
                else{
                    player.stop();
                    song = false;
                }
            }
            break;
        }
        case Qt::Key_B: {
            if(sun) sun = false;
            else sun = true;
        }
        case Qt::Key_C: {
            if(cambia) cambia = false;
            else cambia = true;
        }
        case Qt::Key_R: {
            if(!sun) restart();
        }
        default: event->ignore(); break;
    }
    update();
}

/*
 *****************************
 *                           *
 *   FUNCIONES DE CÁMARA     *
 *                           *
 *****************************
*/

void MyGLWidget::projectTransformPerspectiva(){
    ortogonal = false;
    //FOV, ra, znear, zfar
    glm::mat4 Proj = glm::perspective(FOV,ra,znear,zfar);
    glUniformMatrix4fv(projLoc,1,GL_FALSE,&Proj[0][0]);
}

void MyGLWidget::projectTransformOrtogonal()
{
    ortogonal = true;
    glm::mat4 Proj = glm::ortho(left, right, bottom, top, znear, zfar);
    glUniformMatrix4fv(projLoc,1,GL_FALSE,&Proj[0][0]);
}

void MyGLWidget::viewTransform(){

    // glm::mat4 ViewMat = glm::lookAt(OBS,VRP,UP);
    glm::mat4 ViewMat (1.0f);
    ViewMat = glm::translate(ViewMat,glm::vec3(0.f,0.f,-d));
    ViewMat = glm::rotate(ViewMat,-fi, z);
    ViewMat = glm::rotate(ViewMat,theta,x);
    ViewMat = glm::rotate(ViewMat,-psi,y);
    ViewMat = glm::translate(ViewMat,-VRP);
    glUniformMatrix4fv(viewLoc,1,GL_FALSE,&ViewMat[0][0]);
}

void MyGLWidget::ini_camera_3a_persona()
{
    d = 2*radiModel;
    VRP = origen;
    FOV = FOVini = 2*asin(radiModel/d);
    
    ra = double(width())/double(height());

    
    left = laux = -radiModel;
    right = raux = radiModel;
    bottom = baux = -radiModel;
    top = taux = radiModel;
    
    znear = d - radiModel;
    zfar = d + radiModel;
    
    theta = (float)M_PI/4;
    psi = fi = 0;

    if(ortogonal) projectTransformOrtogonal();
    else projectTransformPerspectiva();

    viewTransform();

}

/*
 ***********************************
 *                                 *
 *   FUNCIONES DE TRANSFORMACIÓN   *
 *                                 *
 ***********************************
*/

void MyGLWidget::modelTransformPat1()
{
    // Matriu de transformació de model
    glm::mat4 transform (1.0f);
    transform = glm::scale(transform, glm::vec3(scale));
    transform = glm::translate(transform, glm::vec3(1.0,-2.0,1.0));
    transform = glm::scale(transform, glm::vec3(scalePat1));

    transform = glm::rotate(transform,degreesX,y);

    transform = glm::translate(transform,-CBMPat);

    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::modelTransformPat2()
{
    // Matriu de transformació de model
    glm::mat4 transform (1.0f);
    transform = glm::scale(transform, glm::vec3(scale));
    transform = glm::translate(transform, glm::vec3(-1.0,-2.0,-1.0));
    transform = glm::scale(transform, glm::vec3(scalePat1));

    transform = glm::rotate(transform,degreesX,y);
    transform = glm::rotate(transform,(float)M_PI,y);

    transform = glm::translate(transform,-CBMPat);

    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::modelTransformHomer1()
{
    // Matriu de transformació de model
    glm::mat4 transform (1.0f);
    transform = glm::scale(transform, glm::vec3(scale));
    transform = glm::translate(transform, glm::vec3(1.0,-2.0,1.0));
    transform = glm::scale(transform, glm::vec3(scaleHomer));

    transform = glm::rotate(transform,degreesX,y);
    transform = glm::rotate(transform, (float)M_PI/2, y);

    transform = glm::translate(transform,-CBMHomer);

    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::modelTransformHomer2()
{
    // Matriu de transformació de model
    glm::mat4 transform (1.0f);
    transform = glm::scale(transform, glm::vec3(scale));
    transform = glm::translate(transform, glm::vec3(-1.0,-2.0,-1.0));
    transform = glm::scale(transform, glm::vec3(scaleHomer));

    transform = glm::rotate(transform,degreesX,y);
    transform = glm::rotate(transform,(float)M_PI,y);
    transform = glm::rotate(transform, (float)M_PI/2, y);

    transform = glm::translate(transform,-CBMHomer);

    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::modelTransformLego1()
{
    // Matriu de transformació de model
    glm::mat4 transform (1.0f);
    transform = glm::scale(transform, glm::vec3(scale));
    transform = glm::translate(transform, glm::vec3(1.0,-2.0,1.0));
    transform = glm::scale(transform, glm::vec3(scaleLego));

    transform = glm::rotate(transform,degreesX,y);

    transform = glm::translate(transform,-CBMLego);

    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::modelTransformLego2()
{
    // Matriu de transformació de model
    glm::mat4 transform (1.0f);
    transform = glm::scale(transform, glm::vec3(scale));
    transform = glm::translate(transform, glm::vec3(-1.0,-2.0,-1.0));
    transform = glm::scale(transform, glm::vec3(scaleLego));

    transform = glm::rotate(transform,degreesX,y);
    transform = glm::rotate(transform,(float)M_PI,y);

    transform = glm::translate(transform,-CBMLego);

    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::modelTransformVaca1()
{
    // Matriu de transformació de model
    glm::mat4 transform (1.0f);
    transform = glm::scale(transform, glm::vec3(scale));
    transform = glm::translate(transform, glm::vec3(1.0,-2.0,1.0));

    transform = glm::scale(transform, glm::vec3(scaleVaca));
    transform = glm::scale(transform, glm::vec3(0.5));

    transform = glm::rotate(transform,degreesX,y);

    transform = glm::translate(transform,-CBMVaca);
    transform = glm::translate(transform,glm::vec3(0,-EsferaMinimaVaca.y,0));

    //Colocar "Bien"
    transform = glm::rotate(transform,(float)-M_PI/2,y);
    transform = glm::rotate(transform,(float)-M_PI/2,x);

    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::modelTransformVaca2()
{
    // Matriu de transformació de model
    glm::mat4 transform (1.0f);
    transform = glm::scale(transform, glm::vec3(scale));
    transform = glm::translate(transform, glm::vec3(-1.0,-2.0,-1.0));

    transform = glm::scale(transform, glm::vec3(scaleVaca));
    transform = glm::scale(transform, glm::vec3(0.5));

    transform = glm::rotate(transform,degreesX,y);
    transform = glm::rotate(transform,(float)M_PI,y);


    transform = glm::translate(transform,-CBMVaca);
    transform = glm::translate(transform,glm::vec3(0,-EsferaMinimaVaca.y,0));

    //Colocar "Bien"
    transform = glm::rotate(transform,(float)-M_PI/2,y);
    transform = glm::rotate(transform,(float)-M_PI/2,x);

    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::modelTransformDelfin1()
{
    // Matriu de transformació de model
    glm::mat4 transform (1.0f);
    transform = glm::scale(transform, glm::vec3(scale));
    transform = glm::translate(transform, glm::vec3(1.0,-2.0,1.0));

    transform = glm::scale(transform, glm::vec3(scaleDelfin));

    transform = glm::rotate(transform,degreesX,y);

    transform = glm::translate(transform,-CBMDelfin);

    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::modelTransformDelfin2()
{
    // Matriu de transformació de model
    glm::mat4 transform (1.0f);
    transform = glm::scale(transform, glm::vec3(scale));
    transform = glm::translate(transform, glm::vec3(-1.0,-2.0,-1.0));

    transform = glm::scale(transform, glm::vec3(scaleDelfin));

    transform = glm::rotate(transform,degreesX,y);
    transform = glm::rotate(transform,(float)M_PI,y);

    transform = glm::translate(transform,-CBMDelfin);

    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::modelTransformF161()
{
    // Matriu de transformació de model
    glm::mat4 transform (1.0f);
    transform = glm::scale(transform, glm::vec3(scale));
    transform = glm::translate(transform, glm::vec3(1.0,-2.0,1.0));

    transform = glm::scale(transform, glm::vec3(scaleF16));
    transform = glm::scale(transform, glm::vec3(0.5));

    transform = glm::rotate(transform,degreesX,y);

    transform = glm::translate(transform,-CBMF16);

    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::modelTransformF162()
{
    // Matriu de transformació de model
    glm::mat4 transform (1.0f);
    transform = glm::scale(transform, glm::vec3(scale));
    transform = glm::translate(transform, glm::vec3(-1.0,-2.0,-1.0));

    transform = glm::scale(transform, glm::vec3(scaleF16));
    transform = glm::scale(transform, glm::vec3(0.5));

    transform = glm::rotate(transform,degreesX,y);
    transform = glm::rotate(transform,(float)M_PI,y);

    transform = glm::translate(transform,-CBMF16);

    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::modelTransformPorsche1()
{
    // Matriu de transformació de model
    glm::mat4 transform (1.0f);
    transform = glm::scale(transform, glm::vec3(scale));
    transform = glm::translate(transform, glm::vec3(1.0,-2.0,1.0));

    transform = glm::scale(transform, glm::vec3(scalePorsche));
    transform = glm::scale(transform, glm::vec3(0.5));

    transform = glm::rotate(transform,degreesX,y);

    transform = glm::translate(transform,-CBMPorsche);

    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::modelTransformPorsche2()
{
    // Matriu de transformació de model
    glm::mat4 transform (1.0f);
    transform = glm::scale(transform, glm::vec3(scale));
    transform = glm::translate(transform, glm::vec3(-1.0,-2.0,-1.0));

    transform = glm::scale(transform, glm::vec3(scalePorsche));
    transform = glm::scale(transform, glm::vec3(0.5));

    transform = glm::rotate(transform,degreesX,y);
    transform = glm::rotate(transform,(float)M_PI,y);

    transform = glm::translate(transform,-CBMPorsche);

    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::modelTransformFloor()
{
    glm::mat4 transform (1.0f);
    transform = glm::scale(transform, glm::vec3(scale));
    glUniformMatrix4fv(transLoc,1,GL_FALSE, &transform[0][0]);
}

void MyGLWidget::evitaDeformacions(int w, int h)
{
    ra = float(w)/h;
    if(ra < 1.0f) FOV = 2*atan(tan(FOVini/2)/ra);

    if(ortogonal){
        resizeOrto();
        projectTransformOrtogonal();
    }
    else projectTransformPerspectiva();

}

void MyGLWidget::resizeOrto()
{
    if(ra > 1.0){
        left = laux * ra;
        right = raux * ra;
        bottom = baux;
        top = taux;
    }
    if(ra < 1.0){
        left = laux;
        right = raux;
        top = taux/ra;
        bottom = baux/ra;
    }
    projectTransformOrtogonal();
}

/*
 *************************
 *                       *
 *    FUNCIONES RANDOM   *
 *                       *
 * ***********************
 */

void MyGLWidget::gira()
{
    makeCurrent();
    for(long long int i = 1; i > 0; ++i)
        for(long long int j = 1; j > 0; ++j);
    degreesX += M_PI/(4*4*4);
    update();
}

/*
 **************
 *            *
 *    SLOTS   *
 *            *
 **************
 */

void MyGLWidget::ZoomPerspectiva(int v)
{
    makeCurrent();
    FOVini -= (v - fvant)*(M_PI/180);
    FOV -= (v - fvant)*(M_PI/180);
    if (FOVini < 15*(float)M_PI/180) FOVini = 15*(float)M_PI/180;
    else if (FOVini > 100*(float)M_PI/180) FOVini = 100*(float)M_PI/180;
    if (FOV < 15*(float)M_PI/180) FOV = 15*(float)M_PI/180;
    else if (FOV > 100*(float)M_PI/180) FOV = 100*(float)M_PI/180;
    projectTransformPerspectiva();
    update();
    fvant = v;
}

void MyGLWidget::ZoomOrtogonal(int v){
    makeCurrent();
    float aux = (float)(v-150)/-100;
    laux = -aux*radiModel;
    raux = aux*radiModel;
    taux = aux*radiModel;
    baux = -aux*radiModel;

    if (laux > -radiModel/2) laux = -radiModel/2;
    else if (laux < -1.5*radiModel) laux = -1.5*radiModel;
    if (raux < radiModel/2) raux = radiModel/2;
    else if (raux > 1.5*radiModel) raux = 1.5*radiModel;
    if (taux < radiModel/2) taux = radiModel/2;
    else if (taux > 1.5*radiModel) taux = 1.5*radiModel;
    if (baux > -radiModel/2) baux = -radiModel/2;
    else if (baux < -1.5*radiModel) baux = -1.5*radiModel;
    resizeOrto();
    projectTransformOrtogonal();
    update();
}

void MyGLWidget::Perspective(bool b)
{
    makeCurrent();
    if(b){
        projectTransformPerspectiva();
        ortogonal = false;
    }
    update();
}

void MyGLWidget::Ortogonal(bool b)
{
    makeCurrent();
    if(b){
        projectTransformOrtogonal();
        ortogonal = true;
    }
    update();
}


void MyGLWidget::CarregaModel(QString str)
{
    makeCurrent();
    if(str == "Patricio"){
        model = "Patricio";
        index = 1;
    }
    else if(str == "Homer"){
        model = "Homer";
        index = 2;
    }
    else if(str == "Legoman"){
        model = "Legoman";
        index = 3;
    }
    else if(str == "Vaca"){
        model = "Vaca";
        index = 4;
    }
    else if(str == "Delfin"){
        model = "Delfin";
        index = 5;
    }
    else if(str == "F16"){
        model = "F16";
        index = 6;
    }
    else if(str == "Porsche"){
        model = "Porsche";
        index = 7;
    }
    update();
}

void MyGLWidget::updateTheta(int value)
{
    makeCurrent();
    theta = value*(float)(M_PI/180);
    viewTransform();
    update();
}

void MyGLWidget::updatePsi(int value)
{
    makeCurrent();
    psi = value*(float)(M_PI/180);
    viewTransform();
    update();
}


void MyGLWidget::setEscalat(double value){
    makeCurrent();
    scale = value;
    update();
}

void MyGLWidget::setRed(int color)
{

    red = (float)color/(float)255;

}

void MyGLWidget::setGreen(int color)
{

    green = (float)color/(float)255;

}

void MyGLWidget::setBlue(int color)
{

    blue = (float)color/(float)255;
}

void MyGLWidget::setSong(QString song)
{
    makeCurrent();
    filename = song;
    player.setMedia(QUrl::fromLocalFile(filename));
    update();
}

void MyGLWidget::sunshine()
{
    makeCurrent();
    glUniform3fv(posFocusLoc, 1, &posFocus[0]);
    glUniform3fv(colorFocusLoc, 1, &colFocus[0]);
    if(!cambia){
        posFocus.y += 0.1*factor;
        if(posFocus.y >= 4) factor = -1;
        else if(posFocus.y <= -4) factor = 1;
    }
    else{
        posFocus.x +=0.1*factor;
        if(posFocus.x >= 4) factor = -1;
        else if(posFocus.x <= -4) factor = 1;
    }
    update();
}

void MyGLWidget::restart()
{
    glUniform3fv(posFocusLoc, 1, &posFocus[0]);
    posFocus = glm::vec3(2,0,2);
}
