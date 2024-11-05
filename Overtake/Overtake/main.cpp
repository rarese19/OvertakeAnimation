#include <windows.h> 
#include <stdlib.h>        
#include <stdio.h>
#include <GL/glew.h>        
#include <GL/freeglut.h>    

#include "loadShaders.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "SOIL.h"
#include <cmath>

GLuint
	VaoId1, VaoId2, VaoId3, VaoId4,
	VboId1, VboId2, VboId3, VboId4,
	ColorBufferId1, ColorBufferId2,
	EboId1, EboId2, EboId3, EboId4,
	TextureId1, TextureId2, TextureId3, TextureId4,
	ProgramId,
	myMatrixLocation,
	codColLocation;

GLuint texture1, texture2, texture3, texture4, codCol;

GLfloat
	winWidth = 500, winHeight = 800;

glm::mat4
	myMatrix, resizeMatrix, lineTransMatrix, car1TransMatrix, car2TransMatrix, car1RotationMatrix, car2RotationMatrix, toCenterTransMatrix, fromCenterTransMatrix;

float xMin = -250.f, xMax = 250.f, yMin = -400.f, yMax = 400.f, middleLineY = -420.f, middleLineIncreaseY = 30.0, middleLineYOffset = 0.0,
car1Y = -400.0, car2Y = -470.0, defaultSpeed = 0.03, carOvertakeSpeed = 0.09, car1YTranslation = 0.0,
car2YTranslation = 0.0, xOvertakeTranslation = 0.0, car1RotationAngle = 0.0f, car2RotationAngle = 0.0f, rotationSpeed = 0.0002f, 
car1XCenter = 27.5, car1YCenter = -375.0, car2XCenter = 27.5, car2YCenter = -445.0;

double counter;
bool overtake = true;

void LoadTexture(const char* texturePath, GLuint& texture)
{
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	int width, height;
	unsigned char* image = SOIL_load_image(texturePath, &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);
}


void CreateShaders(void)
{
	ProgramId = LoadShaders("proiect_Shader.vert", "proiect_Shader.frag");
	glUseProgram(ProgramId);
}

void CreateVAO1(void)
{
 	static const GLfloat Vertices1[] =
	{
		-250.f, -400.f, 0.f, 1.f,
		-50.f,  -400.f, 0.f, 1.f,
		-50.f,   400.f, 0.f, 1.f,
		-250.f,  400.f, 0.f, 1.f,

		 50.f,  -400.f, 0.f, 1.f,
		 250.f, -400.f, 0.f, 1.f,
		 250.f,  400.f, 0.f, 1.f,
		 50.f,   400.f, 0.f, 1.f,
	};

	static const GLfloat Colors1[] =
	{
		0.5f, 0.5f, 0.5f, 1.0f,
		0.6f, 0.6f, 0.6f, 1.0f,
		0.7f, 0.7f, 0.7f, 1.0f,
		0.9f, 0.9f, 0.9f, 1.0f,
	};
	
	static const GLuint Indices1[] =
	{
		0, 1, 2, 3, 4, 5, 6, 7
	};

	glGenVertexArrays(1, &VaoId1);
	glBindVertexArray(VaoId1);

	glGenBuffers(1, &VboId1);
	glBindBuffer(GL_ARRAY_BUFFER, VboId1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices1), Vertices1, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

	glGenBuffers(2, &ColorBufferId1);
	glBindBuffer(GL_ARRAY_BUFFER, ColorBufferId1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Colors1), Colors1, GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);

	glGenBuffers(3, &EboId1);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EboId1);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices1), Indices1, GL_STATIC_DRAW);
 
}

