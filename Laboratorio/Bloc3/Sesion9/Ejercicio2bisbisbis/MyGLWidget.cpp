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

    index = 2;

    origen = glm::vec3(0,0,0);

    one_degree = (float)M_PI/180;

    v = glm::vec3(1/sqrt(3),1/sqrt(3),1/sqrt(3));

    radiModel = 4;

    // Cal inicialitzar l'ús de les funcions d'OpenGL
    initializeOpenGLFunctions();

    glClearColor(0.1, 0.1, 0.1, 1.0); // defineix color de fons (d'esborrat)
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
    tm = 0;

    my_bool = 0;
    glUniform1i(boolLoc, my_bool);

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


    modelTransform1();
    pinta_model();

    modelTransform2();
    pinta_model();

    modelTransformFloor ();
    pinta_floor();

    modelTransformWall();
    pinta_wall();

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
    size = 11;
    carregaModels();
    calculaCapsaModels();

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
    boolLoc = glGetUniformLocation(program->programId(),"my_bool");
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

void MyGLWidget::sizeVectors()
{
  models.resize(size);
  scales.resize(size);
  radiModels.resize(size);
  Vs.resize(size);
  for(int i = 0; i < (int) size; ++i) Vs[i].resize(7);
  EsferaModels.resize(size);
  for(int i = 0; i < (int) size; ++i) EsferaModels[i].resize(4);

}

void MyGLWidget::carregaModels(){


    //carrega el model
    sizeVectors();

    models[0].load("../../models/Suelo/Files/CobbleStones2.obj");
    models[1].load("../../models/fence/grade.obj");
    models[2].load("../../models/Patricio.obj");
    models[3].load("../../models/homer.obj");
    models[4].load("../../models/legoman.obj");
    models[5].load("../../models/legoman-assegut.obj");
    models[6].load("../../models/Shaun_Hastings.obj");
    models[7].load("../../models/cow.obj");
    models[8].load("../../models/dolphin.obj");
    models[9].load("../../models/f-16.obj");
    models[10].load("../../models/porsche.obj");


    for(int i = 0; i < size; ++i){
       carregaModel(i);
    }
}

void MyGLWidget::carregaModel(int i)
{
      // Creació del Vertex Array Object per pintar
      glGenVertexArrays(1, &Vs[i][0]);
      glBindVertexArray(Vs[i][0]);

      //Vertex
      glGenBuffers(1, &Vs[i][1]);
      glBindBuffer(GL_ARRAY_BUFFER, Vs[i][1]);
      glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*models[i].faces().size()*3*3, models[i].VBO_vertices(), GL_STATIC_DRAW);

      // Activem l'atribut vertexLoc
      glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
      glEnableVertexAttribArray(vertexLoc);

      //Normals
      glGenBuffers(1, &Vs[i][2]);
      glBindBuffer(GL_ARRAY_BUFFER, Vs[i][2]);
      glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*models[i].faces().size()*3*3, models[i].VBO_normals(), GL_STATIC_DRAW);

      // Activem l'atribut normalLoc
      glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
      glEnableVertexAttribArray(normalLoc);

      //Normals
      glGenBuffers(1, &Vs[i][3]);
      glBindBuffer(GL_ARRAY_BUFFER, Vs[i][3]);
      glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*models[i].faces().size()*3*3, models[i].VBO_matamb(), GL_STATIC_DRAW);

      // Activem l'atribut matambLoc
      glVertexAttribPointer(matambLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
      glEnableVertexAttribArray(matambLoc);

      glGenBuffers(1, &Vs[i][4]);
      glBindBuffer(GL_ARRAY_BUFFER, Vs[i][4]);
      glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*models[i].faces().size()*3*3, models[i].VBO_matdiff(), GL_STATIC_DRAW);

      // Activem l'atribut matdiffLoc
      glVertexAttribPointer(matdiffLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
      glEnableVertexAttribArray(matdiffLoc);

       // Buffer de component especular
      glGenBuffers(1, &Vs[i][5]);
      glBindBuffer(GL_ARRAY_BUFFER, Vs[i][5]);
      glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*models[i].faces().size()*3*3, models[i].VBO_matspec(), GL_STATIC_DRAW);

      glVertexAttribPointer(matspecLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
      glEnableVertexAttribArray(matspecLoc);

      // Buffer de component shininness
      glGenBuffers(1, &Vs[i][6]);
      glBindBuffer(GL_ARRAY_BUFFER, Vs[i][6]);
      glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*models[i].faces().size()*3, models[i].VBO_matshin(), GL_STATIC_DRAW);

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
    glDrawArrays(GL_TRIANGLES, 0, models[index].faces().size()*3);
}

