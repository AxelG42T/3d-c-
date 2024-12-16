#include "Texture.h"

Texture::Texture(std::string path)
{
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nbChannels;
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &nbChannels, 0);

    if (data) {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to load resources !" << std::endl;
    }
    stbi_image_free(data);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::activate()
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
}

void Texture::deactivate()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}
