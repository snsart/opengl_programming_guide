//
//  main.cpp
//  openg_01
//
//  Created by snsart on 2021/9/13.
//  Copyright © 2021 snsart. All rights reserved.
//
#pragma clang diagnostic push
#pragma clang diagnostic ignored "_Wdocumentation"

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

using namespace std;

void init(GLFWwindow* window){}
void display(GLFWwindow* window,double currentTime){
    glClearColor(1.0, 0.1, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
}

int main(int argc, const char * argv[]) {
    
    if(!glfwInit()){
        exit(EXIT_FAILURE);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    GLFWwindow* window = glfwCreateWindow(600, 600, "opengl-01", NULL, NULL);
    glfwMakeContextCurrent(window);
    glewExperimental=GL_TRUE;
    if(glewInit()!=GLEW_OK){
        exit(EXIT_FAILURE);
    }
    glfwSwapInterval(1);
    init(window);
    while(!glfwWindowShouldClose(window)){
        display(window,glfwGetTime());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}



