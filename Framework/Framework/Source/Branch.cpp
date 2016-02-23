#include "Branch.h"
#include "Entity.h"
#include "View.h"

Branch::Branch(Branch * parent, int level, Vector3 center, Vector3 size)
{
	this->parent = parent;
	this->m_iLevel = level;
	this->m_cBranchCenter = center;

	this->m_cMax = (center + (size * 0.5f));
	this->m_cMin = (center - (size * 0.5f));

	// Initialize the children
	for (unsigned int i = 0; i < 8; i++)
	{
		child[i] = NULL;
	}

	this->m_color = Color((float)level, (float)level, (float)level);
	this->m_cMesh = MeshBuilder::GenerateBoundingBox("Branch", this->m_cMax, this->m_cMin, m_color);
}

Branch::~Branch()
{
	if (m_cMesh)
	{
		delete m_cMesh;
	}

	for (unsigned int i = 0; i < 8; i++)
	{
		delete child[i];
	}

	m_entityList.clear();
	m_entityList.~vector();
}

Branch * Branch::getRoot() //Recursive to get root node
{
	if (parent == NULL)
	{
		return this;
	}
	else
	{
		return parent->getRoot();
	}
}

bool Branch::canContain(Entity * entity)
{
	float nLowX = entity->getComponent<CollisionComponent>()->getAABB()->Min.x + entity->getComponent<InformationComponent>()->getPosition().x;
	float nLowY = entity->getComponent<CollisionComponent>()->getAABB()->Min.y + entity->getComponent<InformationComponent>()->getPosition().y;
	float nLowZ = entity->getComponent<CollisionComponent>()->getAABB()->Min.z + entity->getComponent<InformationComponent>()->getPosition().z;

	float nHighX = entity->getComponent<CollisionComponent>()->getAABB()->Max.x + entity->getComponent<InformationComponent>()->getPosition().x;
	float nHighY = entity->getComponent<CollisionComponent>()->getAABB()->Max.y + entity->getComponent<InformationComponent>()->getPosition().y;
	float nHighZ = entity->getComponent<CollisionComponent>()->getAABB()->Max.z + entity->getComponent<InformationComponent>()->getPosition().z;

	return(nLowX > m_cMin.x && nHighX < m_cMax.x && nLowY > m_cMin.y && nHighY < m_cMax.y && nLowZ > m_cMin.z && nHighZ < m_cMax.z);
}

void Branch::addObject(Entity * entity)
{
	if (child[0] == NULL)
	{
		// Entity Belongs to this partition
		entity->getComponent<InformationComponent>()->partition = this;
		this->m_entityList.push_back(entity);
		return;
	}

	// Get its relative position in the branch
	int test = 0;

	test += (entity->getComponent<InformationComponent>()->getPosition().x < this->m_cBranchCenter.x) ? 0 : 1;
	test += (entity->getComponent<InformationComponent>()->getPosition().y < this->m_cBranchCenter.y) ? 2 : 0;
	test += (entity->getComponent<InformationComponent>()->getPosition().z < this->m_cBranchCenter.z) ? 4 : 0;

	delete this->getMesh();
	this->setMesh(MeshBuilder::GenerateBoundingBox("Updated", this->m_cMax, this->m_cMin, Color(1.f, 0.f, 0.f)));

	if (child[test]->canContain(entity))
	{
		child[test]->addObject(entity);
	}
	else
	{
		entity->getComponent<InformationComponent>()->partition = this;
		this->m_entityList.push_back(entity);
	}
}

void Branch::removeObject(int ID)
{
	std::vector<Entity*>::iterator itr;
	for (itr = m_entityList.begin(); itr != m_entityList.end(); itr++)
	{
		if ((*itr)->getComponent<InformationComponent>()->getUID() == ID)
		{
			m_entityList.erase(itr);
			return;
		}
	}
}

void Branch::checkCollisions(int & noTests, int & noCollisions, int & score)
{
	if (hasChild())
	{
		for (unsigned int i = 0; i < 8; i++)
		{
			child[i]->checkCollisions(noTests, noCollisions, score);
		}
	}

	int numberofobjects = this->m_entityList.size();
	if (numberofobjects >= 2)
	{
		for (int i = 0; i < (numberofobjects - 1); i++)
		{
			for (int j = i + 1; j < numberofobjects; j++)
			{
				noTests++;

				auto * collisionC = m_entityList[i]->getComponent<CollisionComponent>();
				auto * collisionC2 = m_entityList[j]->getComponent<CollisionComponent>();

				if (collisionC->collidesWith(collisionC2,score) > 0)
				{
					noCollisions++;
				}
			}
		}
	}
	if (numberofobjects >= 1 && parent != NULL)
	{
		parent->checkBorderCollisions(this, noTests, noCollisions, score);
	}
}

