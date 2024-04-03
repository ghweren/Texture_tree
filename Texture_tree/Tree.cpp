#include <iostream>

#include <stdio.h>
#include "GL/glew.h"
#include "GL/glut.h"
#include "GLFW/glfw3.h"
#include "shader_s.h"
#include "stb_image.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// Константы
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{



	// start GL context and O/S window using the GLFW helper library
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		return 1;
	}

	// uncomment these lines if on Apple OS X
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(640, 480, "Hello Triangle", NULL, NULL);
	if (!window) {
		fprintf(stderr, "ERROR: could not open window with GLFW3\n");
		glfwTerminate();
		return 1;
	}
	glfwMakeContextCurrent(window);

	// start GLEW extension handler
	glewExperimental = GL_TRUE;
	glewInit();
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	// Компилирование нашей шейдерной программы
	Shader ourShader("4.1.texture.vs", "4.1.texture.fs");

	float crown_vertices[] = {
		//крона
		// координаты        // цвета            // текстурные координаты
		0.15f, 0.0f,0.0f,	  1.0f,0.0f,0.0f,	  1.0f,1.0f,//нижний правый угол
		-0.15f, 0.0f,0.0f,	  0.0f,1.0f,0.0f,	  1.0f,0.0f,//нижний левый угол
		0.0f, 0.7f,0.0f,	  0.0f,0.0f,1.0f,	  0.0f,1.0f,//верхний угол
	};
	// Указание вершин (и буфера(ов)) и настройка вершинных атрибутов
	float trunk_vertices[] = {
		// координаты        // цвета            // текстурные координаты
		0.05f,  0.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // верхняя правая вершина
		0.05f, -0.2f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // нижняя правая вершина
	   -0.05f, -0.2f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // нижняя левая вершина
	   -0.05f,  0.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // верхняя левая вершина 
	};
	unsigned int trunk_indices[] = {
		0, 1, 3, // первый треугольник
		1, 2, 3  // второй треугольник
	};
	unsigned int crown_indices[] = {
		0,1,2
	};
	unsigned int trunk_VBO, trunk_VAO, trunk_EBO;
	glGenVertexArrays(1, &trunk_VAO);
	glGenBuffers(1, &trunk_VBO);
	glGenBuffers(1, &trunk_EBO);


	glBindVertexArray(trunk_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, trunk_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(trunk_vertices), trunk_vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, trunk_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(trunk_indices), trunk_indices, GL_STATIC_DRAW);

	// Координатные атрибуты
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Цветовые атрибуты
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Атрибуты текстурных координат
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);


	// Загрузка и создание текстуры
	unsigned int trunk_texture;
	glGenTextures(1, &trunk_texture);
	glBindTexture(GL_TEXTURE_2D, trunk_texture); // все последующие GL_TEXTURE_2D-операции теперь будут влиять на данный текстурный объект

	// Установка параметров наложения текстуры
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // установка метода наложения текстуры GL_REPEAT (стандартный метод наложения)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Установка параметров фильтрации текстуры
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Загрузка изображения, создание текстуры и генерирование мипмап-уровней
	int width, height, nrChannels;
	unsigned char* data = stbi_load("textures/trunk.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	unsigned int crown_VBO, crown_VAO, crown_EBO;

	glGenVertexArrays(1, &crown_VAO);
	glGenBuffers(1, &crown_VBO);
	glGenBuffers(1, &crown_EBO);

	glBindVertexArray(crown_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, crown_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(crown_vertices), crown_vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, crown_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(crown_indices), crown_indices, GL_STATIC_DRAW);

	// Координатные атрибуты
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Цветовые атрибуты
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Атрибуты текстурных координат
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	unsigned int crown_texture;
	glGenTextures(1, &crown_texture);
	glBindTexture(GL_TEXTURE_2D, crown_texture); // все последующие GL_TEXTURE_2D-операции теперь будут влиять на данный текстурный объект

	// Установка параметров наложения текстуры
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // установка метода наложения текстуры GL_REPEAT (стандартный метод наложения)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Установка параметров фильтрации текстуры
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Загрузка и создание текстуры
	data = stbi_load("textures/tree.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	// Цикл рендеринга
	while (!glfwWindowShouldClose(window))
	{
		// Обработка ввода
		processInput(window);

		// Рендеринг
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Связывание текстуры
		glBindTexture(GL_TEXTURE_2D, trunk_texture);

		// Рендеринг ящика
		ourShader.use();
		glBindVertexArray(trunk_VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glBindTexture(GL_TEXTURE_2D, crown_texture);

		// Рендеринг ящика
		ourShader.use();
		glBindVertexArray(crown_VAO);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

		// glfw: обмен содержимым front- и back- буферов. Отслеживание событий ввода/вывода (была ли нажата/отпущена кнопка, перемещен курсор мыши и т.п.)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Опционально: освобождаем все ресурсы, как только они выполнили свое предназначение
	glDeleteVertexArrays(1, &trunk_VAO);
	glDeleteBuffers(1, &trunk_VBO);
	glDeleteBuffers(1, &trunk_EBO);

	// glfw: завершение, освобождение всех выделенных ранее GLFW-реурсов
	glfwTerminate();
	return 0;
}

// Обработка всех событий ввода: запрос GLFW о нажатии/отпускании кнопки мыши в данном кадре и соответствующая обработка данных событий
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

// glfw: всякий раз, когда изменяются размеры окна (пользователем или операционной системой), вызывается данная callback-функция
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// Убеждаемся, что окно просмотра соответствует новым размерам окна.
	// Обратите внимание, что высота и ширина будут значительно больше, чем указано, на Retina-дисплеях
	glViewport(0, 0, width, height);
}