#define GLM_FORCE_RADIANS
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QKeyEvent>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "model.h"

class MyGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core 
{
  Q_OBJECT

  public:
    MyGLWidget (QWidget *parent=0);
    ~MyGLWidget ();

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

  private:
    void createBuffers ();
    void carregaShaders ();
    void modelTransform ();
    void modelTransform2 ();
    void projectTransform ();
    void viewTransform ();
    void ini_camera ();
    void carregaModelHomer ();
    void carregaFloor ();
    void pinta_homer ();
    void pinta_floor ();
    void evitaDeformacions (int w, int h);
    void gira();
    void calculaVertexMaxMin();
    void calculaCentreModel();
    void ini_camera_3a_persona();
    
    // attribute locations
    GLuint vertexLoc;
    GLuint colorLoc;
 
    // uniform locations
    GLuint transLoc;
    GLuint projLoc;
    GLuint viewLoc;
    
    // VAO i VBO names
    GLuint VAO_Homer, VBO_HomerPos, VBO_HomerCol;
    GLuint VAO_Floor, VBO_FloorPos, VBO_FloorCol;
    
    // Program
    QOpenGLShaderProgram *program;
    
    
    // Internal vars
    
    //Variables Model
    Model homer;
    
    //Variable de transformació
    QPoint position;
    float scale;
    float degrees;
    
    //Variables Project Matrix
    float FOV;
    float ra;
    float znear;
    float zfar;
    
    //Variables View Matrix
    glm::vec3 pos;
    glm::vec3 OBS;
    glm::vec3 VRP;
    glm::vec3 UP;
    
    //Eixos
    glm::vec3 x;
    glm::vec3 y;
    glm::vec3 z;
    
    //Variables càmara 3a persona
    glm::vec3 Modelmaxim;
    glm::vec3 Modelminim;
    glm::vec3 centreModel;
    
    //vector normalitzat
    glm::vec3 v;
    
    float radiModel;
    float d;
    
};