void Branch::UpdateAIComponents(double dt, Entity * thePlayer)
{
	if (hasChild())
	{
		for (unsigned int i = 0; i < 8; i++)
		{
			child[i]->UpdateAIComponents(dt,thePlayer);
		}
	}

	for (Entity * entity : m_entityList)
	{
		auto aiComponent = entity->getComponent<AIComponent>();
		if (aiComponent)
		{
			//aiComponent->Update(dt,thePlayer);
		}
	}
}

void Branch::UpdateLevelOfDetails(Vector3 camPos)
{
	if (hasChild())
	{
		for (unsigned int i = 0; i < 8; i++)
		{
			child[i]->UpdateLevelOfDetails(camPos);
		}
	}

	Vector3 distanceToSquare = this->m_cBranchCenter - camPos;
	float distanceSquared = (distanceToSquare.x*distanceToSquare.x + distanceToSquare.z*distanceToSquare.z);

	for (Entity * entity : m_entityList)
	{
		auto infoC = entity->getComponent<InformationComponent>();
		if (infoC)
		{
			auto gC = entity->getComponent<GraphicsComponent>();
			if (gC)
			{
				if (distanceSquared < 50000.f)
				{
					gC->setResLevel(2); // High
				}
				else if (distanceSquared < 150000.f)
				{
					gC->setResLevel(1); // Med
				}
				else
				{
					gC->setResLevel(0); // Low
				}
			}
		}
	}
}

void Branch::checkBorderCollisions(Branch * branch, int & noTests, int & noCollisions, int & score)
{
	int numberofobjectsBranch = branch->m_entityList.size();
	int numberofobjects = this->m_entityList.size();

	if (numberofobjects > 0)
	{
		for (int i = 0; i < numberofobjectsBranch; i++)
		{
			for (int j = 0; j < numberofobjects; j++)
			{
				auto * collisionC = branch->m_entityList[i]->getComponent<CollisionComponent>();
				auto * collisionC2 = this->m_entityList[j]->getComponent<CollisionComponent>();

				if (collisionC->collidesWith(collisionC2, score) > 0)
				{
					noCollisions++;
				}
			}
		}
	}
	if (parent != NULL)
	{
		parent->checkBorderCollisions(branch, noTests, noCollisions, score);
	}
}

void Branch::createSubPartitions(int noLevels)
{
	if (this->m_iLevel < (noLevels - 1))
	{
		Vector3 newSize = (this->m_cMax - this->m_cMin) * 0.5f;

		Vector3 newCenter = Vector3(this->m_cBranchCenter.x - newSize.x * 0.5f, this->m_cBranchCenter.y + newSize.y * 0.5f, this->m_cBranchCenter.z + newSize.z * 0.5f);
		child[0] = new Branch(this, m_iLevel + 1, newCenter, newSize);
		newCenter = Vector3(this->m_cBranchCenter.x + newSize.x * 0.5f, this->m_cBranchCenter.y + newSize.y * 0.5f, this->m_cBranchCenter.z + newSize.z * 0.5f);
		child[1] = new Branch(this, m_iLevel + 1, newCenter, newSize);
		newCenter = Vector3(this->m_cBranchCenter.x - newSize.x * 0.5f, this->m_cBranchCenter.y - newSize.y * 0.5f, this->m_cBranchCenter.z + newSize.z * 0.5f);
		child[2] = new Branch(this, m_iLevel + 1, newCenter, newSize);
		newCenter = Vector3(this->m_cBranchCenter.x + newSize.x * 0.5f, this->m_cBranchCenter.y - newSize.y * 0.5f, this->m_cBranchCenter.z + newSize.z * 0.5f);
		child[3] = new Branch(this, m_iLevel + 1, newCenter, newSize);
		newCenter = Vector3(this->m_cBranchCenter.x - newSize.x * 0.5f, this->m_cBranchCenter.y + newSize.y * 0.5f, this->m_cBranchCenter.z - newSize.z * 0.5f);
		child[4] = new Branch(this, m_iLevel + 1, newCenter, newSize);
		newCenter = Vector3(this->m_cBranchCenter.x + newSize.x * 0.5f, this->m_cBranchCenter.y + newSize.y * 0.5f, this->m_cBranchCenter.z - newSize.z * 0.5f);
		child[5] = new Branch(this, m_iLevel + 1, newCenter, newSize);
		newCenter = Vector3(this->m_cBranchCenter.x - newSize.x * 0.5f, this->m_cBranchCenter.y - newSize.y * 0.5f, this->m_cBranchCenter.z - newSize.z * 0.5f);
		child[6] = new Branch(this, m_iLevel + 1, newCenter, newSize);
		newCenter = Vector3(this->m_cBranchCenter.x + newSize.x * 0.5f, this->m_cBranchCenter.y - newSize.y * 0.5f, this->m_cBranchCenter.z - newSize.z * 0.5f);
		child[7] = new Branch(this, m_iLevel + 1, newCenter, newSize);

		for (int i = 0; i < 8; i++)
		{
			child[i]->createSubPartitions(noLevels);
		}
	}
}

