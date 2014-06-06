/*
 * =====================================================================================
 *
 *       Filename:  meshshader_glsl.h
 *
 *    Description:  shader for mesh
 *
 *        Version:  1.0
 *        Created:  06/05/2014 15:28:24
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef  meshshader_glsl_INC
#define  meshshader_glsl_INC

string meshvert = R"(

   // ATTRIBUTES
   attribute vec3 position;
   attribute vec3 normal;
   attribute vec4 sourceColor;
   attribute vec2 texCoord;

   // UNIFORMS
   uniform vec3 lightPosition;
   uniform mat4 modelView;           // Model * View
   uniform mat4 projection;          // Projection Matrix (ortho or frustum)
   uniform mat4 normalMatrix;        // Normal Matrix (inverse transpose of mvm)

  // VARYING 
   varying vec4 colorDst;
   varying lowp vec2 texco;

  // NORMAL CALC
  vec3 doNormal(vec4 n) {
      return normalize( ( normalMatrix * n ).xyz );
  }


  // COLOR AND LIGHTING CALC
   vec4 doColor(){
       
       //Infinite Viewer
       vec3 L = normalize( lightPosition );
       vec3 N = doNormal( vec4(normal,1.0) );
       vec3 E = vec3(0,0,1); 
       vec3 H = normalize( L + E );
       
       //Diffusion Coefficient
       float df = max(0.0, dot(N,L) );

       //Specular Coefficient
       float sf = max(0.0, dot(L,H) );
           
       return vec4((sourceColor * (df + sf)).xyz , 1 );
   }

  //POSITION CALC
   vec4 doVertex (vec4 v) {
      mat4 m = projection * modelView;
      return m * v;
   }

   void main(void){
     
      vec4 colorfalse= doColor();  
      colorDst = vec4(normal.zyx, position.z);//sourceColor;
      //colorDst = vec4(1.0 - normal.x, 1.0 - normal.y, 1.0 - normal.z * normal.y, 1.0);//sourceColor;
      
      texco = texCoord;
                      
      vec4 pos =  vec4(position,1.0);
      vec4 nor = vec4(normal,1.0);
      
      //xm = submodel;
      
      gl_PointSize = 1.0;
      gl_Position = doVertex(pos);
   }

)";


string meshfrag = R"(

  uniform sampler2D sampleTexture;  
  varying vec4 colorDst;
  varying lowp vec2 texco;
 
  void main(void){
      
      vec4 litColor = colorDst;
      
      vec4 texColor = texture2D(sampleTexture, texco);
      
      gl_FragColor = litColor;     //mix(litColor, texColor, .5);
  }

)";


struct MeshProcess : public Process {

  
  MeshProcess(Renderer * r) : Process(0,0,r) { init(); }

  virtual void init(){
    this->program = new ShaderProgram( meshvert, DefaultFragES, 0);
    this->bindAll();
  }

  virtual void udpate(){}

  virtual void operator()(){

  }

};


#endif   /* ----- #ifndef meshshader_glsl_INC  ----- */
