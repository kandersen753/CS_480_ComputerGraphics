#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <iostream>
using namespace std;
#include <btBulletDynamicsCommon.h>

#include "graphics_headers.h"
#include "camera.h"
#include "shader.h"
#include "object.h"

class Graphics
{
  public:
    Graphics();
    ~Graphics();
    bool Initialize(int width, int height);
    void Update(unsigned int dt, vector <unsigned int>  keyPress, int force);
    void Render(vector <unsigned int>  keyPress);
    int collisionDetection(unsigned int dt);
    void reinitateBall();
    bool getGameState();

  private:
    std::string ErrorString(GLenum error);

    Camera *m_camera;
    Shader *gouraund_shader;
    Shader *phong_shader;

    GLint m_projectionMatrix;
    GLint m_viewMatrix;
    GLint m_modelMatrix;
    GLint ball;

    GLint m_scalar;
    GLint m_spot;
    GLint m_spec;
    GLint m_height;

    glm::vec3 scalar;
    glm::vec3 spot;
    GLfloat height;

    Object *m_table;
    Object *m_ball;
    Object *m_leftFlipper;
    Object *m_rightFlipper;
    Object *m_bump1;
    Object *m_bump2;
    Object *m_bump3;
    vector <Object*> numbers;
    vector <Object*> ScoreArray;
    vector <Object*> BallNum;
    glm::vec4 pos;

    btRigidBody* ceilingBody;
    btRigidBody* wallBody;

    uint score;
    int ballsLeft;
    bool resetable;
    bool ballCleared;
    int count1, count2, count3;
    bool gamestate;

    /* Bullet Variables */
    btBroadphaseInterface* broadphase;
    btDefaultCollisionConfiguration* collisionConfig;
    btCollisionDispatcher* dispatcher;
    btSequentialImpulseConstraintSolver* solver;
    btDiscreteDynamicsWorld* dynamicsWorld;

};

#endif /* GRAPHICS_H */
