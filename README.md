# Overtake Animation
 
## Requirements
OVERTAKE: Simulate an 'overtake': a car / rectangle moves uniformly (through translation), another rectangle comes from behind (also through translations / rotations), at a certain moment it starts overtaking, then it moves ahead of the first one.

## Project Concept
The animation simulates one car overtaking another, in a continuous manner (the car that is behind will be the one overtaking). The overtake action itself is triggered by pressing <code>Space</code> on the keyboard. When exiting the window at the top, both cars will reappear at the bottom, maintaining their order. 

## Demo 
![Gif](/gif.gif)

## Transformations

### Coordinate System Adjustment
This transformation defines a custom interval for coordinates in the rendering space, allowing for precise control over the visible area and how objects are displayed on the screen.

### Translation

This transformation is used for each car to manage their movement along the road, allowing for real-time adjustments in position. It also applies to the discontinuous road line, enabling a realistic representation of gaps or interruptions in the driving path.

### Incremental Rotation

This transformation is used for both cars when entering the overtake maneuver. By being incremental, it allows for smooth and continuous motion, ensuring a natural transition as the cars adjust their orientation.

## Originality

This animation is original because it seamlessly integrates continuous motion with user interactivity, allowing users to trigger overtaking maneuvers by pressing the <code>space bar</code>. The consistent behavior where the rear car always overtakes the front car introduces a unique dynamic to the scene. Additionally, the continuous looping of the cars reappearing at the bottom after exiting the top maintains uninterrupted motion. The moving discontinuous line effectively creates the illusion of speed, enhancing the overall visual experience.

## Rendered Objects

The objects drawn were grouped into different <code>VAOs</code>: one for the grass, one for the road and the discontinuous line (only one rectangle drawn multiple times at each render), and one for each car. The approach was used for modularity.

### Example - The VAO for one car (with texturing)

```cpp
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
```

### The discontinuous line

In order to render the discontinuous line, only 4 vertices were used and then translated on the Y axis. The rendering of the line stops when it reaches the top of the window. To achieve the illusion of speed, it is rendered with a small offset each time. This offset is reset each time it reaches a threshold.

```cpp
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
```

The function above is called in the <code>RenderFunction</code>, which is the <code>glutDisplayFunc</code>.

### Cars

In order to render the cars, I used 2 functions: <code>Idle</code> and <code>Overtake</code>. 

Initially, <code>Idle</code> is set as the <code>glutIdleFunc</code>.

```cpp
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
```

It checks if each car is above the top margin of the window, and then sets the translation matrix for each car. At the end, it triggers the <code>RenderFunction</code>.

The <code>UseKeyboard</code> function listens for keystrokes, and then switches the idle function if the pressed key is <code>Space</code>

```cpp
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
```

It also checks if the overtaking action is already happening, to prevent the same car from going off-road in case of multiple keystrokes. When the <code>Space</code> key is pressed, the <code>glutIdleFunc</code> changes and the overtaking starts.

```cpp
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
```

Initially, the red car (the one behind), is the one to overtake. The action is done within a certain "time". 

First, the car overtaking moves on the other lane. This implies the vertical translation, horizontal translation and incremental rotation. 

When the car is fully positioned, it accelerates and goes at a faster speed than the car in front of it. This requires a vertical translation but at a faster rate.

Lastly, the car returns to it's lane. To do so, it needs the vertical translation (back at original speed), horizontal translation back to the lane and incremental rotation.

We can see these actions performed in the [Demo](#demo).


### Texturing

In order to make the cars use their blinkers, I had to create two separate textures that have the blinkers on.

For each car, I checked to see if the blinkers needed to be used, and then switched between textures.

```cpp
if (overtake) {
    if (counter != 400.0 && counter > 0) {
        int modValue = (counter >= 100 && counter <= 300) ? 120 : 40; 
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
```

This example is for the red car. If it is the one overtaking, we need to start using the texture in which the blinker is on. 

### Car rendering

```cpp
glBindVertexArray(VaoId4);
codCol = 4;
glUniform1i(codColLocation, codCol);
myMatrix = resizeMatrix * fromCenterTransMatrix * car2RotationMatrix * toCenterTransMatrix * car2TransMatrix;
glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
```

Since I used <code>gludIdleFunc</code>, and both idle functions determine the transformation matrices, in the <code>RenderFunction</code> no additional calculations were needed. When a car is not overtaking, the matrices used for rotation are set to the identity matrix. 

### Fragment Shader

```code
#version 330 core

in vec4 ex_Color;
in vec2 tex_Coord;

out vec4 out_Color;		

uniform int codColShader;
uniform sampler2D myTexture;

vec4 black = vec4(0.0, 0.0, 0.0, 1.0);
vec4 green= vec4(0.0, 1.0, 0.0, 1.0);
vec4 red = vec4(1.0, 0.0, 0.0, 1.0);
vec4 blue = vec4(0.0, 0.0, 1.0, 1.0);


void main(void)
  {
    switch(codColShader) {
        case 1: {
            out_Color = mix(black, green, 0.6);
            break;
        }
        case 2: {
            out_Color = vec4(0.22f, 0.23f, 0.22f, 1.0f);
            break;
        }
        case 3: {
            out_Color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
            break;
        }
        case 4: {
            out_Color = texture(myTexture, tex_Coord);
            break;      
        }
        default: {
            out_Color = ex_Color;
            break;
        }
    }
  }
```

In order to determine whether the object needs colouring or texturing, the <code>codCol</code> variable is used, which is received from the <code>RenderFunction</code>. It also decides the color used.

### Vertex Shader

```code
#version 330 core

layout (location = 0) in vec4 in_Position;
layout (location = 1) in vec4 in_Color;
layout (location = 2) in vec2 texCoord;

out vec4 gl_Position;
out vec4 ex_Color;
out vec2 tex_Coord;

uniform mat4 myMatrix;

void main(void)
  {
    gl_Position = myMatrix*in_Position;
    ex_Color = in_Color;
    tex_Coord = vec2(texCoord.x, 1-texCoord.y);
  } 
```

