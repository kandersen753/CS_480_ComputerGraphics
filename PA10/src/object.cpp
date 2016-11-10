#include "object.h"


Object::Object(std::string filename, btScalar mass, btVector3 inertia, btVector3 startOrigin)
{  
  //Verticies and indicies needs to be initilized for run
  //Presumably we will call the assimp functions here
  scene = importer.ReadFile("../assets/" + filename, aiProcess_Triangulate);
  aiColor3D color (0.0f,0.0f, 0.0f);
  // create triangle mesh
  mTriMesh = new btTriangleMesh();

  for(unsigned int meshNums = 0; meshNums < scene->mNumMeshes; meshNums++){
    const aiMesh* mesh = scene->mMeshes[meshNums];
    // get material properties per mesh
    scene->mMaterials[meshNums+1]->Get(AI_MATKEY_COLOR_AMBIENT, color);
    glm::vec3 Ka = glm::vec3(color.r, color.g, color.b);
    scene->mMaterials[meshNums+1]->Get(AI_MATKEY_COLOR_SPECULAR, color);
    glm::vec3 Ks = glm::vec3(color.r, color.g, color.b);
    scene->mMaterials[meshNums+1]->Get(AI_MATKEY_COLOR_DIFFUSE, color);
    glm::vec3 Kd = glm::vec3(color.r, color.g, color.b);

    Color materialsColor (Ka,Kd, Ks);

    for(unsigned int vertex = 0; vertex < mesh->mNumVertices; vertex++){
      Vertices.push_back(Vertex(
                            glm::vec3(mesh->mVertices[vertex].x, 
                                      mesh->mVertices[vertex].y, 
                                      mesh->mVertices[vertex].z), 
                            glm::vec3(mesh->mNormals[vertex].x, 
                                      mesh->mNormals[vertex].y, 
                                      mesh->mNormals[vertex].z),
                            materialsColor));
    }

    for(unsigned int index = 0; index < mesh->mNumFaces; index++){
      Indices.push_back(mesh->mFaces[index].mIndices[0]);
      Indices.push_back(mesh->mFaces[index].mIndices[1]);
      Indices.push_back(mesh->mFaces[index].mIndices[2]);


      btVector3 v0 = btVector3( mesh->mVertices[ mesh->mFaces[index].mIndices[0]].x,
                                mesh->mVertices[ mesh->mFaces[index].mIndices[0]].y,
                                mesh->mVertices[ mesh->mFaces[index].mIndices[0]].z);

      btVector3 v1 = btVector3( mesh->mVertices[ mesh->mFaces[index].mIndices[1]].x,
                                mesh->mVertices[ mesh->mFaces[index].mIndices[1]].y,
                                mesh->mVertices[ mesh->mFaces[index].mIndices[1]].z);

      btVector3 v2 = btVector3( mesh->mVertices[ mesh->mFaces[index].mIndices[2]].x,
                                mesh->mVertices[ mesh->mFaces[index].mIndices[2]].y,
                                mesh->mVertices[ mesh->mFaces[index].mIndices[2]].z);                            

      mTriMesh->addTriangle (v0,v1,v2);
    }

      glGenBuffers(1, &VB);
      glBindBuffer(GL_ARRAY_BUFFER, VB);
      glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(),  &Vertices[0], GL_STATIC_DRAW);

      glGenBuffers(1, &IB);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER,  sizeof(unsigned int) * Indices.size(), &Indices[0], GL_STATIC_DRAW);
}

   // create collision shape
    btCollisionShape* shape = new btBvhTriangleMeshShape (mTriMesh, true);
    // vec3 sets the intial position of the collision object

    btDefaultMotionState* motion = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), startOrigin));
    
    // static bodies get a mass of 0
    btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(mass, motion, shape, inertia);
    body = new btRigidBody(rigidBodyCI);
    body->setActivationState (DISABLE_DEACTIVATION);


}

Object::~Object()
{
    Vertices.clear();
    Indices.clear();

}

void Object::setOrientation(int x){
  model = glm::translate(glm::mat4(1.0), glm::vec3(0.0f, (0.25f*x), 0.0f));

}


void Object::Update()
{

  btTransform trans;
  btScalar m[16];

  body->getMotionState()->getWorldTransform(trans);
  //std::cout << meshNumber  << ": " << trans.getOrigin().getX() << " " << trans.getOrigin().getY() << " " << trans.getOrigin().getZ() << std::endl;
  trans.getOpenGLMatrix(m);
  model = glm::make_mat4(m);
}


void Object::setBodyTransform(btVector3 position)
{
 // body->proceedToTransform(btTransform(btQuaternion(btScalar(0),btScalar(0),btScalar(0),btScalar(1)), position));
}

glm::mat4 Object::GetModel()
{
  return model;
}


btRigidBody* Object::GetRigidBody(){
  return body;
}

void Object::Render()
{
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);

  glBindBuffer(GL_ARRAY_BUFFER, VB);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,properties.Kd));

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

  glDrawArrays(GL_TRIANGLES, 0, Indices.size());
  
  glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
}
