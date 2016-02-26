Guard = {
InformationComponent = {
name = "Guard",
position = {1,27},
direction = {0,-1,0},
rotation = {1.8e+002,0,0},
type = "NPC",
},
GraphicsComponent = {
meshType = "Quad",
meshName = "Guard Mesh",
meshImage = "Images//guard.tga",
meshSize = 32,
meshColor = {1, 1, 1},
meshAlpha = 1,
},
AIComponent = {
aiState = "Patrol",
aiType = "Guard",
aiDifficulty = "Normal",
aiSightLength = 5,
},
WaypointComponent = {
numberOfPoints = 2,
point1 = {1,27},
point2 = {1,27},
},
}