void CreateVAO2(void)
{
	static const GLfloat Vertices2[] =
	{
		 -50.f, -400.f, 0.f, 1.f,
		  50.f, -400.f, 0.f, 1.f,
		  50.f,  400.f, 0.f, 1.f,
		 -50.f,  400.f, 0.f, 1.f,

		 -5.f, -420.f, 0.f, 1.f,
		  5.f, -420.f, 0.f, 1.f,
		  5.f, -400, 0.f, 1.f,
		 -5.f, -400.f, 0.f, 1.f
	};

	static const GLfloat Colors2[] =
	{
		0.5f, 0.5f, 0.5f, 1.0f,
		0.6f, 0.6f, 0.6f, 1.0f,
		0.7f, 0.7f, 0.7f, 1.0f,
		0.9f, 0.9f, 0.9f, 1.0f,
	};

	static const GLuint Indices2[] =
	{
		0, 1, 2, 3, 4, 5, 6, 7
	};

	glGenVertexArrays(1, &VaoId2);
	glBindVertexArray(VaoId2);
 
	glGenBuffers(1, &VboId2);
	glBindBuffer(GL_ARRAY_BUFFER, VboId2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices2), Vertices2, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

	glGenBuffers(2, &ColorBufferId2);
	glBindBuffer(GL_ARRAY_BUFFER, ColorBufferId2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Colors2), Colors2, GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);

	glGenBuffers(3, &EboId2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EboId2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices2), Indices2, GL_STATIC_DRAW);
}

void CreateVAO3(void)
{
	static const GLfloat Vertices3[] =
	{
		 0.f, -400.f, 0.f, 1.f,
		 55.f, -400.f, 0.f, 1.f,
		 55.f, -350.f, 0.f, 1.f,
		 0.f, -350.f, 0.f, 1.f,
		  
	};

	static const GLfloat Textures3[] =
	{
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f
	};

	static const GLuint Indices3[] =
	{
		0, 1, 2, 3
	};

	glGenVertexArrays(1, &VaoId3);
	glBindVertexArray(VaoId3);

	glGenBuffers(1, &VboId3);
	glBindBuffer(GL_ARRAY_BUFFER, VboId3);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices3), Vertices3, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

	glGenBuffers(2, &TextureId1);
	glBindBuffer(GL_ARRAY_BUFFER, TextureId1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Textures3), Textures3, GL_STATIC_DRAW);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glGenBuffers(3, &EboId3);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EboId3);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices3), Indices3, GL_STATIC_DRAW);
}

void CreateVAO4(void)
{
	static const GLfloat Vertices4[] =
	{
		 0.f, -470.f, 0.f, 1.f,
		 55.f, -470.f, 0.f, 1.f,
		 55.f, -420.f, 0.f, 1.f,
		 0.f, -420.f, 0.f, 1.f,
	};

	static const GLfloat Textures4[] =
	{
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f
	};

	static const GLuint Indices4[] =
	{
		0, 1, 2, 3
	};

	glGenVertexArrays(1, &VaoId4);
	glBindVertexArray(VaoId4);

	glGenBuffers(1, &VboId4);
	glBindBuffer(GL_ARRAY_BUFFER, VboId4);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices4), Vertices4, GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

	glGenBuffers(2, &TextureId2);
	glBindBuffer(GL_ARRAY_BUFFER, TextureId2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Textures4), Textures4, GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glGenBuffers(3, &EboId4);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EboId4);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices4), Indices4, GL_STATIC_DRAW);
}

void DestroyShaders(void)
{
	glDeleteProgram(ProgramId);
}

void DestroyVBO(void)
{
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &VboId1);
	glDeleteBuffers(1, &EboId1);
	glDeleteBuffers(1, &ColorBufferId1);

	glDeleteBuffers(1, &VboId2);
	glDeleteBuffers(1, &EboId2);
	glDeleteBuffers(1, &ColorBufferId2);

	glDeleteBuffers(1, &VboId3);
	glDeleteBuffers(1, &EboId3);
	glDeleteBuffers(1, &TextureId1);
	glDeleteBuffers(1, &TextureId3);

	glDeleteBuffers(1, &VboId4);
	glDeleteBuffers(1, &EboId4);
	glDeleteBuffers(1, &TextureId2);	
	glDeleteBuffers(1, &TextureId4);	

	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VaoId1);
	glDeleteVertexArrays(1, &VaoId2);
	glDeleteVertexArrays(1, &VaoId3);
	glDeleteVertexArrays(1, &VaoId4);
}

