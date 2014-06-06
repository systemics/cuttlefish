/*
 * =====================================================================================
 *
 *       Filename:  particleshader_glsl.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/05/2014 20:34:18
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *   Organization:  
 *
 * =====================================================================================
 */


#ifndef  particlshader_glsl_INC
#define  particlshader_glsl_INC

string particlevert = R"(

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
           
       return vec4((sourceColor * sf).xyz , 1 );
   }

  //POSITION CALC
   vec4 doVertex (vec4 v) {
      mat4 m = projection * modelView;
      return m * v;
   }

   void main(void){
     
      vec4 colorfalse= doColor();  
      colorDst = vec4(1.0+sourceColor.r, normal.g, normal.b, sourceColor.a + 2.0);
      //colorDst = vec4(1.0 - normal.x, 1.0 - normal.y * normal.y, 1.0 - normal.z * normal.y, 1.0);//sourceColor;
      
      texco = texCoord;
                      
      vec4 pos =  vec4(position,1.0);
      vec4 nor = vec4(normal,1.0);
      
      //xm = submodel;
      
      gl_PointSize = 1.0;
      gl_Position = doVertex(pos);
   }

)";


/* string meshfrag = R"( */

/*   uniform sampler2D sampleTexture; */  
/*   varying vec4 colorDst; */
/*   varying lowp vec2 texco; */
 
/*   void main(void){ */
      
/*       vec4 litColor = colorDst; */
      
/*       vec4 texColor = texture2D(sampleTexture, texco); */
      
/*       gl_FragColor = litColor;     //mix(litColor, texColor, .5); */
/*   } */

/* )"; */


struct ParticleProcess : public Process {

  
  ParticleProcess(Renderer * r) : Process(0,0,r) { init(); }

  virtual void init(){
    this->program = new ShaderProgram( particlevert, DefaultFragES, 0);
    this->bindAll();
  }

  virtual void udpate(){}

  virtual void operator()(){

  }

};


#endif   /* ----- #ifndef meshshader_glsl_INC  ----- */
