Guard = {
InformationComponent = {
    name = "Guard",
    position = {7,11},
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
    point1 = {7,11},
    point2 = {7,4},
    point3 = {9,4},
    point4 = {9,11},
  },
}
