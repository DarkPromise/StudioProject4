Guard = {
InformationComponent = {
name = "Guard",
position = {10,2},
direction = {0,1,0},
rotation = {0,0,0},
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
aiSightLength = 4,
},
WaypointComponent = {
numberOfPoints = 6,
point2 = {4,2},
point3 = {4,13},
point4 = {4,2},
point5 = {11,2},
point6 = {11,4},
point1 = {11,2},
},
}
