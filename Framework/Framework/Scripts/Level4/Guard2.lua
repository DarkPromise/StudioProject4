Guard = {
InformationComponent = {
    name = "Guard",
    position = {6,25},
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
    numberOfPoints = 6,
    point1 = {6,25},
    point2 = {8,25},
    point3 = {8,22},
    point4 = {6,22},
    point5 = {8,22},
    point6 = {8,25},
  },
}
