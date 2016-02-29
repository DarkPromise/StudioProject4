Guard = {
InformationComponent = {
name = "Guard",
position = {8,25},
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
numberOfPoints = 6,
point2 = {8,22},
point3 = {6,22},
point4 = {8,22},
point5 = {8,25},
point6 = {6,25},
point1 = {8,25},
},
}