void MyGLWidget::pinta_floor()
{
    // Activem el VAO per a pintar el floor
    glBindVertexArray(Vs[0][0]);

    //pintem
    glDrawArrays(GL_TRIANGLES, 0, models[0].faces().size()*3);

}

void MyGLWidget::pinta_wall()
{
    // Activem el VAO per a pintar el floor
    glBindVertexArray(Vs[1][0]);

    //pintem
    glDrawArrays(GL_TRIANGLES, 0, models[1].faces().size()*3);
}

/*
 *****************************
 *                           *
 *   FUNCIONES DE CALCULO    *
 *                           *
 *****************************
*/
void MyGLWidget::calculaCapsaModels()
{
  for(int i = 0; i < size; ++i)
  {
    calculaCapsaModel(i);
  }
}

void MyGLWidget::calculaCapsaModel(int i)
{

  EsferaModels[i][0].x = EsferaModels[i][1].x = models[i].vertices()[0];
  EsferaModels[i][0].y = EsferaModels[i][1].y = models[i].vertices()[1];
  EsferaModels[i][0].z = EsferaModels[i][1].z = models[i].vertices()[2];

  for(unsigned int j = 3; j < models[i].vertices().size(); j+=3)
  {
    //Comprovamos la componente 'x'
    EsferaModels[i][0].x = std::min(EsferaModels[i][0].x,(float) models[i].vertices()[j]);
    EsferaModels[i][1].x = std::max(EsferaModels[i][1].x,(float) models[i].vertices()[j]);

    //Comprovamos la componente 'y'
    EsferaModels[i][0].y = std::min(EsferaModels[i][0].y,(float) models[i].vertices()[j+1]);
    EsferaModels[i][1].y = std::max(EsferaModels[i][1].y,(float) models[i].vertices()[j+1]);

    //Comprovamos la componente 'z'
    EsferaModels[i][0].z = std::min(EsferaModels[i][0].z,(float) models[i].vertices()[j+2]);
    EsferaModels[i][1].z = std::max(EsferaModels[i][1].z,(float) models[i].vertices()[j+2]);
  }
  //escalat del Model 'i'
  scales[i] = 2/(EsferaModels[i][1].y - EsferaModels[i][0].y);
  if(i == 7 or i == 10) scales[i] *= 0.5;
  if(i == 9)scales[i] *= 0.4;
  //Centre de la Base del Model 'i'
  EsferaModels[i][3] = glm::vec3((EsferaModels[i][1].x + EsferaModels[i][0].x)/2,
                                  EsferaModels[i][0].y,
                                 (EsferaModels[i][1].z + EsferaModels[i][0].z)/2 );
  //Centre del Model 'i'
  EsferaModels[i][2] = glm::vec3((EsferaModels[i][1].x + EsferaModels[i][0].x)/2,
                                 (EsferaModels[i][1].y + EsferaModels[i][0].y)/2,
                                 (EsferaModels[i][1].z + EsferaModels[i][0].z)/2 );
  // r = √((x2 - x1)^2 + (y2 - y1)^2 + (z2 - z1)^2)
  float auxX = pow((EsferaModels[i][1].x - EsferaModels[i][0].x),2);
  float auxY = pow((EsferaModels[i][1].y - EsferaModels[i][0].y),2);
  float auxZ = pow((EsferaModels[i][1].z - EsferaModels[i][0].z),2);

  radiModels[i] = sqrt(auxX + auxY + auxZ);
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

        if(psi < 0) psi = 359*(float)(M_PI/180);
        else if(psi > 359) psi = 0;
        emit updatepsi((int)(psi*((float)180/M_PI))%360);

        if(theta < 0) theta = 359*(float)(M_PI/180);
        else if(theta > 359) theta = 0;
        emit updatetheta((int)(theta*((float)180/M_PI))%360);

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
        case Qt::Key_Q: {
            if(my_bool != 0){
                my_bool = 0;
                glUniform1i(boolLoc, my_bool);
            }
            break;
        }
        case Qt::Key_W: {
            if(my_bool != 1){
                my_bool = 1;
                glUniform1i(boolLoc, my_bool);
            }
            break;
        }
        case Qt::Key_E: {
            if(my_bool != 2){
                my_bool = 2;
                glUniform1i(boolLoc, my_bool);
            }
            break;
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

void MyGLWidget::modelTransform1()
{
  glm::mat4 transform (1.0f);
  transform = glm::scale(transform, glm::vec3(scale));
  if(index == 7) transform = glm::translate(transform, glm::vec3(1.0,-2.1,0.4));
  else if(index == 9) transform = glm::translate(transform, glm::vec3(1.0,-2.0,0.5));
  else if(index == 10) transform = glm::translate(transform, glm::vec3(1.0,-2.0,0.3));
  else transform = glm::translate(transform, glm::vec3(1.0,-2.0,1.0));
  transform = glm::scale(transform, glm::vec3(scales[index]));

  transform = glm::rotate(transform,degreesX,y);
  if(index == 3) transform = glm::rotate(transform, (float)M_PI/2, y);

  transform = glm::translate(transform,-EsferaModels[index][3]);

  if(index == 7){
    transform = glm::translate(transform,glm::vec3(0,-EsferaModels[7][0].y,0));

    //Colocar "Bien"
    transform = glm::rotate(transform,(float)-M_PI/2,y);
    transform = glm::rotate(transform,(float)-M_PI/2,x);
  }

  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);

}

void MyGLWidget::modelTransform2()
{
  glm::mat4 transform (1.0f);
  transform = glm::scale(transform, glm::vec3(scale));
  if(index == 7) transform = glm::translate(transform, glm::vec3(-1.0,-2.1,-0.4));
  else if(index == 9) transform = glm::translate(transform, glm::vec3(-1.0,-2.0,-0.5));
  else if(index == 10) transform = glm::translate(transform, glm::vec3(-1.0,-2.0,-0.3));
  else transform = glm::translate(transform, glm::vec3(-1.0,-2.0,-1.0));
  transform = glm::scale(transform, glm::vec3(scales[index]));


  transform = glm::rotate(transform,degreesX,y);
  transform = glm::rotate(transform,(float)M_PI,y);
  if(index == 3) transform = glm::rotate(transform, (float)M_PI/2, y);

  transform = glm::translate(transform,-EsferaModels[index][3]);

  if(index == 7){
    transform = glm::translate(transform,glm::vec3(0,-EsferaModels[7][0].y,0));

    //Colocar "Bien"
    transform = glm::rotate(transform,(float)-M_PI/2,y);
    transform = glm::rotate(transform,(float)-M_PI/2,x);
  }

  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);

}

