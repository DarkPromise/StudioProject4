#ifndef MODEL_H
#define MODEL_H

#include <iostream>

class Model
{
public:
	Model(void);
	virtual ~Model(void);

	Model * getInstance() { return this; }

	void Init();
	void Update(double dt);
private:
};

#endif