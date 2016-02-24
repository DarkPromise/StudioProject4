Guard = {
InformationComponent = {
    name = "Guard",
    position = {18,5},
    direction = {1,0,0},
    rotation = {90,0,0},
    type = "NPC",
  },
GraphicsComponent = {
    meshType = "Quad",
    meshName = "Guard Mesh",
    meshImage = "Images//guard.tga",
    meshSize = 32,
    meshColor = {1,1,1},
    meshAlpha = 1,
  },
 AIComponent = {
    aiState = "Idle",
    aiType = "Guard",
    aiDifficulty = "Normal",
    aiSightLength = 5,
  },
  WaypointComponent = {
    numberOfPoints = 4,
    point1 = {18,5},
    point2 = {16,5},
    point3 = {16,10},
    point4 = {18,10},
  },
}
