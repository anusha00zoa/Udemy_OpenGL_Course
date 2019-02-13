#include "Texture.h"


Texture::Texture(){
	textureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = (char*)"";
}

Texture::Texture(char *fileLoc) {
	textureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = fileLoc;
}

void Texture::LoadTexture() {
  unsigned char *texData = stbi_load(fileLocation, &width, &height, &bitDepth, 0);                      // LOAD TEXTURE FROM IMAGE USING STB_IMAGE LIBRARY
  if (!texData) {
    printf("Failed to find %s\n", fileLocation);
    return;
  }

  glGenTextures(1, &textureID);                                                                         // GENERATE THE TEXTURE AND BIND IT
  glBindTexture(GL_TEXTURE_2D, textureID);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);                                         // SET TEXTURE WRAP PARAMETERS
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);                                     // SET TEXTURE PARAMETERS FOR ZOOM IN AND ZOOM OUT
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);        // LOAD THE TEXTURE
  glGenerateMipmap(GL_TEXTURE_2D);

  glBindTexture(GL_TEXTURE_2D, 0);                                                                      // UNBIND THE TEXTURE AND IMAGE
  stbi_image_free(texData);
}

void Texture::UseTexture() {
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, textureID);
}

void Texture::ClearTexture() {
  glDeleteTextures(1, &textureID);
  textureID = 0;
  width = 0;
  height = 0;
  bitDepth = 0;
  fileLocation = (char*)"";
}

Texture::~Texture() {
	ClearTexture();
}