void Cleanup(void)
{
	DestroyShaders();
	DestroyVBO();
}

void Idle(void) {
	if (car1Y > 400.0) {
		car1YTranslation = -50.0;
		car1Y = -450.0;
		car1YCenter = -425.0;
	}
	else {
		car1YTranslation += defaultSpeed;
		car1Y += defaultSpeed;
		car1YCenter += defaultSpeed;
	}
	car1TransMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, car1YTranslation, 0.0));

	if (car2Y > 400.0) {
		car2YTranslation = 20.0;
		car2Y = -450.0;
		car2YCenter = -425.0;
	}
	else {
		car2YTranslation += defaultSpeed;
		car2Y += defaultSpeed;
		car2YCenter += defaultSpeed;
	}
	car2TransMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, car2YTranslation, 0.0));

	glutPostRedisplay();
}

void Overtake(void) {
	if (car1Y > 400.0) {
		car1YTranslation = -50.0;
		car1Y = -450.0;
		car1YCenter = -425.0;
	} else {
		if (counter >= 100.0 && counter <= 300.0 && !overtake) {
			car1YTranslation += carOvertakeSpeed;
			car1Y += carOvertakeSpeed;
			car1YCenter += carOvertakeSpeed;
		} else {
			car1YTranslation += defaultSpeed;
			car1Y += defaultSpeed;
			car1YCenter += defaultSpeed;
		}
	}

	if (car2Y > 400.0) {
		car2YTranslation = 20.0;
		car2Y = -450.0;
		car2YCenter = -425.0;
	} else {
		if (counter >= 100.0 && counter <= 300.0 && overtake) {
			car2YTranslation += carOvertakeSpeed;
			car2Y += carOvertakeSpeed;
			car2YCenter += carOvertakeSpeed;
		} else {
			car2YTranslation += defaultSpeed;
			car2Y += defaultSpeed;
			car2YCenter += defaultSpeed;
		}
	}

	if (counter >= 100.0 && counter <= 300.0) {
		counter -= carOvertakeSpeed;
	} else {
		counter -= defaultSpeed;
	}

	if (counter >= 300.0 && counter <= 400.0 && !overtake) {
		xOvertakeTranslation -= 0.018;
		car1XCenter -= 0.018;
		if (counter >= 350.0) {
			car1RotationAngle += rotationSpeed;
		} else {
			car1RotationAngle -= rotationSpeed;
		}
		car1TransMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(xOvertakeTranslation, car1YTranslation, 0.0));
		car1RotationMatrix = glm::rotate(glm::mat4(1.0f), car1RotationAngle, glm::vec3(0.0, 0.0, 1.0f));
		toCenterTransMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-car1XCenter, -car1YCenter, 0.0));
		fromCenterTransMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(car1XCenter, car1YCenter, 0.0));
	} else if (counter >= 100.0 && counter <= 300.0 && !overtake) {
		car1TransMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(xOvertakeTranslation, car1YTranslation, 0.0));
		car1RotationMatrix = glm::mat4(1.0f);
		toCenterTransMatrix = glm::mat4(1.0f);
		fromCenterTransMatrix = glm::mat4(1.0f);
		car1RotationAngle = 0.0f;
	} else if (counter >= 0.0 && counter <= 100.0 && !overtake) {
		xOvertakeTranslation += 0.018;
		car1XCenter += 0.018;
		car1TransMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(xOvertakeTranslation, car1YTranslation, 0.0));
		if (counter >= 50.0) {
			car1RotationAngle += rotationSpeed;
		} else {
			car1RotationAngle -= rotationSpeed;
		}
		car1RotationMatrix = glm::rotate(glm::mat4(1.0f), -car1RotationAngle, glm::vec3(0.0, 0.0, 1.0f));
		toCenterTransMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-car1XCenter, -car1YCenter, 0.0));
		fromCenterTransMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(car1XCenter, car1YCenter, 0.0));
	} else if (!overtake) {
		car1RotationMatrix = glm::mat4(1.0f);
		car2RotationAngle = 0.0;
		xOvertakeTranslation = 0.018;
		overtake = true;
		glutIdleFunc(Idle);
		return;
	}
	if (overtake) {
		car1TransMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, car1YTranslation, 0.0));
	}

	if (counter >= 300.0 && counter <= 400.0 && overtake) {
		xOvertakeTranslation -= 0.018;
		car2XCenter -= 0.018;
		car2TransMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(xOvertakeTranslation, car2YTranslation, 0.0));
		if (counter >= 350.0) {
			car2RotationAngle += rotationSpeed;

		} else {
			car2RotationAngle -= rotationSpeed;
		}
		car2RotationMatrix = glm::rotate(glm::mat4(1.0f), car2RotationAngle, glm::vec3(0.0, 0.0, 1.0f));
		toCenterTransMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-car2XCenter, -car2YCenter, 0.0));
		fromCenterTransMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(car2XCenter, car2YCenter, 0.0));
	} else if (counter >= 100.0 && counter <= 300.0 && overtake) {
		car2TransMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(xOvertakeTranslation, car2YTranslation, 0.0));
		car2RotationMatrix = glm::mat4(1.0f);
		toCenterTransMatrix = glm::mat4(1.0f);
		fromCenterTransMatrix = glm::mat4(1.0f);
		car2RotationAngle = 0.0f;
	} else if (counter >= 0.0 && car1Y <= 100.0 && overtake) {
		xOvertakeTranslation += 0.018;
		car2XCenter += 0.018;
		car2TransMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(xOvertakeTranslation, car2YTranslation, 0.0));
		if (counter >= 50.0) {
			car2RotationAngle += rotationSpeed;
		} else {
			car2RotationAngle -= rotationSpeed;
		}
		car2RotationMatrix = glm::rotate(glm::mat4(1.0f), -car2RotationAngle, glm::vec3(0.0, 0.0, 1.0f));
		toCenterTransMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-car2XCenter, -car2YCenter, 0.0));
		fromCenterTransMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(car2XCenter, car2YCenter, 0.0));
	} else if (overtake) {
		car2RotationMatrix = glm::mat4(1.0f);
		car2RotationAngle = 0.0;
		xOvertakeTranslation = 0.018;
		overtake = false;	
		glutIdleFunc(Idle);
	}
	if (!overtake) {
		car2TransMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, car2YTranslation, 0.0));
	}

	glutPostRedisplay();
}

