Guard = {
InformationComponent = {
    name = "Guard",
    position = {22,5},
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
    numberOfPoints = 2,
    point1 = {22,5},
    point2 = {22,15},
  },
}
