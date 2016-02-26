Guard = {
InformationComponent = {
    name = "Guard",
    position = {1,27},
    direction = {0,-1,0},
    rotation = {180,0,0},
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
    aiSightLength = 3,
  },
  WaypointComponent = {
    numberOfPoints = 2,
    point1 = {1,27},
    point2 = {1,27},
  },
}
