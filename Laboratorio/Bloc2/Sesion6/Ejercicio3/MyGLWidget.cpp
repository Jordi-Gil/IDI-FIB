#include "MyGLWidget.h"

#include <iostream>

MyGLWidget::MyGLWidget (QWidget* parent) : QOpenGLWidget(parent)
{
  setFocusPolicy(Qt::ClickFocus);  // per rebre events de teclat
  scale = 1.0f;
}

MyGLWidget::~MyGLWidget ()
{
  if (program != NULL)
    delete program;
}

void MyGLWidget::initializeGL ()
{
  // Cal inicialitzar l'ús de les funcions d'OpenGL
  initializeOpenGLFunctions();

  x = glm::vec3(1,0,0);
  y = glm::vec3(0,1,0);
  z = glm::vec3(0,0,1);

  degreesX = 0.0;
  degreesY = 0.0;

  gir = false;
  sound = false;
  tercera = false;
  primera = false;
  ortogonal = true;

  origen = glm::vec3(0,0,0);

  one_degree = (float)M_PI/180;

  thita = 45*one_degree;
  psi = 0.0;

  v = glm::vec3(1/sqrt(3),1/sqrt(3),1/sqrt(3));


  glClearColor(0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  carregaShaders();
  createBuffers();

  glEnable(GL_DEPTH_TEST);

  ini_camera_3a_persona();

}

void MyGLWidget::paintGL ()
{
  // Esborrem el frame-buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Carreguem la transformació de model
  modelTransformPat1 ();
  pinta_patricio();

  modelTransformPat2();
  pinta_patricio();

  modelTransformFloor ();
  pinta_floor();

  glBindVertexArray (0);

  if(gir) gira();

}

void MyGLWidget::resizeGL (int w, int h)
{
    evitaDeformacions(w,h);
    glViewport(0, 0, w, h);
}

void MyGLWidget::createBuffers ()
{

  carregaModelPatricio();
  carregaFloor();

  calculaVertexMaxMin();

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
  // Obtenim identificador per a l'atribut “color” del vertex shader
  colorLoc = glGetAttribLocation (program->programId(), "color");
  // Uniform locations
  transLoc = glGetUniformLocation(program->programId(), "TG");
  projLoc = glGetUniformLocation(program->programId(), "PM");
  viewLoc = glGetUniformLocation(program->programId(), "VM");
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

void MyGLWidget::carregaModelPatricio(){

    //carrega el model
    patricio.load("../../models/Patricio.obj");
    // Creació del Vertex Array Object per pintar
    glGenVertexArrays(1, &VAO_Patricio);
    glBindVertexArray(VAO_Patricio);

    glGenBuffers(1, &VBO_PatricioPos);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_PatricioPos);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*patricio.faces().size()*3*3, patricio.VBO_vertices(), GL_STATIC_DRAW);

    // Activem l'atribut vertexLoc
    glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexLoc);

    glGenBuffers(1, &VBO_PatricioCol);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_PatricioCol);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*patricio.faces().size()*3*3, patricio.VBO_matdiff(), GL_STATIC_DRAW);

    // Activem l'atribut colorLoc
    glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(colorLoc);

}

void MyGLWidget::carregaFloor(){

      // Tres vèrtexs amb X, Y i Z
    glm::vec3 Colors[4];

    Vertices[0] = glm::vec3(-2.0, 0.0, 2.0);
    Vertices[1] = glm::vec3(2.0, 0.0, 2.0);
    Vertices[2] = glm::vec3(-2.0, 0.0, -2.0);
    Vertices[3] = glm::vec3(2.0, 0.0, -2.0);

    Colors[0] = glm::vec3(1.0,1.0,1.0);
    Colors[1] = glm::vec3(1.0,1.0,1.0);
    Colors[2] = glm::vec3(1.0,1.0,1.0);
    Colors[3] = glm::vec3(1.0,1.0,1.0);

    //Creació del Vertex Array Object
    glGenVertexArrays(1, &VAO_Floor);
    glBindVertexArray(VAO_Floor);

    glGenBuffers(1,&VBO_FloorPos);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_FloorPos);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices,GL_STATIC_DRAW);

    //Activem l'atribut vertexLoc
    glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexLoc);

    glGenBuffers(1, &VBO_FloorCol);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_FloorCol);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Colors), Colors, GL_STATIC_DRAW);

    glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(colorLoc);

}