// discontinuous line
void DrawRoadLine(void) {
	codCol = 3;
	glUniform1i(codColLocation, codCol);
	if (middleLineYOffset >= 30.0) {
		middleLineYOffset = 0.0;
	}
	while (middleLineY <= 400.0f) {
		lineTransMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, middleLineIncreaseY - middleLineYOffset, 0.0));
		myMatrix = resizeMatrix * lineTransMatrix;
		glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
		glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, (void*)(4 * sizeof(GLuint)));
		middleLineIncreaseY += 30.0;
		middleLineY += 30.0f;
	}
	middleLineIncreaseY = 30.0;
	middleLineY = -420.0f;
	middleLineYOffset += 0.06;
	lineTransMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, middleLineIncreaseY, 0.0));
}

void UseKeyboard(unsigned char key, int x, int y) {
	switch (key) {
		case 32: {
			if (counter > 0 && counter != 400) {
				break;
			}
			counter = 400.0;
			glutIdleFunc(Overtake);
			break;
		}
	}
}

void Initialize(void)
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	CreateVAO1();								
	CreateVAO2();
	CreateVAO3();
	CreateVAO4();

	LoadTexture("car1.png", texture1);
	LoadTexture("car2.png", texture2);
	LoadTexture("car1blinker.png", texture3);
	LoadTexture("car2blinker.png", texture4);

	CreateShaders();

	myMatrixLocation = glGetUniformLocation(ProgramId, "myMatrix");
	codColLocation = glGetUniformLocation(ProgramId, "codColShader");

	resizeMatrix = glm::ortho(xMin, xMax, yMin, yMax);
	lineTransMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, middleLineIncreaseY, 0.0));
	car1TransMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, car1YTranslation, 0.0));
	car2TransMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, car2YTranslation, 0.0));

	car1RotationMatrix = glm::mat4(1.0f);
	car2RotationMatrix = glm::mat4(1.0f);
	toCenterTransMatrix = glm::mat4(1.0f);
	fromCenterTransMatrix = glm::mat4(1.0f);
}

