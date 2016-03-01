#include "Texture.h"
#include "stb_image.h"
#include <iostream>

Texture::Texture()
{
}

Texture::Texture(const char* fileName) {

	int t_width, t_height, t_numComponent;
	unsigned char* texture_data = stbi_load(fileName, &t_width, &t_height, &t_numComponent, 4);

	if (texture_data == NULL)
		std::cerr << "Texture loading failed!, for texture: " << fileName << std::endl;
	else {
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, t_width, t_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);

		stbi_image_free(texture_data);
		glBindTexture(GL_TEXTURE_2D,0);

	}

}

void Texture::Bind() {

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->texture);
}


Texture::~Texture()
{
	glDeleteTextures(1, &this->texture);
}