/*
 ********************************
 *                              *
 *   FUNCIONES DE PINTAR VAOs   *
 *                              *
 ********************************
*/

void MyGLWidget::pinta_patricio()
{
    // Activem el VAO per a pintar el homer
  glBindVertexArray (VAO_Patricio);

  // pintem
  glDrawArrays(GL_TRIANGLES, 0, patricio.faces().size()*3);
}

void MyGLWidget::pinta_floor()
{
    // Activem el VAO per a pintar el floor
  glBindVertexArray(VAO_Floor);

  //pintem
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

/*
 *****************************
 *                           *
 *   FUNCIONES DE CALCULO    *
 *                           *
 *****************************
*/

void MyGLWidget::calculaVertexMaxMin()
{
    EsferaMinima.x = EsferaMaxima.x = patricio.vertices()[0];
    EsferaMinima.y = EsferaMaxima.y = patricio.vertices()[1];
    EsferaMinima.z = EsferaMaxima.z = patricio.vertices()[2];

    for(unsigned int i = 3; i < patricio.vertices().size(); i+=3){
        //Comprovamos la componente 'x'
        EsferaMinima.x = std::min(EsferaMinima.x,(float) patricio.vertices()[i]);
        EsferaMaxima.x = std::max(EsferaMaxima.x,(float) patricio.vertices()[i]);

        //Comprovamos la componente 'y'
        EsferaMinima.y = std::min(EsferaMinima.y,(float) patricio.vertices()[i+1]);
        EsferaMaxima.y = std::max(EsferaMaxima.y,(float) patricio.vertices()[i+1]);

        //Comprovamos la componente 'z'
        EsferaMinima.z = std::min(EsferaMinima.z,(float) patricio.vertices()[i+2]);
        EsferaMaxima.z = std::max(EsferaMaxima.z,(float) patricio.vertices()[i+2]);

    }
    //Abansa de recalcular els vertex de l'esfera amb el terra, calculem l'escalat amb la capsa del patricio
    scalePat1 = 2/(EsferaMaxima.y - EsferaMinima.y);
    CBM = glm::vec3((EsferaMaxima.x + EsferaMinima.x)/2, EsferaMinima.y, (EsferaMaxima.z + EsferaMinima.z)/2 );

    for(int i = 0; i < 4; ++i){
        EsferaMinima.x = std::min(EsferaMinima.x, Vertices[i].x);
        EsferaMaxima.x = std::max(EsferaMaxima.x, Vertices[i].x);

        EsferaMinima.y = std::min(EsferaMinima.y, Vertices[i].y);
        EsferaMaxima.y = std::max(EsferaMaxima.y, Vertices[i].y);

        EsferaMinima.z = std::min(EsferaMinima.z, Vertices[i].z);
        EsferaMaxima.z = std::max(EsferaMaxima.z, Vertices[i].z);
    }

    calculaCentreModel();

}

void MyGLWidget::calculaCentreModel()
{
    centreModel = glm::vec3((EsferaMaxima.x + EsferaMinima.x)/2, (EsferaMaxima.y + EsferaMinima.y)/2, (EsferaMaxima.z + EsferaMinima.z)/2 );

    // r = √((x2 - x1)^2 + (y2 - y1)^2 + (z2 - z1)^2)/2
    float auxX = pow((EsferaMaxima.x - EsferaMinima.x),2);
    float auxY = pow((EsferaMaxima.y - EsferaMinima.y),2);
    float auxZ = pow((EsferaMaxima.z - EsferaMinima.z),2);

    radiModel = sqrt(auxX + auxY + auxZ);

    left = -radiModel;
    right = radiModel;
    bottom = -radiModel;
    top = radiModel;

    d = 2*radiModel;
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
    }
    else
    {
        FOVini += one_degree;
        FOV += one_degree;
    }

    if(FOVini < 15*one_degree) FOVini = 15*one_degree;
    else if(FOVini > 160*one_degree) FOVini = 160*one_degree;

    if(FOV < 15*one_degree) FOV = 15*one_degree;
    else if(FOV > 160*one_degree) FOV = 160*one_degree;

    if(!ortogonal) projectTransformPerspectiva();

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
        if(e->x() > position.x())
        {
            if(primera) degreesX += one_degree;
            else if(tercera){
                thita += (float)M_PI/180;
                viewTransform();
            }

        }
        else{

            if(primera) degreesX -= one_degree;
            else if(tercera){
                thita -= (float)M_PI/180;
                viewTransform();
            }
        }

        if(e->y() > position.y()){
            if(primera) degreesY += one_degree;
            else if(tercera){
                psi += (float)M_PI/180;
                viewTransform();
            }
        }
        else{
            if(primera) degreesY -= one_degree;
            else if(tercera){
                psi -= (float)M_PI/180;
                viewTransform();
            }
        }
    }
    position = e->pos();
    update();
}

