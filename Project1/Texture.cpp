#include "Texture.h"
#include "stb_image.h"
#include <iostream>

Texture::Texture(const char* filepath) {
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, channels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* imgData = stbi_load(filepath, &width, &height, &channels, 4);

    if (imgData) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgData);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(imgData);
    }
    else {
        std::cerr << "Chyba pri nacitani textury: " << filepath << std::endl;
    }
}

void Texture::bind(int textureUnit) const {
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_2D, id);
}