void RenderFunction(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	myMatrix = resizeMatrix;
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);

	// both sides of the grass
	glBindVertexArray(VaoId1);
	codCol = 1;
	glUniform1i(codColLocation, codCol);
	glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, (void*)(0));
	glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, (void*)(4 * sizeof(GLuint)));

	// the road
	glBindVertexArray(VaoId2);
	codCol = 2;
	glUniform1i(codColLocation, codCol);
	glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, (void*)(0));

	// function that renders the discontinuous line
	DrawRoadLine();

	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// white car
	glBindVertexArray(VaoId3);
	codCol = 4;
	glUniform1i(codColLocation, codCol);
	myMatrix = resizeMatrix * fromCenterTransMatrix * car1RotationMatrix * toCenterTransMatrix * car1TransMatrix;
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);

	// deciding whether the texture with the blinkers on should be rendered
	if (!overtake) {
		if (counter != 400.0 && counter > 0) {
			int modValue = (counter >= 100 && counter <= 300) ? 120 : 40; // since the car goes 3x faster when it overtakes, the blinker texture should be rendered 3 times less frequently to match the increased speed
			if (((int)round(counter) / modValue) % 2 == 0) {
				glBindTexture(GL_TEXTURE_2D, texture3);
			} else {
				glBindTexture(GL_TEXTURE_2D, texture1);
			}
		} else {
			glBindTexture(GL_TEXTURE_2D, texture1);
		}
	} else {
		glBindTexture(GL_TEXTURE_2D, texture1);
	}
	glUniform1i(glGetUniformLocation(ProgramId, "myTexture"), 0);

	glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, (void*)(0));

	// red car
	glBindVertexArray(VaoId4);
	codCol = 4;
	glUniform1i(codColLocation, codCol);
	myMatrix = resizeMatrix * fromCenterTransMatrix * car2RotationMatrix * toCenterTransMatrix * car2TransMatrix;
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);

	// deciding whether the texture with the blinkers on should be rendered
	if (overtake) {
		if (counter != 400.0 && counter > 0) {
			int modValue = (counter >= 100 && counter <= 300) ? 120 : 40; // since the car goes 3x faster when it overtakes, the blinker texture should be rendered 3 times less frequently to match the increased speed
			if (((int)round(counter) / modValue) % 2 == 0) {
				glBindTexture(GL_TEXTURE_2D, texture4);
			} else {
				glBindTexture(GL_TEXTURE_2D, texture2);
			}
		} else {
			glBindTexture(GL_TEXTURE_2D, texture2);
		}
	} else {
		glBindTexture(GL_TEXTURE_2D, texture2);
	}
	glUniform1i(glGetUniformLocation(ProgramId, "myTexture"), 0);

	glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, (void*)(0));

	glDisable(GL_BLEND);
	glutSwapBuffers();
	glFlush();
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(winWidth, winHeight);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Overtake");

	glewInit();

	Initialize();
	glutIdleFunc(Idle);
	glutDisplayFunc(RenderFunction);
	glutKeyboardFunc(UseKeyboard);
	glutCloseFunc(Cleanup);
	glutMainLoop();

	return 0;
}