#include "shaders.h"

Shader::Shader (const char* vertexPath, const char* fragmentPath)
{
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vertexShaderFile;
    std::ifstream fragmentShaderFile;

    vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    //attempt to read the shader files
    try
    {
        vertexShaderFile.open(vertexPath);
        fragmentShaderFile.open(fragmentPath);
        std::stringstream vertexShaderStream, fragmentShaderStream;

        vertexShaderStream << vertexShaderFile.rdbuf();
        fragmentShaderStream << fragmentShaderFile.rdbuf();

        vertexShaderFile.close();
        fragmentShaderFile.close();

        vertexCode = vertexShaderStream.str();
        fragmentCode = fragmentShaderStream.str();
    }
    catch(std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
    }

    const char* vertexShaderCode = vertexCode.c_str();
    const char* fragmentShaderCode = fragmentCode.c_str();

    //once code is successfully obtained, begin compilation and linking
    int compiled, linked;
    char infoLog[512];

    //vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderCode, NULL);
    glCompileShader(vertexShader);

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compiled);
    if (!compiled)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILIATION_FAILED\n" << infoLog << std::endl;
    }

    //fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderCode, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compiled);
    if (!compiled)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILIATION_FAILED\n" << infoLog << std::endl;
    }

    //generate shader program
    ID = glCreateProgram();
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    glLinkProgram(ID);

    glGetProgramiv(ID, GL_LINK_STATUS, &linked);
    if (!linked)
    {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::use()
{
    glUseProgram(ID);
}

void Shader::setBool(const std::string &name, bool value) const
{
    GLint location = glGetUniformLocation(ID, name.c_str());
    if (location == -1) {
        std::cerr << "ERROR::UNIFORM::" << name << "::NOT_FOUND\n";
        return;
    }

    glUniform1i(location, (int)value);
}

void Shader::setInt(const std::string &name, int value) const
{
    GLint location = glGetUniformLocation(ID, name.c_str());
    if (location == -1) {
        std::cerr << "ERROR::UNIFORM::" << name << "::NOT_FOUND\n";
        return;
    }

    glUniform1i(location, value);
}

void Shader::setFloat(const std::string &name, float value) const
{
    GLint location = glGetUniformLocation(ID, name.c_str());
    if (location == -1) {
        std::cerr << "ERROR::UNIFORM::" << name << "::NOT_FOUND\n";
        return;
    }

    glUniform1f(location, value);
}

void Shader::setVec2(const std::string &name, const glm::vec2 &vec) const
{
    GLint location = glGetUniformLocation(ID, name.c_str());
    if (location == -1) {
        std::cerr << "ERROR::UNIFORM::" << name << "::NOT_FOUND\n";
        return;
    }

    glUniform2fv(location, 1, &vec[0]);
}

void Shader::setVec3(const std::string &name, const glm::vec3 &vec) const
{
    GLint location = glGetUniformLocation(ID, name.c_str());
    if (location == -1) {
        std::cerr << "ERROR::UNIFORM::" << name << "::NOT_FOUND\n";
        return;
    }

    glUniform3fv(location, 1, &vec[0]);
}

void Shader::setVec4(const std::string &name, const glm::vec4 &vec) const
{
    GLint location = glGetUniformLocation(ID, name.c_str());
    if (location == -1) {
        std::cerr << "ERROR::UNIFORM::" << name << "::NOT_FOUND\n";
        return;
    }

    glUniform4fv(location, 1, &vec[0]);
}

void Shader::setMat2(const std::string &name, const glm::mat2 &mat) const
{
    GLint location = glGetUniformLocation(ID, name.c_str());
    if (location == -1) {
        std::cerr << "ERROR::UNIFORM::" << name << "::NOT_FOUND\n";
        return;
    }

    glUniformMatrix2fv(location, 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat3(const std::string &name, const glm::mat3 &mat) const
{
    GLint location = glGetUniformLocation(ID, name.c_str());
    if (location == -1) {
        std::cerr << "ERROR::UNIFORM::" << name << "::NOT_FOUND\n";
        return;
    }

    glUniformMatrix3fv(location, 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const
{
    GLint location = glGetUniformLocation(ID, name.c_str());
    if (location == -1) {
        std::cerr << "ERROR::UNIFORM::" << name << "::NOT_FOUND\n";
        return;
    }

    glUniformMatrix4fv(location, 1, GL_FALSE, &mat[0][0]);
}
