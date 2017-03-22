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
  
  gir = false;
  sound = false;
  origen = glm::vec3(0,0,0);
  
  v = glm::vec3(1/sqrt(3),1/sqrt(3),1/sqrt(3));
  
  glClearColor(0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  carregaShaders();
  createBuffers();
  glEnable(GL_DEPTH_TEST);
  
  ini_camera();
  
}

void MyGLWidget::paintGL () 
{
  // Esborrem el frame-buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Carreguem la transformació de model
  modelTransform ();
  pinta_patricio();
  
  modelTransform2 ();
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
  calculaVertexMaxMin();
  
  carregaFloor();
  
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
    
    glm::vec3 Vertices[4];  // Tres vèrtexs amb X, Y i Z
    glm::vec3 Colors[4];
    
    Vertices[0] = glm::vec3(-1.0,-1.0,1.0);
    Vertices[1] = glm::vec3(1.0,-1.0,1.0);
    Vertices[2] = glm::vec3(-1.0,-1.0,-1.0);
    Vertices[3] = glm::vec3(1.0,-1.0,-1.0);
    
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
 *   FUNCIONES DE "CÁMARA"   *
 *                           *
 *****************************
*/

void MyGLWidget::calculaVertexMaxMin()
{
    Modelminim.x = Modelmaxim.x = patricio.vertices()[0];
    Modelminim.y = Modelmaxim.y = patricio.vertices()[1];
    Modelminim.z = Modelmaxim.z = patricio.vertices()[2];
    
    for(unsigned int i = 3; i < patricio.vertices().size(); i+=3){
        //Comprovamos la componente 'x'
        Modelminim.x = std::min(Modelminim.x,(float) patricio.vertices()[i]);
        Modelmaxim.x = std::max(Modelmaxim.x,(float) patricio.vertices()[i]);
        
        //Comprovamos la componente 'y'
        Modelminim.y = std::min(Modelminim.y,(float) patricio.vertices()[i+1]);
        Modelmaxim.y = std::max(Modelmaxim.y,(float) patricio.vertices()[i+1]);
        
        //Comprovamos la componente 'z'
        Modelminim.z = std::min(Modelminim.z,(float) patricio.vertices()[i+2]);
        Modelmaxim.z = std::max(Modelmaxim.z,(float) patricio.vertices()[i+2]);
        
        calculaCentreModel();
        
    }
}

void MyGLWidget::calculaCentreModel()
{
    centreModel = glm::vec3((Modelmaxim.x + Modelminim.x)/2, (Modelmaxim.y + Modelminim.y)/2, (Modelmaxim.z + Modelminim.z)/2 );
    
    // r = √((x2 - x1)^2 + (y2 - y1)^2 + (z2 - z1)^2)
    float auxX = pow((Modelmaxim.x - Modelminim.x),2);
    float auxY = pow((Modelmaxim.y - Modelminim.y),2);
    float auxZ = pow((Modelmaxim.z - Modelminim.z),2);
    
    radiModel = sqrt(auxX + auxY + auxZ);
    
    d = 2*radiModel;
}

/*
 ********************************
 *                              *
 *   FUNCIONES DE INTERACCIÓN   *
 *                              *
 ********************************
*/

void MyGLWidget::mousePressEvent(QMouseEvent *e)
{
    
    position = e->pos();
    
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *e)
{
    makeCurrent(); 
    if(e->buttons() & Qt::LeftButton){
        if(e->x() > position.x()) degrees +=0.1;
        else degrees -=0.1;
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
        degrees += 0.1;//(float)M_PI/2;
        break;
    }
    case Qt::Key_P: {
        ini_camera();
        break;
    }
    case Qt::Key_T: {
        ini_camera_3a_persona();
        break;
    }
    
    case Qt::Key_G: {
        gir = !gir;
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
 *   FUNCIONES DE "VISION"   *
 *                           *
 ***************************** 
*/

void MyGLWidget::projectTransform(){
    //FOV, ra, znear, zfar
    glm::mat4 Proj = glm::perspective(FOV,ra,znear,zfar);
    glUniformMatrix4fv(projLoc,1,GL_FALSE,&Proj[0][0]);
}

void MyGLWidget::viewTransform(){
    glm::mat4 ViewMat = glm::lookAt(OBS,VRP,UP);
    glUniformMatrix4fv(viewLoc,1,GL_FALSE,&ViewMat[0][0]);
}
 
void MyGLWidget::ini_camera(){
    
    FOV = FOVini = (float)M_PI/4;
    ra = double(width())/double(height());
    znear = d - radiModel;
    zfar = d + radiModel;
    projectTransform();
    
    VRP = glm::vec3(0,0,0); 
    OBS = VRP + d*glm::vec3(0,0,1);
    UP = glm::vec3(0,1,0);
    viewTransform();
}


void MyGLWidget::ini_camera_3a_persona()
{
    FOV = 2*asin(radiModel/d);
    //FOV = (float)M_PI/2.0f;
    ra = double(width())/double(height());
    znear = d - radiModel;
    zfar = d + radiModel;
    projectTransform();
    
    VRP = origen;
    OBS = VRP + d*glm::vec3(0,0,1);
    UP = glm::vec3(0,1,0);
    viewTransform();
    
}
 
/*
 ***********************************
 *                                 *
 *   FUNCIONES DE TRANSFORMACIÓN   *
 *                                 *
 ***********************************
*/

void MyGLWidget::modelTransform () 
{
  // Matriu de transformació de model
  glm::mat4 transform (1.0f);
  transform = glm::scale(transform, glm::vec3(scale));
  transform = glm::rotate(transform,degrees,y);
  transform = glm::translate(transform,-centreModel);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::modelTransform2()
{
    glm::mat4 transform (1.0f);
    transform = glm::scale(transform, glm::vec3(scale));
    glUniformMatrix4fv(transLoc,1,GL_FALSE, &transform[0][0]);
}

void MyGLWidget::evitaDeformacions(int w, int h)
{
    ra = double(w)/double(h);
    if(ra < 1.0f) FOV = 2*atan(tan(FOVini/2)/ra);
    projectTransform();
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
    degrees += M_PI/(4*4*4);
    update();
}
