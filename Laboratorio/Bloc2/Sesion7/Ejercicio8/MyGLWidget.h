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

    void Primera(bool b);

    void Tercera(bool b);

    void updateThita(int value);

    void updatePsi(int value);

    void updateX(int value);

    void updateY(int value);

    void setEscalat(double value);

    void setRed(int color);

    void setGreen(int color);

    void setBlue(int color);

    void setSong(QString song);

  signals:
    //Radio Buttons
    void setPressP(bool pr);
    void setPress(bool pr);
    void setPressPrimera(bool pr);
    void setPressTercera(bool pr);
    //Dials
    void updatepsi(int val);
    void updatethita(int val);
    void updatex(int val);
    void updatey(int val);

    void updateEscalat(double val);

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

    void modelTransformHomer1 ();
    void modelTransformHomer2 ();

    void modelTransformLego1 ();
    void modelTransformLego2 ();

    void modelTransformVaca2 ();
    void modelTransformVaca1 ();

    void modelTransformF161 ();
    void modelTransformF162 ();

    void modelTransformPorsche1 ();
    void modelTransformPorsche2 ();

    void modelTransformFloor ();

    void projectTransformOrtogonal ();
    void projectTransformPerspectiva();

    void viewTransform ();

    void ini_camera ();
    void ini_camera_3a_persona();

    void carregaModelPatricio();
    void carregaModelHomer();
    void carregaModelLegoman();
    void carregaModelVaca();
    void carregaModelF16();
    void carregaModelPorsche();
    void carregaFloor ();

    void pinta_patricio ();
    void pinta_homer();
    void pinta_legoman();
    void pinta_vaca();
    void pinta_f16();
    void pinta_porsche();
    void pinta_floor ();

    void evitaDeformacions (int w, int h);

    void gira();

    void calculaCapsaPatricio();
    void calculaCentreModelPat();

    void calculaCapsaHomer();
    void calculaCentreModelHomer();

    void calculaCapsaLegoman();
    void calculaCentreModelLego();

    void calculaCapsaVaca();
    void calculaCentreModelVaca();

    void calculaCapsaF16();
    void calculaCentreModelF16();

    void calculaCapsaPorsche();
    void calculaCentreModelPorsche();

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
    GLuint VAO_Homer, VBO_HomerPos, VBO_HomerCol;
    GLuint VAO_Legoman, VBO_LegomanPos, VBO_LegomanCol;
    GLuint VAO_Vaca, VBO_VacaPos, VBO_VacaCol;
    GLuint VAO_Floor, VBO_FloorPos, VBO_FloorCol;
    GLuint VAO_Porsche, VBO_PorschePos, VBO_PorscheCol;
    GLuint VAO_F16, VBO_F16Pos, VBO_F16Col;

    glm::vec3 Vertices[4];

    // Program
    QOpenGLShaderProgram *program;


    // Internal vars
    bool primera;
    bool tercera;
    bool song;
    bool gir;
    bool ortogonal;

    float red, green, blue;

    // Slots Variables
    int auxP;
    int auxO;

    QString model;
    QString filename;
    QMediaPlayer player;
    float one_degree;

    float scaleM;
    float scalePat1;
    float scaleHomer;
    float scaleLego;
    float scaleVaca;
    float scaleF16;
    float scalePorsche;
    //Variables Model

    Model patricio;
    Model homer;
    Model legoman;
    Model vaca;
    Model f16;
    Model porsche;

    //Variable de transformació
    QPoint position;
    float scale;
    float degreesX;
    float degreesY;
    int xIni;
    int yIni;

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
    int thitaIni;
    int psiIni;

    glm::vec3 pos;
    glm::vec3 OBS;
    glm::vec3 VRP;
    glm::vec3 UP;


    //Eixos
    glm::vec3 x;
    glm::vec3 y;
    glm::vec3 z;

    //Variables càmara 3a persona
    glm::vec3 EsferaMaximaPat;
    glm::vec3 EsferaMinimaPat;
    glm::vec3 centreModelPat;
    glm::vec3 CBMPat;

    glm::vec3 EsferaMaximaHomer;
    glm::vec3 EsferaMinimaHomer;
    glm::vec3 centreModelHomer;
    glm::vec3 CBMHomer;

    glm::vec3 EsferaMaximaLego;
    glm::vec3 EsferaMinimaLego;
    glm::vec3 centreModelLego;
    glm::vec3 CBMLego;

    glm::vec3 EsferaMaximaVaca;
    glm::vec3 EsferaMinimaVaca;
    glm::vec3 centreModelVaca;
    glm::vec3 CBMVaca;

    glm::vec3 EsferaMaximaF16;
    glm::vec3 EsferaMinimaF16;
    glm::vec3 centreModelF16;
    glm::vec3 CBMF16;

    glm::vec3 EsferaMaximaPorsche;
    glm::vec3 EsferaMinimaPorsche;
    glm::vec3 centreModelPorsche;
    glm::vec3 CBMPorsche;

    //vector normalitzat
    glm::vec3 v;

    float radiModel;
    float radiModelLego;
    float radiModelPat;
    float radiModelHomer;
    float radiModelVaca;
    float radiModelF16;
    float radiModelPorsche;
    float d;

    glm::vec3 origen;

};
