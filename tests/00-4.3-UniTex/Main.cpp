//#include "graphics\Context.h"
//#include "graphics\Vertex.h"
//#include "graphics\RenderObjects.h"
//#include "graphics\draw.h"
//#include <vector>
//using std::vector;
//#include <iostream>
//using std::cout;
//
//GLuint renderHandle, computeHandle;
//
//void updateTex(int);
//void draw();
//
//int main() {
//	//initGL();
//
//	GLuint *texHandle = makeTexture(7, 1, 3, pixels);
//	renderHandle = genRenderProg(texHandle);
//	computeHandle = genComputeProg(texHandle);
//
//	for (int i = 0; i < 1024; ++i) {
//		updateTex(i);
//		draw();
//	}
//
//	return 0;
//}
//
//void updateTex(int frame) {
//	glUseProgram(computeHandle);
//	glUniform1f(glGetUniformLocation(computeHandle, "roll"), (float)frame*0.01f);
//	glDispatchCompute(512 / 16, 512 / 16, 1); // 512^2 threads in blocks of 16^2
//	checkErrors("Dispatch compute shader");
//}
//
//void draw() {
//	glUseProgram(renderHandle);
//	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
//	swapBuffers();
//	checkErrors("Draw screen");
//}