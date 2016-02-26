Guard = {
InformationComponent = {
name = "Guard",
position = {9,4},
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
aiState = "Idle",
aiType = "Guard",
aiDifficulty = "Normal",
aiSightLength = 5,
},
WaypointComponent = {
numberOfPoints = 4,
point1 = {7,11},
point2 = {7,4},
point3 = {9,4},
point4 = {9,11},
},
}
