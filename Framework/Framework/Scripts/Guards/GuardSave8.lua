Guard = {
InformationComponent = {
name = "Guard",
position = {5,23},
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
aiSightLength = 3,
},
WaypointComponent = {
numberOfPoints = 4,
point2 = {21,23},
point3 = {21,8},
point4 = {3,8},
point1 = {3,23},
},
}
