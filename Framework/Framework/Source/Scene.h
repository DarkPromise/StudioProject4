#ifndef SCENE_H
#define SCENE_H

// Scene is abstract

class Scene
{
public:
	Scene();
	~Scene();

	virtual void Init() = 0;
	virtual void Update(double dt) = 0;
	virtual void Render() = 0;
private:
};

#endif