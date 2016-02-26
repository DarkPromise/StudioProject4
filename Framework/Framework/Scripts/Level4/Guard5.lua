Guard = {
InformationComponent = {
    name = "Guard",
    position = {4,13},
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
    aiSightLength = 4,
  },
  WaypointComponent = {
    numberOfPoints = 6,
    point1 = {4,13},
    point2 = {4,2},
    point3 = {11,2},
    point4 = {11,4},
    point5 = {11,2},
    point6 = {4,2},
  },
}
