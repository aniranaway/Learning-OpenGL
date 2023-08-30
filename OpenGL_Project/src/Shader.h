#pragma once

class Shader
{
public:

private:
	Shader(const std::string& filepath);
	~Shader();

	void Bind();
	void Unbind();

};