void MyGLWidget::keyPressEvent(QKeyEvent* event)
{
  makeCurrent();
  switch (event->key()) {
    case Qt::Key_S: { // escalar a més gran
      scale += 0.05;
      break;
    }
    case Qt::Key_D: { // escalar a més petit
      scale -= 0.05;
      break;
    }

    case Qt::Key_R: {
        degreesX += one_degree;
        break;
    }
    case Qt::Key_1: {
        ini_camera();
        break;
    }
    case Qt::Key_3: {
        ini_camera_3a_persona();
        break;
    }

    case Qt::Key_G: {
        gir = !gir;
        break;
    }

    case Qt::Key_O: {
        projectTransformOrtogonal();
        break;
    }

    case Qt::Key_P: {
        projectTransformPerspectiva();
        break;
    }

    case Qt::Key_M: {
        if(gir) {
            if(!sound){
                player.setMedia(QUrl::fromLocalFile("/home/jordi/Documents/Universidad/IDI/Laboratorio/Bloc2/Sesion5/Brusko Problemz - Industrial Hardcore.mp3"));
                player.play();
                sound = true;
            }
            else{
                player.stop();
                sound = false;
            }
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
    ViewMat = glm::rotate(ViewMat,thita,x);
    ViewMat = glm::rotate(ViewMat,-psi,y);
    ViewMat = glm::translate(ViewMat,-VRP);
    glUniformMatrix4fv(viewLoc,1,GL_FALSE,&ViewMat[0][0]);
}

void MyGLWidget::ini_camera()
{
    primera = true;
    tercera = false;
    FOV = FOVini = 2*asin(radiModel/d);

    ra = double(width())/double(height());

    znear = d - radiModel;
    zfar = d + radiModel;

    if(ortogonal) projectTransformOrtogonal();
    else projectTransformPerspectiva();

    VRP = origen;
    viewTransform();
}


void MyGLWidget::ini_camera_3a_persona()
{
    tercera = true;
    primera = false;

    FOV = FOVini = 2*asin(radiModel/d);
    ra = double(width())/double(height());
    znear = d - radiModel;
    zfar = d + radiModel;

    if(ortogonal) projectTransformOrtogonal();
    else projectTransformPerspectiva();

    VRP = origen;
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
  transform = glm::translate(transform, glm::vec3(1.0,0.0,1.0));
  transform = glm::scale(transform, glm::vec3(scalePat1));

  transform = glm::rotate(transform,degreesX,y);

  transform = glm::translate(transform,-CBM);

  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::modelTransformPat2()
{
  // Matriu de transformació de model
  glm::mat4 transform (1.0f);
  transform = glm::translate(transform, glm::vec3(-1.0,0.0,-1.0));
  transform = glm::scale(transform, glm::vec3(scalePat1));

  transform = glm::rotate(transform,degreesX,y);
  transform = glm::rotate(transform,(float)M_PI,y);

  transform = glm::translate(transform,-CBM);

  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::modelTransformFloor()
{
    glm::mat4 transform (1.0f);
    glUniformMatrix4fv(transLoc,1,GL_FALSE, &transform[0][0]);
}

void MyGLWidget::evitaDeformacions(int w, int h)
{
    ra = double(w)/double(h);
    if(ra < 1.0f) FOV = 2*atan(tan(FOVini/2)/ra);

    if(ortogonal){

        projectTransformOrtogonal();
    }
    else projectTransformPerspectiva();

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
