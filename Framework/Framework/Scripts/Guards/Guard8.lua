Guard = {
InformationComponent = {
    name = "Guard",
    position = {3,8},
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
    aiSightLength = 3,
  },
  WaypointComponent = {
    numberOfPoints = 4,
    point1 = {3,8},
    point2 = {3,23},
    point3 = {21,23},
    point4 = {21,8},
  },
}
