#include "MyGLWidget.h"

#include <iostream>

MyGLWidget::MyGLWidget (QWidget* parent) : QOpenGLWidget(parent)
{
  setFocusPolicy(Qt::ClickFocus);  // per rebre events de teclat
  scale = 1.0f;
  emit updateEscalat(scale);
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
  xIni = 0;
  yIni = 0;

  auxP = 15;

  auxO = 0;

  gir = false;
  sound = false;
  tercera = false;
  primera = false;
  ortogonal = true;

  model = "Patricio";

  origen = glm::vec3(0,0,0);

  one_degree = (float)M_PI/180;

  thita = 0.0;
  psi = 0.0;
  thitaIni = 0;
  psiIni = 0;

  v = glm::vec3(1/sqrt(3),1/sqrt(3),1/sqrt(3));


  glClearColor(0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  carregaShaders();
  createBuffers();

  glEnable(GL_DEPTH_TEST);

  ini_camera_3a_persona();

  //Signals
  emit setPressP(true);
  emit setPress(true);
  emit setPressPrimera(true);
  emit setPressTercera(true);

}

void MyGLWidget::paintGL ()
{
  // Esborrem el frame-buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Carreguem la transformació de model
  if(model == "Patricio"){
    modelTransformPat1 ();
    pinta_patricio();

    modelTransformPat2();
    pinta_patricio();
  }
  else if(model == "Legoman"){
    modelTransformLego1();
    pinta_legoman();

    modelTransformLego2();
    pinta_legoman();
  }
  else if(model == "Vaca"){
    modelTransformVaca1();
    pinta_vaca();

    modelTransformVaca2();
    pinta_vaca();
  }
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
    carregaModelLegoman();
    carregaModelVaca();
    carregaFloor();

    calculaCapsaPatricio();
    calculaCapsaLegoman();
    calculaCapsaVaca();

    if(model == "Patricio")
    {
        CBM = CBMPat;
        scaleM = scalePat1;

        radiModel = radiModelPat;

        left = laux = -radiModel;
        right = raux = radiModel;
        bottom = baux = -radiModel;
        top = taux = radiModel;

        d = 2*radiModel;
    }
    else if(model == "Legoman"){

        CBM = CBMLego;
        scaleM = scaleLego;

        radiModel = radiModelLego;

        left = laux = -radiModel;
        right = raux = radiModel;
        bottom = baux = -radiModel;
        top = taux = radiModel;

        d = 2*radiModel;
    }
    else if(model == "Vaca"){

      CBM = CBMVaca;
      scaleM = scaleVaca;

      radiModel = radiModelVaca;

      left = laux = -radiModel;
      right = raux = radiModel;
      bottom = baux = -radiModel;
      top = taux = radiModel;

      d = 2*radiModel;
    }

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

//Carrega el Patricio
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

//Carrega el Legoman
void MyGLWidget::carregaModelLegoman(){

    //carrega el model
    legoman.load("../../models/legoman.obj");
    // Creació del Vertex Array Object per pintar
    glGenVertexArrays(1, &VAO_Legoman);
    glBindVertexArray(VAO_Legoman);

    glGenBuffers(1, &VBO_LegomanPos);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_LegomanPos);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*legoman.faces().size()*3*3, legoman.VBO_vertices(), GL_STATIC_DRAW);

    // Activem l'atribut vertexLoc
    glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexLoc);

    glGenBuffers(1, &VBO_LegomanCol);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_LegomanCol);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*legoman.faces().size()*3*3, legoman.VBO_matdiff(), GL_STATIC_DRAW);

    // Activem l'atribut colorLoc
    glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(colorLoc);

}

//Carrega la Vaca
void MyGLWidget::carregaModelVaca(){

    //carrega el model
    vaca.load("../../models/cow.obj");
    // Creació del Vertex Array Object per pintar
    glGenVertexArrays(1, &VAO_Vaca);
    glBindVertexArray(VAO_Vaca);

    glGenBuffers(1, &VBO_VacaPos);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_VacaPos);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*vaca.faces().size()*3*3, vaca.VBO_vertices(), GL_STATIC_DRAW);

    // Activem l'atribut vertexLoc
    glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexLoc);

    glGenBuffers(1, &VBO_VacaCol);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_VacaCol);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*vaca.faces().size()*3*3, vaca.VBO_matdiff(), GL_STATIC_DRAW);

    // Activem l'atribut colorLoc
    glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(colorLoc);

}

//Carrega el terra
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

void MyGLWidget::pinta_legoman()
{
    // Activem el VAO per a pintar el homer
    glBindVertexArray (VAO_Legoman);

    // pintem
    glDrawArrays(GL_TRIANGLES, 0, legoman.faces().size()*3);
}

