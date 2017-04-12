#define GLM_FORCE_RADIANS
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QKeyEvent>
#include <QMediaPlayer>
#include <QtMultimediaWidgets>
#include <QLabel>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "model.h"

class MyGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
  Q_OBJECT

  public:
    MyGLWidget (QWidget *parent=0);
    ~MyGLWidget ();

  public slots:
    void ZoomPerspectiva(int v);

    void ZoomOrtogonal(int v);

    void Perspective(bool b);

    void Ortogonal(bool b);

    void CarregaModel(QString str);

  signals:
    void setPressP(bool pr);
    void setPress(bool pr);

  protected:
    // initializeGL - Aqui incluim les inicialitzacions del contexte grafic.
    virtual void initializeGL ( );
    // paintGL - Mètode cridat cada cop que cal refrescar la finestra.
    // Tot el que es dibuixa es dibuixa aqui.
    virtual void paintGL ( );
    // resizeGL - És cridat quan canvia la mida del widget
    virtual void resizeGL (int width, int height);
    // keyPressEvent - Es cridat quan es prem una tecla
    virtual void keyPressEvent (QKeyEvent *event);

    virtual void mouseMoveEvent(QMouseEvent *e);

    virtual void mousePressEvent(QMouseEvent *e);

    virtual void wheelEvent (QWheelEvent * e);

  private:
    void createBuffers ();
    void carregaShaders ();

    void modelTransformPat1 ();
    void modelTransformPat2 ();

    void modelTransformLego1 ();
    void modelTransformLego2 ();

    void modelTransformVaca2 ();
    void modelTransformVaca1 ();

    void modelTransformFloor ();

    void projectTransformOrtogonal ();
    void projectTransformPerspectiva();

    void viewTransform ();

    void ini_camera ();
    void ini_camera_3a_persona();

    void carregaModelPatricio ();
    void carregaModelLegoman();
    void carregaModelVaca();
    void carregaFloor ();

    void pinta_patricio ();
    void pinta_legoman();
    void pinta_vaca();
    void pinta_floor ();

    void evitaDeformacions (int w, int h);

    void gira();

    void calculaCapsaPatricio();
    void calculaCentreModelPat();

    void calculaCapsaLegoman();
    void calculaCentreModelLego();

    void calculaCapsaVaca();
    void calculaCentreModelVaca();

    void resizeOrto();

    // attribute locations
    GLuint vertexLoc;
    GLuint colorLoc;

    // uniform locations
    GLuint transLoc;
    GLuint projLoc;
    GLuint viewLoc;

    // VAO i VBO names
    GLuint VAO_Patricio, VBO_PatricioPos, VBO_PatricioCol;
    GLuint VAO_Legoman, VBO_LegomanPos, VBO_LegomanCol;
    GLuint VAO_Vaca, VBO_VacaPos, VBO_VacaCol;
    GLuint VAO_Floor, VBO_FloorPos, VBO_FloorCol;

    glm::vec3 Vertices[4];

    // Program
    QOpenGLShaderProgram *program;


    // Internal vars
    bool primera;
    bool tercera;
    bool sound;
    bool gir;
    bool ortogonal;

    // Slots Variables
    int auxP;
    int auxO;

    QString model;

    QMediaPlayer player;
    float one_degree;

    float scaleM;
    float scalePat1;
    float scaleLego;
    float scaleVaca;
    //Variables Model

    Model patricio;
    Model legoman;
    Model vaca;

    //Variable de transformació
    QPoint position;
    float scale;
    float degreesX;
    float degreesY;

    //Paràmetre inicials
    float FOVini;
    float raini;


    //Variables Project Matrix
    float FOV;
    float ra;
    float znear;
    float zfar;

    float left;
    float right;
    float bottom;
    float top;

    float laux;
    float raux;
    float baux;
    float taux;

    //Variables View Matrix
    float thita;
    float fi; //no cal
    float psi;

    glm::vec3 pos;
    glm::vec3 OBS;
    glm::vec3 VRP;
    glm::vec3 UP;


    //Eixos
    glm::vec3 x;
    glm::vec3 y;
    glm::vec3 z;

    glm::vec3 CBM;

    //Variables càmara 3a persona
    glm::vec3 EsferaMaximaPat;
    glm::vec3 EsferaMinimaPat;
    glm::vec3 centreModelPat;
    glm::vec3 CBMPat;

    glm::vec3 EsferaMaximaLego;
    glm::vec3 EsferaMinimaLego;
    glm::vec3 centreModelLego;
    glm::vec3 CBMLego;

    glm::vec3 EsferaMaximaVaca;
    glm::vec3 EsferaMinimaVaca;
    glm::vec3 centreModelVaca;
    glm::vec3 CBMVaca;

    //vector normalitzat
    glm::vec3 v;

    float radiModel;
    float radiModelLego;
    float radiModelPat;
    float radiModelVaca;
    float d;

    glm::vec3 origen;

};