void Branch::PrintInfo()
{
	std::cout << "Level : " << this->m_iLevel << std::endl;;
	std::cout << "Center : " << this->m_cBranchCenter << std::endl;
	std::cout << "Max : " << this->m_cMax << std::endl;
	std::cout << "Min : " << this->m_cMin << std::endl;
	std::cout << "Size : " << this->m_entityList.size() << std::endl;
	if (child[0] != NULL)
	{
		for (int i = 0; i < MAX_SUBPARTITIONS; i++)
		{
			child[i]->PrintInfo();
		}
	}
}

void Branch::PrintListInfo()
{
	if (m_entityList.size() > 0)
	{
		for (unsigned int i = 0; i < this->m_entityList.size(); i++)
		{
			std::cout << "Index : " << i << std::endl;
			std::cout << "Name : " << m_entityList[i]->getComponent<InformationComponent>()->toString() << std::endl;
			std::cout << "Position : " << m_entityList[i]->getComponent<InformationComponent>()->getPosition() << std::endl;
			std::cout << "AABB Max : " << m_entityList[i]->getComponent<CollisionComponent>()->getAABB()->Max << std::endl;
			std::cout << "AABB Min : " << m_entityList[i]->getComponent<CollisionComponent>()->getAABB()->Min << std::endl;
			std::cout << "Address : " << m_entityList[i] << std::endl;
		}
	}

	if (child[0] != NULL)
	{
		for (int i = 0; i < MAX_SUBPARTITIONS; i++)
		{
			child[i]->PrintListInfo();
		}
	}
}

void Branch::Render(View * theView)
{
	if (hasChild())
	{
		for (int i = 0; i < MAX_SUBPARTITIONS; i++)
		{
			child[i]->Render(theView);
		}
	}
	if (m_entityList.size() > 0)
	{
		glLineWidth(2.0f);
		theView->RenderTreeBranch(this);
		glLineWidth(1.0f);
	
		/*Vector3 distanceToSquare = this->m_cBranchCenter - theView->;
		float distanceSquared = (distanceToSquare.x*distanceToSquare.x + distanceToSquare.z*distanceToSquare.z);*/

		
		for (int i = 0; i < m_entityList.size(); i++)
		{
			theView->modelStack.PushMatrix();
			theView->modelStack.Translate(this->m_entityList[i]->getComponent<InformationComponent>()->getPosition().x, this->m_entityList[i]->getComponent<InformationComponent>()->getPosition().y, this->m_entityList[i]->getComponent<InformationComponent>()->getPosition().z);
			theView->modelStack.Rotate(this->m_entityList[i]->getComponent<InformationComponent>()->getRotation().y, 0.f, 1.f, 0.f);
			theView->RenderMesh(m_entityList[i]->getComponent<GraphicsComponent>()->getMesh(m_entityList[i]->getComponent<GraphicsComponent>()->getResLevel()), false, false);
			theView->modelStack.PopMatrix();
		}
	}
}

Mesh * Branch::getMesh()
{
	return this->m_cMesh;
}

Vector3 Branch::getCenter()
{
	return this->m_cBranchCenter;
}

int Branch::getLevel()
{
	return this->m_iLevel;
}

void Branch::setMesh(Mesh * mesh)
{
	this->m_cMesh = mesh;
}