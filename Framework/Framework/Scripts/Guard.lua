Guard = {
InformationComponent = {
    name = "Guard",
    position = {16,5},
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
    aiSightRadius = 1,
  },
  WaypointComponent = {
    numberOfPoints = 8,
    point1 = {0,0},
    point2 = {0,0},
    point3 = {0,0},
    point4 = {0,0},
    point5 = {0,0},
    point6 = {0,0},
    point7 = {0,0},
    point8 = {0,0},
  },
}
