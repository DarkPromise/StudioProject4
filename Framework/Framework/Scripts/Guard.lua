Guard = {
InformationComponent = {
    name = "Guard",
    position = {1,2,3},
    direction = {0,0,0},
    size = 10,
    type = "NPC",
  },
GraphicsComponent = {
    meshType = "Quad",
    meshName = "Guard Mesh",
    meshImage = "guard.tga",
    meshSize = 32,
    meshColor = {1,1,1},
    meshAlpha = 1,
  },
CollisionComponent = {
    min = {-5,-5,-5},
    max = {5,5,5},
  },
HealthComponent = {
    minHp = "100",
    maxHp = "100",
  },
}