void MyGLWidget::pinta_vaca()
{
    // Activem el VAO per a pintar el homer
    glBindVertexArray (VAO_Vaca);

    // pintem
    glDrawArrays(GL_TRIANGLES, 0, vaca.faces().size()*3);
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

//Patricio
void MyGLWidget::calculaCapsaPatricio()
{
    EsferaMinimaPat.x = EsferaMaximaPat.x = patricio.vertices()[0];
    EsferaMinimaPat.y = EsferaMaximaPat.y = patricio.vertices()[1];
    EsferaMinimaPat.z = EsferaMaximaPat.z = patricio.vertices()[2];

    for(unsigned int i = 3; i < patricio.vertices().size(); i+=3){
        //Comprovamos la componente 'x'
        EsferaMinimaPat.x = std::min(EsferaMinimaPat.x,(float) patricio.vertices()[i]);
        EsferaMaximaPat.x = std::max(EsferaMaximaPat.x,(float) patricio.vertices()[i]);

        //Comprovamos la componente 'y'
        EsferaMinimaPat.y = std::min(EsferaMinimaPat.y,(float) patricio.vertices()[i+1]);
        EsferaMaximaPat.y = std::max(EsferaMaximaPat.y,(float) patricio.vertices()[i+1]);

        //Comprovamos la componente 'z'
        EsferaMinimaPat.z = std::min(EsferaMinimaPat.z,(float) patricio.vertices()[i+2]);
        EsferaMaximaPat.z = std::max(EsferaMaximaPat.z,(float) patricio.vertices()[i+2]);

    }
    //Abansa de recalcular els vertex de l'esfera amb el terra, calculem l'escalat amb la capsa del patricio
    scalePat1 = 2/(EsferaMaximaPat.y - EsferaMinimaPat.y);
    CBMPat = glm::vec3((EsferaMaximaPat.x + EsferaMinimaPat.x)/2, EsferaMinimaPat.y, (EsferaMaximaPat.z + EsferaMinimaPat.z)/2 );

    for(int i = 0; i < 4; ++i){
        EsferaMinimaPat.x = std::min(EsferaMinimaPat.x, Vertices[i].x);
        EsferaMaximaPat.x = std::max(EsferaMaximaPat.x, Vertices[i].x);

        EsferaMinimaPat.y = std::min(EsferaMinimaPat.y, Vertices[i].y);
        EsferaMaximaPat.y = std::max(EsferaMaximaPat.y, Vertices[i].y);

        EsferaMinimaPat.z = std::min(EsferaMinimaPat.z, Vertices[i].z);
        EsferaMaximaPat.z = std::max(EsferaMaximaPat.z, Vertices[i].z);
    }

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

//Legoman
void MyGLWidget::calculaCapsaLegoman()
{

    EsferaMinimaLego.x = EsferaMaximaLego.x = legoman.vertices()[0];
    EsferaMinimaLego.y = EsferaMaximaLego.y = legoman.vertices()[1];
    EsferaMinimaLego.z = EsferaMaximaLego.z = legoman.vertices()[2];


    for(unsigned int i = 3; i < legoman.vertices().size(); i+=3){
        //Comprovamos la componente 'x'
        EsferaMinimaLego.x = std::min(EsferaMinimaLego.x,(float) legoman.vertices()[i]);
        EsferaMaximaLego.x = std::max(EsferaMaximaLego.x,(float) legoman.vertices()[i]);

        //Comprovamos la componente 'y'
        EsferaMinimaLego.y = std::min(EsferaMinimaLego.y,(float) legoman.vertices()[i+1]);
        EsferaMaximaLego.y = std::max(EsferaMaximaLego.y,(float) legoman.vertices()[i+1]);

        //Comprovamos la componente 'z'
        EsferaMinimaLego.z = std::min(EsferaMinimaLego.z,(float) legoman.vertices()[i+2]);
        EsferaMaximaLego.z = std::max(EsferaMaximaLego.z,(float) legoman.vertices()[i+2]);

    }

    //Abansa de recalcular els vertex de l'esfera amb el terra, calculem l'escalat amb la capsa del legomans
    scaleLego = 2/(EsferaMaximaLego.y - EsferaMinimaLego.y);
    CBMLego = glm::vec3((EsferaMaximaLego.x + EsferaMinimaLego.x)/2, EsferaMinimaLego.y, (EsferaMaximaLego.z + EsferaMinimaLego.z)/2 );

    for(int i = 0; i < 4; ++i){
        EsferaMinimaLego.x = std::min(EsferaMinimaLego.x, Vertices[i].x);
        EsferaMaximaLego.x = std::max(EsferaMaximaLego.x, Vertices[i].x);

        EsferaMinimaLego.y = std::min(EsferaMinimaLego.y, Vertices[i].y);
        EsferaMaximaLego.y = std::max(EsferaMaximaLego.y, Vertices[i].y);

        EsferaMinimaLego.z = std::min(EsferaMinimaLego.z, Vertices[i].z);
        EsferaMaximaLego.z = std::max(EsferaMaximaLego.z, Vertices[i].z);
    }

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

    EsferaMinimaVaca.x = EsferaMaximaVaca.x = vaca.vertices()[0];
    EsferaMinimaVaca.y = EsferaMaximaVaca.y = vaca.vertices()[1];
    EsferaMinimaVaca.z = EsferaMaximaVaca.z = vaca.vertices()[2];


    for(unsigned int i = 3; i < vaca.vertices().size(); i+=3){
        //Comprovamos la componente 'x'
        EsferaMinimaVaca.x = std::min(EsferaMinimaVaca.x,(float) vaca.vertices()[i]);
        EsferaMaximaVaca.x = std::max(EsferaMaximaVaca.x,(float) vaca.vertices()[i]);

        //Comprovamos la componente 'y'
        EsferaMinimaVaca.y = std::min(EsferaMinimaVaca.y,(float) vaca.vertices()[i+1]);
        EsferaMaximaVaca.y = std::max(EsferaMaximaVaca.y,(float) vaca.vertices()[i+1]);

        //Comprovamos la componente 'z'
        EsferaMinimaVaca.z = std::min(EsferaMinimaVaca.z,(float) vaca.vertices()[i+2]);
        EsferaMaximaVaca.z = std::max(EsferaMaximaVaca.z,(float) vaca.vertices()[i+2]);

    }

    //Abansa de recalcular els vertex de l'esfera amb el terra, calculem l'escalat amb la capsa del vacas
    scaleVaca = 2/(EsferaMaximaVaca.y - EsferaMinimaVaca.y);
    CBMVaca = glm::vec3((EsferaMaximaVaca.x + EsferaMinimaVaca.x)/2, EsferaMinimaVaca.y, (EsferaMaximaVaca.z + EsferaMinimaVaca.z)/2 );

    for(int i = 0; i < 4; ++i){
        EsferaMinimaVaca.x = std::min(EsferaMinimaVaca.x, Vertices[i].x);
        EsferaMaximaVaca.x = std::max(EsferaMaximaVaca.x, Vertices[i].x);

        EsferaMinimaVaca.y = std::min(EsferaMinimaVaca.y, Vertices[i].y);
        EsferaMaximaVaca.y = std::max(EsferaMaximaVaca.y, Vertices[i].y);

        EsferaMinimaVaca.z = std::min(EsferaMinimaVaca.z, Vertices[i].z);
        EsferaMaximaVaca.z = std::max(EsferaMaximaVaca.z, Vertices[i].z);
    }

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
    if(ortogonal){
      //cerr << "Sin implementar" << endl;

      if(e->delta() > 0.f){

          laux += radiModel/100;
          raux -= radiModel/100;
          baux += radiModel/100;
          taux -= radiModel/100;

      }
      else
      {
        raux += radiModel/100;
        laux -= radiModel/100;
        baux -= radiModel/100;
        taux += radiModel/100;
      }
      resizeOrto();
    }
    else{

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

    }
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
        if(e->x() > position.x())
        {
            if(primera){
              ++xIni;
              if(xIni == 360) xIni = 0;
              degreesX = xIni*one_degree;
              emit updatex(xIni);
            }
            else if(tercera){
                ++thitaIni;
                if(thitaIni == 360) thitaIni = 0;
                thita = thitaIni*one_degree;
                viewTransform();
                emit updatethita(thitaIni);
            }

        }
        else{

            if(primera){
              --xIni;
              if(xIni == -1) xIni = 359;
              degreesX = yIni*one_degree;
              emit updatey(yIni);
            }
            else if(tercera){
                --thitaIni;
                if(thitaIni == -1) thitaIni = 359;
                thita = thitaIni*one_degree;
                viewTransform();
                emit updatethita(thitaIni);
            }
        }

        if(e->y() > position.y()){
            if(primera){
              ++yIni;
              if(yIni == 360) yIni = 0;
              degreesY = yIni * one_degree;
              emit updatey(yIni);
            }
            else if(tercera){
                ++psiIni;
                if(psiIni == 360) psiIni = 0;
                psi = psiIni*one_degree;
                viewTransform();
                emit updatepsi(psiIni);
            }
        }
        else{
            if(primera){
              --yIni;
              if(yIni == -1) yIni = 359;
              degreesY = yIni * one_degree;
              emit updatey(yIni);
            }
            else if(tercera){
                --psiIni;
                if(psiIni == -1) psiIni = 359;
                psi = psiIni*one_degree;
                viewTransform();
                emit updatepsi(psiIni);
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
      emit updateEscalat(scale);
      break;
    }
    case Qt::Key_D: { // escalar a més petit
      scale -= 0.05;
      emit updateEscalat(scale);
      break;
    }
    case Qt::Key_P: {
        ini_camera();
        //signal
        break;
    }
    case Qt::Key_T: {
        ini_camera_3a_persona();
        //signal
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

    FOV = FOVini = (float)M_PI/2.0f;
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
  transform = glm::scale(transform, glm::vec3(scale));
  transform = glm::translate(transform, glm::vec3(1.0,0.0,1.0));
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
  transform = glm::translate(transform, glm::vec3(-1.0,0.0,-1.0));
  transform = glm::scale(transform, glm::vec3(scalePat1));

  transform = glm::rotate(transform,degreesX,y);
  transform = glm::rotate(transform,(float)M_PI,y);

  transform = glm::translate(transform,-CBMPat);

  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::modelTransformLego1()
{
  // Matriu de transformació de model
  glm::mat4 transform (1.0f);
  transform = glm::scale(transform, glm::vec3(scale));
  transform = glm::translate(transform, glm::vec3(1.0,0.0,1.0));
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
  transform = glm::translate(transform, glm::vec3(-1.0,0.0,-1.0));
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
  transform = glm::translate(transform, glm::vec3(1.0,0.0,1.0));

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
  transform = glm::translate(transform, glm::vec3(-1.0,0.0,-1.0));

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


void MyGLWidget::modelTransformFloor()
{
    glm::mat4 transform (1.0f);
    transform = glm::scale(transform, glm::vec3(scale));
    glUniformMatrix4fv(transLoc,1,GL_FALSE, &transform[0][0]);
}

void MyGLWidget::evitaDeformacions(int w, int h)
{
    ra = double(w)/double(h);
    if(ra < 1.0f) FOV = 2*atan(tan(FOVini/2)/ra);

    if(ortogonal) resizeOrto();
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
    if(auxP < v){
        FOVini -= (v-auxP)*one_degree;
        FOV -= (v-auxP)*one_degree;

    }
    else{
        FOVini += (auxP-v)*one_degree;
        FOV += (auxP-v)*one_degree;

    }
    if(FOVini < 15*one_degree) FOVini = 15*one_degree;
    else if(FOVini > 160*one_degree) FOVini = 160*one_degree;

    if(FOV < 15*one_degree) FOV = 15*one_degree;
    else if(FOV > 160*one_degree) FOV = 160*one_degree;

    projectTransformPerspectiva();

    auxP = v;

    update();
}

void MyGLWidget::ZoomOrtogonal(int v){
  makeCurrent();
  if(auxO < v){
      laux += radiModel/40;
      raux -= radiModel/40;
      baux += radiModel/40;
      taux -= radiModel/40;

  }
  else{
      raux += radiModel/40;
      laux -= radiModel/40;
      baux -= radiModel/40;
      taux += radiModel/40;
  }
  resizeOrto();
  projectTransformOrtogonal();
  auxO = v;
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

void MyGLWidget::Primera(bool b){
    makeCurrent();
    if(b){
      ini_camera();
      primera = true;
      tercera = false;
    }
    update();
}

void MyGLWidget::Tercera(bool b){
  if(b){
    ini_camera_3a_persona();
    primera = false;
    tercera = true;
  }
}

void MyGLWidget::CarregaModel(QString str)
{
    makeCurrent();
    if(str == "Patricio") model = "Patricio";
    else if(str == "Legoman") model = "Legoman";
    else if(str == "Vaca") model = "Vaca";
    update();
}

void MyGLWidget::updateThita(int value){
    makeCurrent();
    thita = value*one_degree;
    viewTransform();
    update();
}

void MyGLWidget::updatePsi(int value){
    makeCurrent();
    psi = value*one_degree;
    viewTransform();
    update();
}

void MyGLWidget::updateX(int value){
    makeCurrent();
    degreesX = value*one_degree;
    update();
}

void MyGLWidget::updateY(int value){
    makeCurrent();
    degreesY = value*one_degree;
    update();
}

void MyGLWidget::setEscalat(double value){
    makeCurrent();
    scale = value;
    update();
}
