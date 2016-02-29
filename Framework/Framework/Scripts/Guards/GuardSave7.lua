Guard = {
InformationComponent = {
name = "Guard",
position = {9,11},
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
point2 = {7,11},
point3 = {7,4},
point4 = {9,4},
point1 = {9,11},
},
}
