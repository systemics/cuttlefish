/*
 *  ctl_shader_manager.h
 *  Cuttlefish
 *  Created by Karl Yerkes on Sat May  4 13:33:23 PDT 2013
 *  Copyright 2013 Karl Yerkes. All rights reserved.
 *
*/
#ifndef __SHADER_MANAGER__
#define __SHADER_MANAGER__

#include <cassert>
#include <string>
#include <unordered_map>

namespace ctl {

template <typename t> class HashTable : public std::unordered_map<std::string, t> {};

class ShaderManager {
  struct Shader {
    GLuint programNumber;
    HashTable<GLuint> location;
  };

  HashTable<Shader> shader;
  string bound;

public:
  void bind(const char* programName) {
    assert (shader.find(programName) != shader.end());
    if (bound == programName)
      return;
    Shader* s = &shader[programName];
    glUseProgram(s->programNumber);
    bound = programName;
  }

  GLuint locationOf(const char* attributeName) {
    return shader[bound].location[attributeName];
  }

  void print(void) {
    for (HashTable<Shader>::iterator s = shader.begin(); s != shader.end(); ++s) {
      printf("%s:%d\n", s->first.c_str(), s->second.programNumber);
      for (HashTable<GLuint>::iterator a = s->second.location.begin(); a != s->second.location.end(); ++a) {
        printf("  %s:%d\n", a->first.c_str(), a->second);
      }
    }
  }

  void make(const char* programName, const char* vertexSource, const char* fragmentSource) {
    GLint status, logLength;
    GLuint shaderProgram = glCreateProgram();

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, 0);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0) {
      GLchar log[logLength];
      glGetShaderInfoLog(vertexShader, logLength, &logLength, log);
      printf("%s", log);
    }
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
    assert(status != 0);

    glAttachShader(shaderProgram, vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, 0);
    glCompileShader(fragmentShader);
    glGetProgramiv(fragmentShader, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0) {
      GLchar log[logLength];
      glGetShaderInfoLog(vertexShader, logLength, &logLength, log);
      printf("%s", log);
    }
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
    assert(status != 0);

    glAttachShader(shaderProgram, fragmentShader);

    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0) {
      GLchar log[logLength];
      glGetShaderInfoLog(vertexShader, logLength, &logLength, log);
      printf("%s", log);
    }
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &status);
    assert(status != 0);

    if (vertexShader)
      glDeleteShader(vertexShader);
    if (fragmentShader)
      glDeleteShader(fragmentShader);

    shader[programName];
    shader[programName].programNumber = shaderProgram;
    {
      GLint active_attribs, max_length;

      glGetProgramiv(shaderProgram, GL_ACTIVE_ATTRIBUTES, &active_attribs);
      glGetProgramiv(shaderProgram, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &max_length);

      for (int i = 0; i < active_attribs; i++) {
        GLint size;
        GLenum type;
        char name[max_length + 1];

        glGetActiveAttrib(shaderProgram, i, max_length + 1, NULL, &size, &type, name);

        shader[programName].location[name] = glGetAttribLocation(shaderProgram, name);
      }
    }
    {
      GLint active_uniforms, max_length;

      glGetProgramiv(shaderProgram, GL_ACTIVE_UNIFORMS, &active_uniforms);
      glGetProgramiv(shaderProgram, GL_ACTIVE_UNIFORM_MAX_LENGTH, &max_length);

      for (int i = 0; i < active_uniforms; i++) {
        GLint size;
        GLenum type;
        char name[max_length + 1];

        glGetActiveUniform(shaderProgram, i, max_length + 1, NULL, &size, &type, name);

        shader[programName].location[name] = glGetUniformLocation(shaderProgram, name);
      }
    }

    bind(programName);
  }
};

} // ctl::
#endif