void MyGLWidget::modelTransformFloor()
{
    glm::mat4 transform (1.0f);
    transform = glm::scale(transform, glm::vec3(scale));
    transform = glm::scale(transform, glm::vec3(0.5));
    transform = glm::translate(transform,glm::vec3(0.0,-4.25,0.0));

    transform = glm::translate(transform,-EsferaModels[0][3]);

    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::modelTransformWall()
{
  glm::mat4 transform (1.0f);
  transform = glm::scale(transform, glm::vec3(scale));
  transform = glm::translate(transform, glm::vec3(0.0, -2.0, -2.0));
  transform = glm::scale(transform, glm::vec3(0.525));

  transform = glm::rotate(transform, (float)M_PI/2,y);

  transform = glm::translate(transform, -EsferaModels[1][3]);

  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
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
//NUEVO
void MyGLWidget::resizeVectors()
{
  ++size;
  models.resize(size);
  scales.resize(size);
  radiModels.resize(size);
  Vs.resize(size);
  Vs[size-1].resize(7);
  EsferaModels.resize(size);
  EsferaModels[size-1].resize(4);

}

void MyGLWidget::setOBJ(QString objfile)
{
  makeCurrent();
  resizeVectors();
  std::cerr << objfile.toStdString() << "\n";
  models[size-1].load(objfile.toStdString());
  carregaModel(size-1);
  calculaCapsaModel(size-1);
  update();

}

void MyGLWidget::setIndex(int ind)
{
  makeCurrent();
  index = ind + 2;
  update();
}

void MyGLWidget::sunshine()
{
    makeCurrent();
    posFocus.x = 3.0*glm::cos(tm);
    posFocus.y = 3.0*glm::sin(tm);
    tm += 0.01;
    if(tm >= (float)2*M_PI) tm = 0;
    glUniform3fv(posFocusLoc, 1, &posFocus[0]);
    glUniform3fv(colorFocusLoc, 1, &colFocus[0]);
    update();
}

void MyGLWidget::restart()
{
    glUniform3fv(posFocusLoc, 1, &posFocus[0]);
    posFocus = glm::vec3(2,0,2);
}
