#pragma once
#ifndef _SHAPE_H_
#define _SHAPE_H_

#include <string>
#include <vector>

class ShaderProgram;
class GameObject;

/**
 * A shape defined by a list of triangles
 * - posBuf should be of length 3*ntris
 * - norBuf should be of length 3*ntris (if normals are available)
 * - texBuf should be of length 2*ntris (if texture coords are available)
 * posBufID, norBufID, and texBufID are OpenGL buffer identifiers.
 */
class Mesh
{
public:
	Mesh(const std::string &meshName);
	virtual ~Mesh();
	void loadMesh();
	void init();
	//void draw(const std::shared_ptr<Shader> prog, std::shared_ptr<GameObject> torso) const;
	void bind(ShaderProgram* prog);
	void unbind(ShaderProgram* prog);
	size_t vertCount() { return posBuf.size() / 3; }

private:
	std::string meshName;
	std::vector<float> posBuf;
	std::vector<float> norBuf;
	std::vector<float> texBuf;
	unsigned posBufID;
	unsigned norBufID;
	unsigned texBufID;
};

#endif
